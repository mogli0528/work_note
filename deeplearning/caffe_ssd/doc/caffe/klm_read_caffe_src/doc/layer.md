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
   * @brief 返回 layer 需要的 bottom blobs 的准确个数,  required by the layer,
   *        or -1 if no exact number is required.
   *
   * This method should be overridden to return a non-negative value if your
   * layer expects some exact number of bottom blobs.
   */
  virtual inline int ExactNumBottomBlobs() const { return -1; }
  /**
   * @brief Returns the minimum number of bottom blobs required by the layer,
   *        or -1 if no minimum number is required.
   *
   * This method should be overridden to return a non-negative value if your
   * layer expects some minimum number of bottom blobs.
   */
  virtual inline int MinBottomBlobs() const { return -1; }
  /**
   * @brief Returns the maximum number of bottom blobs required by the layer,
   *        or -1 if no maximum number is required.
   *
   * This method should be overridden to return a non-negative value if your
   * layer expects some maximum number of bottom blobs.
   */
  virtual inline int MaxBottomBlobs() const { return -1; }
  /**
   * @brief Returns the exact number of top blobs required by the layer,
   *        or -1 if no exact number is required.
   *
   * This method should be overridden to return a non-negative value if your
   * layer expects some exact number of top blobs.
   */
  virtual inline int ExactNumTopBlobs() const { return -1; }
  /**
   * @brief Returns the minimum number of top blobs required by the layer,
   *        or -1 if no minimum number is required.
   *
   * This method should be overridden to return a non-negative value if your
   * layer expects some minimum number of top blobs.
   */
  virtual inline int MinTopBlobs() const { return -1; }
  /**
   * @brief Returns the maximum number of top blobs required by the layer,
   *        or -1 if no maximum number is required.
   *
   * This method should be overridden to return a non-negative value if your
   * layer expects some maximum number of top blobs.
   */
  virtual inline int MaxTopBlobs() const { return -1; }
  /**
   * @brief Returns true if the layer requires an equal number of bottom and
   *        top blobs.
   *
   * This method should be overridden to return true if your layer expects an
   * equal number of bottom and top blobs.
   */
  virtual inline bool EqualNumBottomTopBlobs() const { return false; }

  /**
   * @brief Return whether "anonymous" top blobs are created automatically
   *        by the layer.
   *
   * If this method returns true, Net::Init will create enough "anonymous" top
   * blobs to fulfill the requirement specified by ExactNumTopBlobs() or
   * MinTopBlobs().
   */
  virtual inline bool AutoTopBlobs() const { return false; }

  /**
   * @brief Return whether to allow force_backward for a given bottom blob
   *        index.
   *
   * If AllowForceBackward(i) == false, we will ignore the force_backward
   * setting and backpropagate to blob i only if it needs gradient information
   * (as is done when force_backward == false).
   */
  virtual inline bool AllowForceBackward(const int bottom_index) const {
    return true;
  }

  /**
   * @brief Specifies whether the layer should compute gradients w.r.t. a
   *        parameter at a particular index given by param_id.
   *
   * You can safely ignore false values and always compute gradients
   * for all parameters, but possibly with wasteful computation.
   */
  inline bool param_propagate_down(const int param_id) {
    return (param_propagate_down_.size() > param_id) ?
        param_propagate_down_[param_id] : false;
  }
  /**
   * @brief Sets whether the layer should compute gradients w.r.t. a
   *        parameter at a particular index given by param_id.
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
1. 以卷积层的 Reshape() 为例进行分析.    
TODO: 应该再开一个单独的文件分析.    
```cpp
template <typename Dtype>
void BaseConvolutionLayer<Dtype>::Reshape(const vector<Blob<Dtype>*>& bottom,
      const vector<Blob<Dtype>*>& top) {
  const int first_spatial_axis = channel_axis_ + 1;
  CHECK_EQ(bottom[0]->num_axes(), first_spatial_axis + num_spatial_axes_)
      << "bottom num_axes may not change.";
  num_ = bottom[0]->count(0, channel_axis_);
  CHECK_EQ(bottom[0]->shape(channel_axis_), channels_)
      << "Input size incompatible with convolution kernel.";
  // TODO: generalize to handle inputs of different shapes.
  for (int bottom_id = 1; bottom_id < bottom.size(); ++bottom_id) {
    CHECK(bottom[0]->shape() == bottom[bottom_id]->shape())
        << "All inputs must have the same shape.";
  }
  // Shape the tops.
  bottom_shape_ = &bottom[0]->shape();
  compute_output_shape();
  vector<int> top_shape(bottom[0]->shape().begin(),
      bottom[0]->shape().begin() + channel_axis_);
  top_shape.push_back(num_output_);
  for (int i = 0; i < num_spatial_axes_; ++i) {
    top_shape.push_back(output_shape_[i]);
  }
  for (int top_id = 0; top_id < top.size(); ++top_id) {
    top[top_id]->Reshape(top_shape);
  }
  if (reverse_dimensions()) {
    conv_out_spatial_dim_ = bottom[0]->count(first_spatial_axis);
  } else {
    conv_out_spatial_dim_ = top[0]->count(first_spatial_axis);
  }
  col_offset_ = kernel_dim_ * conv_out_spatial_dim_;
  output_offset_ = conv_out_channels_ * conv_out_spatial_dim_ / group_;
  // Setup input dimensions (conv_input_shape_).
  vector<int> bottom_dim_blob_shape(1, num_spatial_axes_ + 1);
  conv_input_shape_.Reshape(bottom_dim_blob_shape);
  int* conv_input_shape_data = conv_input_shape_.mutable_cpu_data();
  for (int i = 0; i < num_spatial_axes_ + 1; ++i) {
    if (reverse_dimensions()) {
      conv_input_shape_data[i] = top[0]->shape(channel_axis_ + i);
    } else {
      conv_input_shape_data[i] = bottom[0]->shape(channel_axis_ + i);
    }
  }
  // The im2col result buffer will only hold one image at a time to avoid
  // overly large memory usage. In the special case of 1x1 convolution
  // it goes lazily unused to save memory.
  col_buffer_shape_.clear();
  col_buffer_shape_.push_back(kernel_dim_ * group_);
  for (int i = 0; i < num_spatial_axes_; ++i) {
    if (reverse_dimensions()) {
      col_buffer_shape_.push_back(input_shape(i + 1));
    } else {
      col_buffer_shape_.push_back(output_shape_[i]);
    }
  }
  col_buffer_.Reshape(col_buffer_shape_);
  bottom_dim_ = bottom[0]->count(channel_axis_);
  top_dim_ = top[0]->count(channel_axis_);
  num_kernels_im2col_ = conv_in_channels_ * conv_out_spatial_dim_;
  num_kernels_col2im_ = reverse_dimensions() ? top_dim_ : bottom_dim_;
  // Set up the all ones "bias multiplier" for adding biases by BLAS
  out_spatial_dim_ = top[0]->count(first_spatial_axis);
  if (bias_term_) {
    vector<int> bias_multiplier_shape(1, out_spatial_dim_);
    bias_multiplier_.Reshape(bias_multiplier_shape);
    caffe_set(bias_multiplier_.count(), Dtype(1),
        bias_multiplier_.mutable_cpu_data());
  }
}
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


Forward和Backward对应前向计算和反向更新，输入统一都是bottom，输出为top，其中Backward里面有个propagate_down参数，用来表示该Layer是否反向传播参数。
在Forward和Backward的具体实现里，会根据Caffe::mode()进行对应的操作，即使用cpu或者gpu进行计算，两个都实现了对应的接口Forward_cpu、Forward_gpu和Backward_cpu、Backward_gpu，这些接口都是virtual，具体还是要根据layer的类型进行对应的计算（注意：有些layer并没有GPU计算的实现，所以封装时加入了CPU的计算作为后备）。另外，还实现了ToProto的接口，将Layer的参数写入到protocol buffer文件中。


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

