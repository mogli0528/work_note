# Layer 的实现   
Layer 是 Caffe 中最庞大最繁杂的模块。由于 Caffe 强调模块化设计, 因此只允许每个 Layer完成一类特定的计算. 例如 convolution 操作、 pooling 、 非线性变换 、 内积运算 、数据加载、归一化和损失计算等.     
Layer 这个类可以说是里面最终的一个基本类了, 深度网络呢就是一层一层的 Layer, 相互之间通过传输 Blob 数据连接起来.   

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
  /** The protobuf that stores the layer parameters */
  LayerParameter layer_param_;
  /** The phase: TRAIN or TEST */
  Phase phase_;
  /** The vector that stores the learnable parameters as a set of blobs. */
  vector<shared_ptr<Blob<Dtype> > > blobs_;
  /** Vector indicating whether to compute the diff of each param blob. */
  vector<bool> param_propagate_down_;

  /** The vector that indicates whether each top blob has a non-zero weight in
   *  the objective function. */
  vector<Dtype> loss_;

  /** @brief Using the CPU device, compute the layer output. */
  virtual void Forward_cpu(const vector<Blob<Dtype>*>& bottom,
      const vector<Blob<Dtype>*>& top) = 0;
  /**
   * @brief Using the GPU device, compute the layer output.
   *        Fall back to Forward_cpu() if unavailable.
   */
  virtual void Forward_gpu(const vector<Blob<Dtype>*>& bottom,
      const vector<Blob<Dtype>*>& top) {
    // LOG(WARNING) << "Using CPU code as backup.";
    return Forward_cpu(bottom, top);
  }

  /**
   * @brief Using the CPU device, compute the gradients for any parameters and
   *        for the bottom blobs if propagate_down is true.
   */
  virtual void Backward_cpu(const vector<Blob<Dtype>*>& top,
      const vector<bool>& propagate_down,
      const vector<Blob<Dtype>*>& bottom) = 0;
  /**
   * @brief Using the GPU device, compute the gradients for any parameters and
   *        for the bottom blobs if propagate_down is true.
   *        Fall back to Backward_cpu() if unavailable.
   */
  virtual void Backward_gpu(const vector<Blob<Dtype>*>& top,
      const vector<bool>& propagate_down,
      const vector<Blob<Dtype>*>& bottom) {
    // LOG(WARNING) << "Using CPU code as backup.";
    Backward_cpu(top, propagate_down, bottom);
  }

  /**
   * Called by the parent Layer's SetUp to check that the number of bottom
   * and top Blobs provided as input match the expected numbers specified by
   * the {ExactNum,Min,Max}{Bottom,Top}Blobs() functions.
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

  /**
   * Called by SetUp to initialize the weights associated with any top blobs in
   * the loss function. Store non-zero loss weights in the diff blob.
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

 private:
  /** Whether this layer is actually shared by other nets*/
  bool is_shared_;

  /** The mutex for sequential forward if this layer is shared */
  shared_ptr<boost::mutex> forward_mutex_;

  /** Initialize forward_mutex_ */
  void InitMutex();
  /** Lock forward_mutex_ if this layer is shared */
  void Lock();
  /** Unlock forward_mutex_ if this layer is shared */
  void Unlock();

  DISABLE_COPY_AND_ASSIGN(Layer);
```

Layer类的构造函数
```cpp
explicit Layer(const LayerParameter& param) : layer_param_(param)
```
会尝试从`protobuf`文件读取参数。其三个主要接口：
```cpp
virtual void SetUp(const vector<Blob<Dtype>*>& bottom, vector<Blob<Dtype>*>* top)
inline Dtype Forward(const vector<Blob<Dtype>*>& bottom, vector<Blob<Dtype>*>* top);
inline void Backward(const vector<Blob<Dtype>*>& top, const vector<bool>& propagate_down, const <Blob<Dtype>*>* bottom);
```

SetUp()函数   
```
void SetUp(const vector<Blob<Dtype>*>& bottom,
  const vector<Blob<Dtype>*>& top) 
{
  1. 检查输入输出 blob 个数是否满足要求，每个层能处理的输入输出数据不一样 
    2. 调用 LayerSetUp 函数初始化特定的层，每个 Layer 子类需重写这个函数完成定制的初始化 
    3. 调用 Reshape 函数为 top blob 分配合适大小的存储空间 
    4. 为每个 top blob 的非零 loss 权重设置 loss 权重乘子
}
```
SetUp函数需要根据实际的参数设置进行实现，对各种类型的参数初始化；

Forward和Backward对应前向计算和反向更新，输入统一都是bottom，输出为top，其中Backward里面有个propagate_down参数，用来表示该Layer是否反向传播参数。
在Forward和Backward的具体实现里，会根据Caffe::mode()进行对应的操作，即使用cpu或者gpu进行计算，两个都实现了对应的接口Forward_cpu、Forward_gpu和Backward_cpu、Backward_gpu，这些接口都是virtual，具体还是要根据layer的类型进行对应的计算（注意：有些layer并没有GPU计算的实现，所以封装时加入了CPU的计算作为后备）。另外，还实现了ToProto的接口，将Layer的参数写入到protocol buffer文件中。

2. public 成员  


## 额外的 Tricks   
1. `device_alternate.hpp`中，通过`#ifdef CPU_ONLY`定义了一些宏来`取消GPU的调用`：   
```cpp
#define STUB_GPU(classname)
#define STUB_GPU_FORWARD(classname, funcname)
#define STUB_GPU_BACKWARD(classname, funcname)
```


## 获取卷积层
 GetConvolutionLayer()
 根据`engine`是否为`GPU`來获取对应的卷积层(ConvolutionLayer 或 CuDNNConvolutionLayer)。

```
shared_ptr<Layer<Dtype> > GetConvolutionLayer(
    const LayerParameter& param) {
  ...
  if (engine == ConvolutionParameter_Engine_CAFFE) {
    return shared_ptr<Layer<Dtype> >(new ConvolutionLayer<Dtype>(param));
#ifdef USE_CUDNN
  } else if (engine == ConvolutionParameter_Engine_CUDNN) {
      ...
      return shared_ptr<Layer<Dtype> >(new CuDNNConvolutionLayer<Dtype>(param));
  } else {
    ...
    throw;  // Avoids missing return warning
  }
}
```

## 获取Pooling, LRN, ReLU, Sigmoid, TanH, Python层
 GetPoolingLayer(), GetLRNLayer(), GetReLULayer(), GetSigmoidLayer(), GetTanHLayer(), GetPythonLayer()
 同样是根据`engine`是否为`GPU`來获取对应的层。
```
class Layer
{
    explicit Layer(const LayerParameter& param)
      : layer_param_(param) {...}
  
    /* 1.检查层的输入数据和输出数据的个数是否正确；
     * 2.在LayerSetUp()函数中为一些特别的层做特殊处理。因此该函数是应该被重载的。
     * 3.对输出数据reshape
     * 4.为非零权重值设置`loss`权重`multiplier`
     */
    void SetUp(const vector<Blob<Dtype>*>& bottom,
      const vector<Blob<Dtype>*>& top) {...}
   
    /*
     * 实现特定层的一些初始化工作;
     * 用户自定义的层应该实现这个函数和Reshape()函数
     */
    virtual void LayerSetUp(const vector<Blob<Dtype>*>& bottom,
      const vector<Blob<Dtype>*>& top) {}

}
```

 用户不应该实现自己的构造函数。任何设置代码应该放在SetUp()函数中, 这个函数中提供了输入数据的维度。



## Layer 相关的 Proto   
1. LayerParameter      
```proto
message LayerParameter {
  optional string name = 1;   // layer 的 name
  optional string type = 2;   // layer 的 type
  repeated string bottom = 3; // 每个 bottom blob 的 name
  repeated string top = 4;    // 每个 top blob 的 name

  optional Phase phase = 10;  // TRAIN 或 TEST

  // 分配给每个 top blob 的权重值的数量 
  // 每个 layer 中都会分配一个默认值，通常给每个 top blob 分配的是 0 或 1
  repeated float loss_weight = 5;   

  // 指定训练参数(全局学习常数的乘子，name和其他权重值分享的参数)
  repeated ParamSpec param = 6; 

  repeated BlobProto blobs = 7;  // 该层所有的 blobs ,其中包含数值参数

  // 指示每个 bottom 数据是否需要进行反向运算。 如果未指定， Caffe 会自动推测是够需要
  // 进行反向运算来计算参数的梯度。 如果设置为 true ，则强制反向计算。 如果设置为 false ，
  // 则不进行反向运算。
  //
  // propagate_down_.size() 要么为 0 ， 要么等于 bottoms_ 的大小   
  repeated bool propagate_down = 11;

  // 根据当前的 NetState 控制 [是否 OR 何时] 将一个 layer 包含到一个 net 中。你可以指
  // 定一个非零值表示的规则来 [include OR exclude]。如果没指定任何规则，那么这个 layer 
  // 默认是被包含的。如果当前的 NetState 满足任何一个特定的规则，那么该 layer 就会被包含。  
  repeated NetStateRule include = 8;
  repeated NetStateRule exclude = 9;

  optional TransformationParameter transform_param = 100; // 数据预处理参数.

  optional LossParameter loss_param = 101;  // loss layers 共享的参数

  // Layer type-specific parameters.
  //
  // Note: 某些层有不止一个的计算引擎(GPUS)。这些层就会包含 Engine type 和
  // engine parameter 来选择计算方式。 默认的 engine 实在编译阶段通过 ENGINE
  // 开关来设置的。 
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
3.2 ParamSpec 定义   
```proto
// 指定 training parameters 
// 这些参数有：
//    1. multipliers on global learning constants
//    2. multipliers on global weight decay
//    3. name 
//    4. other settings used for weight sharing
message ParamSpec {
  // The names of the parameter blobs -- useful for sharing parameters among
  // layers, but never required otherwise.  To share a parameter between two
  // layers, give it a (non-empty) name.
  optional string name = 1;

  // Whether to require shared weights to have the same shape, or just the same
  // count -- defaults to STRICT if unspecified.
  optional DimCheckMode share_mode = 2;
  enum DimCheckMode {
    // STRICT (default) requires that num, channels, height, width each match.
    STRICT = 0;
    // PERMISSIVE requires only the count (num*channels*height*width) to match.
    PERMISSIVE = 1;
  }

  // The multiplier on the global learning rate for this parameter.
  optional float lr_mult = 3 [default = 1.0];

  // The multiplier on the global weight decay for this parameter.
  optional float decay_mult = 4 [default = 1.0];
}

```
3.3 NetStateRule 定义   
```proto
message NetStateRule {
  // Set phase to require the NetState have a particular phase (TRAIN or TEST)
  // to meet this rule.
  optional Phase phase = 1;

  // Set the minimum and/or maximum levels in which the layer should be used.
  // Leave undefined to meet the rule regardless of level.
  optional int32 min_level = 2;
  optional int32 max_level = 3;

  // Customizable sets of stages to include or exclude.
  // The net must have ALL of the specified stages and NONE of the specified
  // "not_stage"s to meet the rule.
  // (Use multiple NetStateRules to specify conjunctions of stages.)
  repeated string stage = 4;
  repeated string not_stage = 5;
}
```
3.4 TransformationParameter  
```proto
// Message that stores parameters used to apply transformation
// to the data layer's data
message TransformationParameter {
  // For data pre-processing, we can do simple scaling and subtracting the
  // data mean, if provided. Note that the mean subtraction is always carried
  // out before scaling.
  optional float scale = 1 [default = 1];
  // Specify if we want to randomly mirror data.
  optional bool mirror = 2 [default = false];
  // Specify if we would like to randomly crop an image.
  optional uint32 crop_size = 3 [default = 0];
  // mean_file and mean_value cannot be specified at the same time
  optional string mean_file = 4;
  // if specified can be repeated once (would subtract it from all the channels)
  // or can be repeated the same number of times as channels
  // (would subtract them from the corresponding channel)
  repeated float mean_value = 5;
  // Force the decoded image to have 3 color channels.
  optional bool force_color = 6 [default = false];
  // Force the decoded image to have 1 color channels.
  optional bool force_gray = 7 [default = false];
}

``` 
3.5 LossParameter   
```proto
// Message that stores parameters shared by loss layers
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
  // For historical reasons, the default normalization for
  // SigmoidCrossEntropyLoss is BATCH_SIZE and *not* VALID.
  optional NormalizationMode normalization = 3 [default = VALID];
  // Deprecated.  Ignored if normalization is specified.  If normalization
  // is not specified, then setting this to false will be equivalent to
  // normalization = BATCH_SIZE to be consistent with previous behavior.
  optional bool normalize = 2;
}
```

