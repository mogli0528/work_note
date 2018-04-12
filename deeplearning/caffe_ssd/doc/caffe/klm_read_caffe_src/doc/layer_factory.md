# layer_factory 的实现  
实现了 Layer 的注册功能. 在运行时调用 CreateLayer(const LayerParameter& ) 函数来注册层.    

## 目录   
- 两种注册层的方式   
- layer_factory.hpp 的实现    
- layer_factory.cpp 的实现 

## 两种注册层的方式    
1. REGISTER_LAYER_CLASS()    
```cpp
template <typename Dtype>
class MyAwesomeLayer : public Layer<Dtype> {
  // your implementations
};
```
类名去掉子串 "Layer" 就是这个层的 name ("MyAwesomeLayer" -> "MyAwesome").
如果是使用构造函数来创建 layer, 在 c++ 代码中加上以下这行:   
```cpp
REGISTER_LAYER_CLASS(MyAwesome);
```
2. REGISTER_LAYER_CREATOR()   
```cpp
template <typename Dtype>
Layer<Dtype*> GetMyAwesomeLayer(const LayerParameter& param) {
 // your implementation
}
```
例如, 当你的层有多个末端时(例如 GetConvolutionLayer 中的 GPU 层和 CPU 层), 你可以直接出则 creator 函数.   
```cpp
REGISTER_LAYER_CREATOR(MyAwesome, GetMyAwesomeLayer)
```
Note:   
所有的层都只能被注册一次.    

## layer_factory.hpp 的实现    
1. private 成员.    
```cpp
template <typename Dtype>
class LayerRegistry {
 public:
  ...
 
 private:
  // 私有成员, 说明 Layer registry 不能被实例化, 使用类中定义的 static 成员函数完成注册功能.  
  LayerRegistry() {}  

  // 将所有注册过的层的 type name 用逗号连接后返回.   
  static string LayerTypeListString() {
    vector<string> layer_types = LayerTypeList();
    string layer_types_str;
    for (vector<string>::iterator iter = layer_types.begin();
         iter != layer_types.end(); ++iter) {
      if (iter != layer_types.begin()) {
        layer_types_str += ", ";
      }
      layer_types_str += *iter;
    }
    return layer_types_str;
  }
};
```
2. public 成员.   
```cpp
template <typename Dtype>
class LayerRegistry {
 public:
  typedef shared_ptr<Layer<Dtype> > (*Creator)(const LayerParameter&);
  typedef std::map<string, Creator> CreatorRegistry;

  // g_registry_ 是 static 成员, 不管调用该函数多少次, 只会在首次对它进行初始化.  
  static CreatorRegistry& Registry() {
    static CreatorRegistry* g_registry_ = new CreatorRegistry();
    return *g_registry_;
  }

  // 添加一个 creator, 用关联容器 map 管理.
  static void AddCreator(const string& type, Creator creator) {
    CreatorRegistry& registry = Registry();
    CHECK_EQ(registry.count(type), 0)
        << "Layer type " << type << " already registered.";
    registry[type] = creator;
  }

  // 使用 LayerParameter 参数获取一个 Layer , 调用的是 Layer 的构造函数.
  static shared_ptr<Layer<Dtype> > CreateLayer(const LayerParameter& param) {
    if (Caffe::root_solver()) {
      LOG(INFO) << "Creating layer " << param.name();
    }
    const string& type = param.type();
    CreatorRegistry& registry = Registry();
    CHECK_EQ(registry.count(type), 1) << "Unknown layer type: " << type
        << " (known types: " << LayerTypeListString() << ")";
    return registry[type](param);
  }

  // 将所有 layer 的类型名用 vector 保存后返回.   
  static vector<string> LayerTypeList() {
    CreatorRegistry& registry = Registry();
    vector<string> layer_types;
    for (typename CreatorRegistry::iterator iter = registry.begin();
         iter != registry.end(); ++iter) {
      layer_types.push_back(iter->first);
    }
    return layer_types;
  }

 private:
  ...
};

// 注册一个新的层.   
template <typename Dtype>
class LayerRegisterer {
 public:
  LayerRegisterer(const string& type,
                  shared_ptr<Layer<Dtype> > (*creator)(const LayerParameter&)) {
    // LOG(INFO) << "Registering layer type: " << type;
    LayerRegistry<Dtype>::AddCreator(type, creator);
  }
};

// 定义两个 LayerRegisterer 类型的 static 全局变量, 构造函数调用 AddCreator() 函数.  
#define REGISTER_LAYER_CREATOR(type, creator)                                  \
  static LayerRegisterer<float> g_creator_f_##type(#type, creator<float>);     \
  static LayerRegisterer<double> g_creator_d_##type(#type, creator<double>)    \

// 调用层的构造函数返回一个智能指针.     
#define REGISTER_LAYER_CLASS(type)                                             \
  template <typename Dtype>                                                    \
  shared_ptr<Layer<Dtype> > Creator_##type##Layer(const LayerParameter& param) \
  {                                                                            \
    return shared_ptr<Layer<Dtype> >(new type##Layer<Dtype>(param));           \
  }                                                                            \
  REGISTER_LAYER_CREATOR(type, Creator_##type##Layer)
```

## layer_factory.cpp 的实现 
1. 获取卷积层
GetConvolutionLayer() 根据 engine 是否为 GPU 來获取对应的卷积层(ConvolutionLayer 或 CuDNNConvolutionLayer).  
TODO: dilation 用于膨胀 kernel 的因子(cudnn 中不支持扩展后的卷积运算). (Kernel dilation is sometimes referred to by its use in the algorithme à trous from Holschneider et al. 1987.)   
```
template <typename Dtype>
shared_ptr<Layer<Dtype> > GetConvolutionLayer(
    const LayerParameter& param) {
  ConvolutionParameter conv_param = param.convolution_param();
  ConvolutionParameter_Engine engine = conv_param.engine();
#ifdef USE_CUDNN
  bool use_dilation = false;
  for (int i = 0; i < conv_param.dilation_size(); ++i) {
    if (conv_param.dilation(i) > 1) {
      use_dilation = true;   
    }
  }
#endif
  if (engine == ConvolutionParameter_Engine_DEFAULT) {
    engine = ConvolutionParameter_Engine_CAFFE;
#ifdef USE_CUDNN
    if (!use_dilation) {
      engine = ConvolutionParameter_Engine_CUDNN;
    }
#endif
  }
  if (engine == ConvolutionParameter_Engine_CAFFE) {
    return shared_ptr<Layer<Dtype> >(new ConvolutionLayer<Dtype>(param));
#ifdef USE_CUDNN
  } else if (engine == ConvolutionParameter_Engine_CUDNN) {
    if (use_dilation) {
      LOG(FATAL) << "CuDNN doesn't support the dilated convolution at Layer "
                 << param.name();
    }
    return shared_ptr<Layer<Dtype> >(new CuDNNConvolutionLayer<Dtype>(param));
#endif
  } else {
    LOG(FATAL) << "Layer " << param.name() << " has unknown engine.";
    throw;  // Avoids missing return warning
  }
}

REGISTER_LAYER_CREATOR(Convolution, GetConvolutionLayer);
```

## 获取 Pooling, LRN, ReLU, Sigmoid, TanH, Python层
GetPoolingLayer(), GetLRNLayer(), GetReLULayer(), GetSigmoidLayer(), GetTanHLayer(), GetPythonLayer() 同样是根据 engine 是否为 GPU 來获取对应的层.    
