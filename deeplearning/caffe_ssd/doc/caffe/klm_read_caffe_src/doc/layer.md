# Layer 的实现   
Layer 是 Caffe 中最庞大最繁杂的模块。由于 Caffe 强调模块化设计, 因此只允许每个 Layer完成一类特定的计算. 例如 convolution 操作、 pooling 、 非线性变换 、 内积运算 、数据加载、归一化和损失计算等.     
Layer 这个类可以说是里面最终的一个基本类了, 深度网络呢就是一层一层的 Layer, 相互之间通过传输 Blob 数据连接起来.   

## 目录    
- Caffe 中的 Layers   
- layer.hpp 文件的实现
- layer.cpp 文件的实现
- 协调使用 CPU 和 GPU 设备.   
- Layer 相关的 Proto  

## Caffe 中的 Layers   
Caffe 中与 Layer 相关的头文件有 7 个:    

- layer.hpp: 父类 Layer , 定义所有 layer 的基本接口。   
- data_layers.hpp: 继承自父类 Layer , 定义与`输入数据操作`相关的子 Layer, 例如 DataLayer,  HDF5DataLayer 和 ImageDataLayer 等。  
- vision_layers.hpp: 继承自父类 Layer, 定义与`特征表达相关`的子 Layer, 例如 ConvolutionLayer,  PoolingLayer 和 LRNLayer 等。  
- neuron_layers.hpp: 继承自父类 Layer, 定义与`非线性变换相关`的子 Layer, 例如 ReLULayer, TanHLayer 和 SigmoidLayer 等。  
- loss_layers.hpp: 继承自父类 Layer , 定义与`输出误差计算相关`的子 Layer, 例如 EuclideanLossLayer, SoftmaxWithLossLayer 和 HingeLossLayer 等。   
- common_layers.hpp: 继承自父类 Layer , 定义与`中间结果数据变形`、`逐元素操作相关`的子 Layer, 例如 ConcatLayer, InnerProductLayer 和 SoftmaxLayer 等。  
- layer_factory.hpp: Layer 工厂模式类, 负责维护现有可用 layer 和相应 layer 构造方法的`映射表`。   

下表列出了 Caffe 中所有的 Layers, 其中 Layer 对应的就是 $(Layer).cpp 实现文件.   

|Layer|功能|
|----|-----|
|window_data_layer|TODO|
|hinge_loss_layer|TODO|
|conv_layer|将一系列滤波器对输入卷积, 一个滤波器对应一个特征提取器|
|pooling_layer|保留显著的特征的条件下实现降维|
|lrn_layer|对输入的一个局部区域进行归一化|
|im2col_layer|将图片文件转化为列矩阵, 便于卷积操作和矩阵乘法|
|loss_layer|前向运算中计算 loss|
|softmax_loss_layer|计算输入的多项式回归损失, 计算出的梯度相比 softmax_layer 更可靠|
|sigmoid_cross_entropy_loss_layer| 求解 sigmoid 交叉熵损失|
|relu_layer|ReLu 激活函数|
|sigmoid_layer|sigmoid 激活函数|
|tanh_layer|tanh_layer 激活函数|
|absval_layer|求解每个元素的绝对值|
|power_layer|求解每个元素的幂次, 允许有偏移和缩放|
|bnll_layer|二项正态对数似然, 输出为 log(1 + exp(x))|
|data_layer|基本的输入预处理(例如：减去均值, 缩放, 随机裁剪, 镜像处理)|
|memory_data_layer|读取保存在内存中的数据|
|hdf5_data_layer|读取保存在 HDF5 的数据|
|hdf5_output_layer|写入 HDF5 数据|
|image_data_layer|读取保存在 image 中的数据|
|inner_product_layer|全连接层, 输出为(n * c_o * 1 * 1)|
|split_layer|可以将一个 blob 输入到多个输出 layer |
|flatten_layer|把 `n * c * h * w` 转化为 `n * (c*h*w)`|
|reshape_layer|只改变输入数据的维度, 但内容不变, 也没有数据复制的过程|
|concat_layer|按照指定的轴进行 Blob 的拼接|
|slice_layer|根据给定的维度将一个 input layer 分割成多个 output layers|

## layer.hpp 文件的实现   
layer.hpp 中定义了抽象基类 Layer, 其中包括所有 layer 的基本接口。用户添加新的层时, 只需继承 Layer(抽象基类必须被继承), 然后按照这些接口定义特定的操作即可(因此, layer.cpp 文件是空的).  

- Layer%s 必须实现 Forward(), 通过输入计算输出.
- 也必须实现 Backward(), 计算输入的 error gradients 作为输出.    

1. private 成员    
```cpp
template <typename Dtype>
class Layer {
 public:
  ...
 
 protected:
  LayerParameter layer_param_;  // 保存 layer parameters 的protobuf
  Phase phase_;   // The phase: TRAIN or TEST  
  vector<shared_ptr<Blob<Dtype> > > blobs_;   // vector, 保存有可学习参数的一些列 blobs
  vector<bool> param_propagate_down_;   // 是是否需要计算每一个输入 blob的 diff

  vector<Dtype> loss_;  // 指示目标函数中的每个 top blob 是否有非零权重值

  /** @brief 使用 CPU 计算层的输出. */
  virtual void Forward_cpu(const vector<Blob<Dtype>*>& bottom,
      const vector<Blob<Dtype>*>& top) = 0;
  /**
   * @brief 使用 GPU 计算层的输出..
   *        如果没有 GPU, 则回退到调用 Forward_cpu() 版本.
   */
  virtual void Forward_gpu(const vector<Blob<Dtype>*>& bottom,
      const vector<Blob<Dtype>*>& top) {
    // LOG(WARNING) << "Using CPU code as backup.";
    return Forward_cpu(bottom, top);
  }

  /**
   * @brief 当 propagate_down = true 时, 使用 CPU 计算 bottom blobs 的参数的梯度
   */
  virtual void Backward_cpu(const vector<Blob<Dtype>*>& top,
      const vector<bool>& propagate_down,
      const vector<Blob<Dtype>*>& bottom) = 0;
  /**
   * @brief 当 propagate_down = true 时, 使用 GPU 计算 bottom blobs 的参数的梯度
   *        如果没有 GPU, 则回退到调用 Backward_cpu() 版本.  
   */
  virtual void Backward_gpu(const vector<Blob<Dtype>*>& top,
      const vector<bool>& propagate_down,
      const vector<Blob<Dtype>*>& bottom) {
    // LOG(WARNING) << "Using CPU code as backup.";
    Backward_cpu(top, propagate_down, bottom);
  }

  /**
   * SetUp() 调用这个函数来初始化 loss 函数中 top blobs 的权重值, diff blob 中的  
   * loss 权重值为非零数.
   *
   * Note: 这个函数不能被重写.  
   */
  inline void SetLossWeights(const vector<Blob<Dtype>*>& top) {
    const int num_loss_weights = layer_param_.loss_weight_size();
    if (num_loss_weights) {
      CHECK_EQ(top.size(), num_loss_weights) << "loss_weight must be "
          "unspecified or specified once per top blob.";
      for (int top_id = 0; top_id < top.size(); ++top_id) {
        const Dtype loss_weight = layer_param_.loss_weight(top_id);
        if (loss_weight == Dtype(0)) { continue; }
        this->set_loss(top_id, loss_weight);
        const int count = top[top_id]->count();
        Dtype* loss_multiplier = top[top_id]->mutable_cpu_diff();
        caffe_set(count, loss_weight, loss_multiplier);
      }
    }
  }

  /**
   * SetUp() 会调用这个函数来检查 bottom 和 top Blobs 的个数是否和
   * 这几个 {ExactNum,Min,Max}{Bottom,Top}Blobs() 函数指定的 numbers 相匹配.
   *
   * {ExactNum,Min,Max}{Bottom,Top}Blobs() 函数分别是:  
   *   ExactNumBottomBlobs(), MinBottomBlobs() 和 MaxBottomBlobs()
   *   ExactNumTopBlobs(), MinTopBlobs() 和 MaxTopBlobs()
   */
  virtual void CheckBlobCounts(const vector<Blob<Dtype>*>& bottom,
                               const vector<Blob<Dtype>*>& top) {
    if (ExactNumBottomBlobs() >= 0) {
      CHECK_EQ(ExactNumBottomBlobs(), bottom.size())
          << type() << " Layer takes " << ExactNumBottomBlobs()
          << " bottom blob(s) as input.";
    }
    if (MinBottomBlobs() >= 0) {
      CHECK_LE(MinBottomBlobs(), bottom.size())
          << type() << " Layer takes at least " << MinBottomBlobs()
          << " bottom blob(s) as input.";
    }
    if (MaxBottomBlobs() >= 0) {
      CHECK_GE(MaxBottomBlobs(), bottom.size())
          << type() << " Layer takes at most " << MaxBottomBlobs()
          << " bottom blob(s) as input.";
    }
    if (ExactNumTopBlobs() >= 0) {
      CHECK_EQ(ExactNumTopBlobs(), top.size())
          << type() << " Layer produces " << ExactNumTopBlobs()
          << " top blob(s) as output.";
    }
    if (MinTopBlobs() >= 0) {
      CHECK_LE(MinTopBlobs(), top.size())
          << type() << " Layer produces at least " << MinTopBlobs()
          << " top blob(s) as output.";
    }
    if (MaxTopBlobs() >= 0) {
      CHECK_GE(MaxTopBlobs(), top.size())
          << type() << " Layer produces at most " << MaxTopBlobs()
          << " top blob(s) as output.";
    }
    if (EqualNumBottomTopBlobs()) {
      CHECK_EQ(bottom.size(), top.size())
          << type() << " Layer produces one top blob as output for each "
          << "bottom blob input.";
    }
  }

 private:
  bool is_shared_;  // 其他网络是否也共享这一层
  shared_ptr<boost::mutex> forward_mutex_; // 如果这一层是共享的, 那么使用这个前向运算锁

  void InitMutex();  // 初始化 forward_mutex_
  void Lock();    // 为 forward_mutex_ 上锁
  void Unlock();  // 为 forward_mutex_ 解锁

  DISABLE_COPY_AND_ASSIGN(Layer);
```

2. public 成员  
```cpp
template <typename Dtype>
class Layer {
 public:
  /**
   * 用户自定义 layer 时, 不必实现自己的构造函数. 所有的初始化代码都放在 SetUp() 中,
   * SetUp(onst vector<Blob<Dtype>*>& bottom, const vector<Blob<Dtype>*>& top)
   * 函数会提供 bottom blobs 的维度参数信息.   
   */
  explicit Layer(const LayerParameter& param)
    : layer_param_(param) {
      // Set phase and copy blobs (if there are any).
      phase_ = param.phase();
      if (layer_param_.blobs_size() > 0) {
        blobs_.resize(layer_param_.blobs_size());
        for (int i = 0; i < layer_param_.blobs_size(); ++i) {
          blobs_[i].reset(new Blob<Dtype>());
          blobs_[i]->FromProto(layer_param_.blobs(i));
        }
      }
    }
  virtual ~Layer() {}

  /**
   * @brief SetUp() 实现了 Layer 的公共初始化函数.
   *
   * @param bottom 未改变维度的 input blobs
   * @param top 空间分配好了但是没有将维度变为 output blobs, 因此函数中会调用 Reshape()
   *
   * 1. CheckBlobCounts() 检查 bottom and top blobs 的数量是否正确.
   * 2. LayerSetUp() 完成"特定层"的初始化操作.
   * 3. Reshape() 用于设置 top blobs 的 size 及其内部的 buffers.
   * 4. SetLossWeights() 设置 loss weight 乘法器的 blobs 为任意非零权重值.
   */
  void SetUp(const vector<Blob<Dtype>*>& bottom,
      const vector<Blob<Dtype>*>& top) {
    CheckBlobCounts(bottom, top);
    LayerSetUp(bottom, top);
    Reshape(bottom, top);
    SetLossWeights(top);
  }

  /**
   * @brief LayerSetUp() 完成特定层的初始化操作, 用户自定义的层需要实现这个函数
   *        和 Reshape().  
   *
   * @param bottom 未改变维度的 input blobs, 其 data_ 域保存的是该层的输入数据
   * @param top 空间分配好了但是没有将维度变为 output blobs
   *
   * 该函数只调用一次就可实现 Layer 的初始化. 函数会读取并处理相关的 layer_param_ 参数, 
   * 之后 Reshape() 会设置 top blobs 的 shapes 和 internal buffers, 在前向运算之前
   * 调整 top blob 的 sizes, 即有多少个 top blob.  
   */
  virtual void LayerSetUp(const vector<Blob<Dtype>*>& bottom,
      const vector<Blob<Dtype>*>& top) {}

  /**
   * @brief Reshape() 调整 top blobs 的 shapes 和 internal buffers, 以适应 
   *        bottom blobs 的 shapes. 
   *
   * @param bottom 是 input blobs, 输入的 shapes 是确定的.   
   * @param top 是 top blobs, 如果需要的话会 reshape (其空间已分配好).  
   *
   * 如果需要的话, Reshape() 应该根据 bottom (input) blobs 的 shapes 对 
   * top blobs shapes 进行 reshape. 同时也会对 internal buffers 进行 reshape 
   * 以适应输出(bottom) blobs.  
   */
  virtual void Reshape(const vector<Blob<Dtype>*>& bottom,
      const vector<Blob<Dtype>*>& top) = 0;

  /**
   * @brief Forward() 在给定 bottom blobs 的情况下计算输出(top blobs) 和 loss.
   *
   * @param bottom 未改变维度的 input blobs, 其 data_ 域保存的是该层的输入数据
   * @param top 空间分配好了但是没有将维度变为 output blobs
   * \return 返回这层的 total loss.
   *
   * 给定输入 blobs(bottom blobs), Forward() 函数会调用对应的设备相关函数(Forward_cpu 或 
   * Forward_gpu)来计算 top blob 的值. 如果层中有非零 loss 权重, 那么就可以计算 loss
   *
   * 用户定义的层应该实现 Forward_cpu(), Forward_gpu() 可以选择性的实现.  
   */
  inline Dtype Forward(const vector<Blob<Dtype>*>& bottom,
      const vector<Blob<Dtype>*>& top);

  /**
   * @brief 给定输出 blob(top blob) 的误差梯度值, 计算输入 blob(bottom blob) 的
   *        误差梯度值.
   *
   * @param top
   *     输出 blobs, 其 diff_ 保存的是误差梯度 
   * @param propagate_down
   *     a vector with equal length to bottom, with each index indicating
   *     whether to propagate the error gradients down to the bottom blob at
   *     the corresponding index
   * @param bottom
   *     输入 blobs, 其 diff_ 保存的是 Backward() 运行之后的误差梯度 
   *
   * 给定输出 blobs(top blobs) 的 diff , Backward() 函数会调用对应的设备相关函数
   * (Backward_cpu 或 Backward_gpu)来计算 bottom blob 的 diffs .  
   *
   * 用户定义的层应该实现 Backward_cpu(), Backward_gpu() 可以选择性的实现.  
   */
  inline void Backward(const vector<Blob<Dtype>*>& top,
      const vector<bool>& propagate_down,
      const vector<Blob<Dtype>*>& bottom);

  vector<shared_ptr<Blob<Dtype> > >& blobs() {
    return blobs_;   // 返回可学习参数 blobs 的 vector
  }

  const LayerParameter& layer_param() const { return layer_param_; }

  /**
   * @brief 将 layer parameter 写入到一个 protocol buffer. 
   */
  virtual void ToProto(LayerParameter* param, bool write_diff = false);

  /**
   * @brief 返回指定 index 的 top blob 对应的 loss 标量.
   */
  inline Dtype loss(const int top_index) const {
    return (loss_.size() > top_index) ? loss_[top_index] : Dtype(0);
  }

  /**
   * @brief 设置指定 index 的 top blob 对应的 loss 值.
   */
  inline void set_loss(const int top_index, const Dtype value) {
    if (loss_.size() <= top_index) {
      loss_.resize(top_index + 1, Dtype(0));
    }
    loss_[top_index] = value;
  }

  virtual inline const char* type() const { return ""; }  // 返回 layer 类型名

  /**
   * @brief 返回 layer 需要的 bottom blobs 的准确个数, 如果是 -1, 表示不需要指定确切个数
   *
   * 如果用户自定义的层的 bottom blobs 有确切的个数, ExactNumBottomBlobs() 函数应该被
   * 重写并返回一个非零数.
   */
  virtual inline int ExactNumBottomBlobs() const { return -1; }
  /**
   * @brief 返回 layer 需要的 bottom blobs 的最小个数, 如果是 -1, 表示不需要指定最小个数
   *
   * 如果用户自定义的层的 bottom blobs 需要指定其最小数量, MinBottomBlobs() 函数应该被
   * 重写并返回一个非零数.
   */
  virtual inline int MinBottomBlobs() const { return -1; }
  /**
   * @brief 返回 layer 需要的 bottom blobs 的最大个数, 如果是 -1, 表示不需要指定最大个数
   *
   * 如果用户自定义的层的 bottom blobs 需要指定其最大数量, MaxBottomBlobs() 函数应该被
   * 重写并返回一个非零数.
   */
  virtual inline int MaxBottomBlobs() const { return -1; }
  /**
   * @brief 返回 layer 需要的 top blobs 的最大个数, 如果是 -1, 表示不需要指定最大个数
   *
   * 如果用户自定义的层的 top blobs 有确切的个数, ExactNumTopBlobs() 函数应该被
   * 重写并返回一个非零数
   */
  virtual inline int ExactNumTopBlobs() const { return -1; }
  /**
   * @brief 返回 layer 需要的 top blobs 的最小个数, 如果是 -1, 表示不需要指定最小个数
   *
   * 如果用户自定义的层的 top blobs 需要指定其最小数量, MinTopBlobs() 函数应该被
   * 重写并返回一个非零数.
   */
  virtual inline int MinTopBlobs() const { return -1; }
  /**
   * @brief 返回 layer 需要的 top blobs 的最大个数, 如果是 -1, 表示不需要指定最大个数
   *
   * 如果用户自定义的层的 top blobs 需要指定其最大数量, MaxTopBlobs() 函数应该被
   * 重写并返回一个非零数.
   */
  virtual inline int MaxTopBlobs() const { return -1; }
  /**
   * @brief 如果层的 bottom 和 top blobs 的数量应该是相同的, 则返回 true.
   *
   * 如果用户自定义的层的 bottom 和 top blobs 的数量应该一致, 则 EqualNumBottomTopBlobs() 
   * 函数应该被重写并返回一个非零数.
   */
  virtual inline bool EqualNumBottomTopBlobs() const { return false; }

  /**
   * @brief Return whether "anonymous" top blobs are created automatically
   *        by the layer.
   *
   * 如果 AutoTopBlobs() 函数返回 true, Net::Init() 将会创建足够的 "anonymous" top
   * blobs 来填充 ExactNumTopBlobs() 或 MinTopBlobs() 函数指定的数量.  
   */
  virtual inline bool AutoTopBlobs() const { return false; }

  /**
   * @brief 返回指定索引对应的 bottom blob 是否需要强制 force_backward.    
   *
   * 如果 AllowForceBackward(i) == false, 就会忽略 force_backward 的设置
   * ,并且只有它需要 gradient 信息的时候才会反向传播到 blob i. (作用类似于 
   * force_backward == false).
   */
  virtual inline bool AllowForceBackward(const int bottom_index) const {
    return true;
  }

  /**
   * @brief 给定 param_id, 确定 layer 是否需要计算这个参数的 gradients .
   *
   * 当然, 如果你不在乎多余的计算的话, 可以直接忽略那些 false 值(即总是计算所有参数的梯度) 
   */
  inline bool param_propagate_down(const int param_id) {
    return (param_propagate_down_.size() > param_id) ?
        param_propagate_down_[param_id] : false;
  }

  /**
   * @brief 对应于上面的 param_propagate_down() 函数. 
   *        设置 layer 是否需要计算这个参数的 gradients.
   */
  inline void set_param_propagate_down(const int param_id, const bool value) {
    if (param_propagate_down_.size() <= param_id) {
      param_propagate_down_.resize(param_id + 1, true);
    }
    param_propagate_down_[param_id] = value;
  }


 protected:
 ...
```
3. 主要函数功能总结    
1) LayerSetUp() 会尝试从 protobuf 文件读取参数。   
2) Forward() 和 Backward() 对应前向计算和反向更新，输入都是 bottom，输出为 top，其中 Backward() 里面有个 propagate_down 参数，用来表示该 Layer 是否反向传播参数。  
3) 在 Forward() 和 Backward() 的具体实现里, 会根据 Caffe::mode()进行对应的操作，即使用 cpu 或者 gpu 进行计算, 两个都实现了对应的接口 Forward_cpu()、Forward_gpu() 和 Backward_cpu()、Backward_gpu()，这些接口都是 virtual， 具体还是要根据 layer 的类型进行对应的计算(注意: 有些 layer 并没有 GPU 计算的实现, 所以封装时加入了 CPU 的计算作为后备).    
4) 实现了 ToProto() 的接口，可以将 Layer 的参数写入到 protocol buffer 文件。   

## layer.cpp 的实现   
一共就一行代码.    
```cpp
#include "caffe/layer.hpp"

namespace caffe {

INSTANTIATE_CLASS(Layer);

}  // namespace caffe

```

## 协调使用 CPU 和 GPU 设备.   
1. device_alternate.hpp 中, 通过 #ifdef CPU_ONLY 定义了一些宏来取消 GPU 的调用：   
```cpp
#define STUB_GPU(classname)
#define STUB_GPU_FORWARD(classname, funcname)
#define STUB_GPU_BACKWARD(classname, funcname)
```

## Layer 相关的 Proto   
1. LayerParameter   
用于保存 layer 的参数.   
```proto
// NOTE
// 当你添加完毕一个自定义的 LayerParameter 之后, 记得更新下一个可用的 ID. 
//
// LayerParameter next available layer-specific ID: 148 (last added:
// triplet_loss_param)
message LayerParameter {
  optional string name = 1;   // layer 的 name
  optional string type = 2;   // layer 的 type
  repeated string bottom = 3; // bottom blob(s) 的 name
  repeated string top = 4;    // top blob(s) 的 name

  optional Phase phase = 10;  // 网络使用阶段: TRAIN 或 TEST

  // 分配给每个 top blob 的权重值 amount 
  // 每个 layer 中都会给 top blob 分配一个默认值，默认是 0 或 1
  repeated float loss_weight = 5;   

  // 指定训练参数(全局学习常数的乘数因子, 用于参数共享的 name 域和其他域的设置)
  repeated ParamSpec param = 6; 

  repeated BlobProto blobs = 7;  // blobs 中包含层的数值参数

  // 指示每个 bottom 数据是否需要反向运算。 如果未指定(并不是值为 false), Caffe 会自动
  // 推测该输入是否需要进行反向运算(计算参数的梯度). 如果设置为 true , 则强制反向计算. 
  // 如果设置为 false, 则不进行反向运算。
  //
  // propagate_down_.size() 要么为 0, 要么等于 bottoms_.size()   
  repeated bool propagate_down = 11;

  // 根据当前的 NetState 判断是否(或何时)将一个 layer 包含到一个 net 中。如果 include 
  // 为非零值, 则表示将该 layer 包含到一个 net 中, 如果 exclude 为非零值, 则表示不包含. 
  // 但是不能同时指定 include 和 exclude. 
  //
  // 如果没指定任何规则，那么这个 layer 默认是被包含的.
  repeated NetStateRule include = 8;
  repeated NetStateRule exclude = 9;

  optional TransformationParameter transform_param = 100; // 数据预处理.
  optional LossParameter loss_param = 101;  // 在 loss 层间共享的参数

  // 用户自定义层的参数.
  //
  // Note: 层对应的计算可能有多个的计算引擎(GPUS), 那么通过 Engine type 和 engine
  // parameter 就可以选择计算方式。 默认的 engine 是在编译阶段通过 ENGINE 开关来设置的. 
  //
  optional AccuracyParameter accuracy_param = 102;
  optional ArgMaxParameter argmax_param = 103;
  optional BatchNormParameter batch_norm_param = 139;
  optional BiasParameter bias_param = 141;
  optional ConcatParameter concat_param = 104;
  optional ContrastiveLossParameter contrastive_loss_param = 105;
  optional ConvolutionParameter convolution_param = 106;
  optional CropParameter crop_param = 144;
  optional DataParameter data_param = 107;
  optional DropoutParameter dropout_param = 108;
  optional DummyDataParameter dummy_data_param = 109;
  optional EltwiseParameter eltwise_param = 110;
  optional ELUParameter elu_param = 140;
  optional EmbedParameter embed_param = 137;
  optional ExpParameter exp_param = 111;
  optional FlattenParameter flatten_param = 135;
  optional HDF5DataParameter hdf5_data_param = 112;
  optional HDF5OutputParameter hdf5_output_param = 113;
  optional HingeLossParameter hinge_loss_param = 114;
  optional ImageDataParameter image_data_param = 115;
  optional InfogainLossParameter infogain_loss_param = 116;
  optional InnerProductParameter inner_product_param = 117;
  optional InputParameter input_param = 143;
  optional LogParameter log_param = 134;
  optional LRNParameter lrn_param = 118;
  optional MemoryDataParameter memory_data_param = 119;
  optional MVNParameter mvn_param = 120;
  optional ParameterParameter parameter_param = 145;
  optional PoolingParameter pooling_param = 121;
  optional PowerParameter power_param = 122;
  optional PReLUParameter prelu_param = 131;
  optional PythonParameter python_param = 130;
  optional RecurrentParameter recurrent_param = 146;
  optional ReductionParameter reduction_param = 136;
  optional ReLUParameter relu_param = 123;
  optional ReshapeParameter reshape_param = 133;
  optional ScaleParameter scale_param = 142;
  optional SigmoidParameter sigmoid_param = 124;
  optional SoftmaxParameter softmax_param = 125;
  optional SPPParameter spp_param = 132;
  optional SliceParameter slice_param = 126;
  optional TanHParameter tanh_param = 127;
  optional ThresholdParameter threshold_param = 128;
  optional TileParameter tile_param = 138;
  optional TripletLossParameter triplet_loss_param = 147;
  optional WindowDataParameter window_data_param = 129;
}
```
2. ParamSpec 定义   
用于指定 training parameters.   
```proto
// ParamSpec 包含的参数有：
//  1. multipliers on global learning constants
//  2. multipliers on global weight decay
//  3. parameter blobs 对应的 name 
//  4. other settings used for weight sharing
message ParamSpec {
  // name 可用于在层间共享参数. 为了在两个层之间共享一个参数, 需要给 name 一个非空值.
  optional string name = 1;  

  // 共享的权重值是否需要有相同的 shape, 或者只是需要 count 匹配即可. 
  optional DimCheckMode share_mode = 2;
  enum DimCheckMode {
    // STRICT (default): num, channels, height, width 四个值需要全部指定.
    STRICT = 0;
    // PERMISSIVE: 只需要指定 count(num*channels*height*width) 的值, 然后去匹配.
    PERMISSIVE = 1;
  }

  optional float lr_mult = 3 [default = 1.0];  // 该参数的全局学习率的乘数因子.
  optional float decay_mult = 4 [default = 1.0]; // 该参数的全局权重衰减的乘数因子.
}

```
3. NetStateRule 定义   
用于指定 layer 是否被 include / exclude 的规则.  
```proto
message NetStateRule {
  optional Phase phase = 1;  // 设置 NetState 起作用的阶段 (TRAIN or TEST)

  // 设置 layer 会使用到的 level 的最小值和(或)最大值.  如果和 level 无关的话, 不用定义.
  optional int32 min_level = 2;
  optional int32 max_level = 3;

  // 可定制的 stages (用于 include 或 exclude) 的集合.
  // 网络应该包含所有指定的 stages (not_stage 可以赋值为 NONE).
  // 可以使用多个 NetStateRule 来表示不同 stages 的连接.  
  repeated string stage = 4;
  repeated string not_stage = 5;
}
```
4. TransformationParameter  
用于指定训练时使用什么样的数据增广方式.   
```proto
message TransformationParameter {
  // 在数据预处理阶段, 我们会对图像进行简单的缩放和减去均值(如果之前计算过均值)操作.
  // Note: 减均值操作是在图像缩放之前进行的.   
  optional float scale = 1 [default = 1];
  optional bool mirror = 2 [default = false];   // 随机镜像
  optional uint32 crop_size = 3 [default = 0];  // 随机裁剪
  // mean_file and mean_value cannot be specified at the same time
  optional string mean_file = 4;   // 指定均值文件, 不能同时指定均值文件和均值数组
  repeated float mean_value = 5;   // 指定均值文件和均值数组
  optional bool force_color = 6 [default = false];  // 强制解码为彩色图像
  optional bool force_gray = 7 [default = false];   // 强制解码为灰度图
}
``` 
5. LossParameter   
保存 loss 层间会共享的参数.   
```proto
message LossParameter {
  // If specified, ignore instances with the given label.
  optional int32 ignore_label = 1;
  // How to normalize the loss for loss layers that aggregate across batches,
  // spatial dimensions, or other dimensions.  Currently only implemented in
  // SoftmaxWithLoss and SigmoidCrossEntropyLoss layers.
  enum NormalizationMode {
    // Divide by the number of examples in the batch times spatial dimensions.
    // Outputs that receive the ignore label will NOT be ignored in computing
    // the normalization factor.
    FULL = 0;
    // Divide by the total number of output locations that do not take the
    // ignore_label.  If ignore_label is not set, this behaves like FULL.
    VALID = 1;
    // Divide by the batch size.
    BATCH_SIZE = 2;
    // Do not normalize the loss.
    NONE = 3;
  }

  // 由于历史原因, SigmoidCrossEntropyLoss 的默认正则化方式是 BATCH_SIZE, 而不是 VALID. 
  optional NormalizationMode normalization = 3 [default = VALID];
  
  // 旧版本. 如果上一个参数 normalization 被指定了的话就直接忽略这个参数. 如果 
  // normalization 没有指定, 那么 normalize = false 相当于是上一个参数 normalization 
  // = BATCH_SIZE
  optional bool normalize = 2;
}
```

