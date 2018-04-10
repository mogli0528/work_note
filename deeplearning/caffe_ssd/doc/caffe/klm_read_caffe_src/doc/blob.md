# blob 的实现   
 
Caffe 中贯穿整个网络中的数据都用 blob 存储. 例如交换和处理网络中正向和反向迭代时的数据和导数信息。  
blob 是 Caffe 的标准数组结构，它提供了一个统一的内存接口.    
Layer 是 Caffe 模型和计算的基本单元， Net是一系列 layers 和其连接的集合.   
blob 详细描述了信息是如何在 layer 和 net 中存储和交互的.     

> Blob 实现的头文件是`caffe/include/caffe/blob.hpp`.    
## 目录   
- blob.hpp 的实现   
- blob.cpp 实现部分的重要代码   
- 和  Blob 相关的 Proto    

## blob.hpp 的实现     
1. private 成员  
```cpp
class Blob {
 public:
  ...

 protected:
  shared_ptr<SyncedMemory> data_;       // 是前向传递的数据, 指针类型是 shared_ptr 
  shared_ptr<SyncedMemory> diff_;       // 是反向传播中的梯度
  shared_ptr<SyncedMemory> shape_data_; // 保存 shape_ 数据, 这些数据可能会在 GPU 中使用
  vector<int> shape_;    // Blob 的形状
  int count_;     // 表示 Blob 的 size，也就是 n*c*h*w,  
  int capacity_;  // 表示当前的元素个数, 因为 Blob 可能会 Reshape  

  DISABLE_COPY_AND_ASSIGN(Blob);
};
```
2. public 成员  
```cpp
template <typename Dtype>
class Blob {
 public:
  Blob() : data_(), diff_(), count_(0), capacity_(0) {}  // 默认构造函数  

  /// @brief 旧版本; 推荐使用 Blob(const vector<int>& shape).
  /// explicit 关键字的作用是禁止构造函数的参数隐式转换  
  explicit Blob(const int num, const int channels, const int height,
      const int width);
  explicit Blob(const vector<int>& shape);

  /// @brief 旧版本; 推荐使用 Reshape(const vector<int>& shape).
  /// Reshape() 将 'num,channels,height,width' 传递给 'vector shape_', 然后调用
  /// 推荐版本的 Reshape(const vector<int>& shape) 函数.   
  void Reshape(const int num, const int channels, const int height,
      const int width);
  /**
   * @brief 改变 blob 的维度. 因为数据总维度可能也会变, 所以可能会产生内存分配
   *
   * 这个函数既可以在创建初始化内存的时候使用, 也可以在 Layer::Reshape() 
   * 或 Layer::Forward() 中调用以调整 top blob 的维度.  当改变 blob 的维度时, 
   * 如果没有内存, 则会重新分配内存. 然而如果有多余的内存, 那么多余的内存也不会被 free.   
   *
   * Note: 如果在对一个 input blob 调用 Reshape() 之后立即调用 Net::Backward()
   * 将会引发 error; 况且将新的 input shape 传递到更高的层时并不需要调用 Net::Forward()
   * 或 Net::Reshape() 函数.   
   * 
   * 初始化 shape_ 和 shape_data_, 同时为 data_ 和 diff_ 分配空间.
   * 
   */
  void Reshape(const vector<int>& shape); 
  void Reshape(const BlobShape& shape);
  void ReshapeLike(const Blob& other);
  
  /// 以 string 形式获取 shape_ ( 格式为 n c w h(count) ). 
  inline string shape_string() const {
    ostringstream stream;
    for (int i = 0; i < shape_.size(); ++i) {
      stream << shape_[i] << " ";
    }
    stream << "(" << count_ << ")";
    return stream.str();
  }
  inline const vector<int>& shape() const { return shape_; }   /// 获取 shape_    
  
  /**
   * @brief 返回第 index 条轴上的维度(如果 index 为负数，则从 end 端开始计数)
   *
   * @param 轴的 index, 因为是按照规范化设计, 因此可以是负值. 但是不能越界访问.     
   */ 
  inline int shape(int index) const {
    return shape_[CanonicalAxisIndex(index)];
  }

  inline int num_axes() const { return shape_.size(); } // 获取 blob 的维度 
  inline int count() const { return count_; }  // 获取当前 data 的大小  

  /**
   * @brief 计算 blob 的一个切片的 volume (切片区间为左闭右开)
   *        也就是在给定轴间范围内维度的乘积;
   *
   * @param start_axis slice 的起始轴(包含在内).
   * @param end_axis slice 的结束轴(不包含在内).
   */
  inline int count(int start_axis, int end_axis) const {
    CHECK_LE(start_axis, end_axis);
    CHECK_GE(start_axis, 0);
    CHECK_GE(end_axis, 0);
    CHECK_LE(start_axis, num_axes());
    CHECK_LE(end_axis, num_axes());

    int count = 1;
    for (int i = start_axis; i < end_axis; ++i) {
      count *= shape(i);
    }
    return count;
  }

  /**
   * @brief 给定切片的起始轴, 计算其到结束轴维度的 volume
   *
   * @param start_axis 切片的起始轴.
   */
  inline int count(int start_axis) const {  
    return count(start_axis, num_axes());
  }

  /**
   * @brief 返回用户指定轴的规范化版本,允许轴的 index 是负值, 可以对负数的索引进行转换.  
   *        (e.g., -1 表示最后一条轴).
   *
   * @param axis_index 轴的索引.
   *        如果 0 <= index < num_axes(), 直接返回 index.
   *        如果 -num_axes <= index <= -1, 返回 (num_axes() - (-index)),
   *        e.g., 最后一条轴的 index: (num_axes() - 1) ==> index == -1,
   */
  inline int CanonicalAxisIndex(int axis_index) const {
    CHECK_GE(axis_index, -num_axes())
        << "axis " << axis_index << " out of range for " << num_axes()
        << "-D Blob with shape " << shape_string();
    CHECK_LT(axis_index, num_axes())
        << "axis " << axis_index << " out of range for " << num_axes()
        << "-D Blob with shape " << shape_string();
    if (axis_index < 0) {
      return axis_index + num_axes();
    }
    return axis_index;
  }

  /**
   * 以下是 Blob 中的 4 个基本变量 num, channel, height, width 
   *
   * @brief 旧版本, 推荐使用 shape(0) 访问 num.
   * @brief 旧版本, 推荐使用 shape(1) 访问 channels.
   * @brief 旧版本, 推荐使用 shape(2) 访问 height.
   * @brief 旧版本, 推荐使用 shape(3) 访问 width.
   */
  inline int num() const { return LegacyShape(0); }
  inline int channels() const { return LegacyShape(1); }
  inline int height() const { return LegacyShape(2); }
  inline int width() const { return LegacyShape(3); }

  // @brief 旧版本, data_ 维数不大于 4 时才能使用, 功能同 shape() 类似。
  inline int LegacyShape(int index) const {
    CHECK_LE(num_axes(), 4)
        << "Cannot use legacy accessors on Blobs with > 4 axes.";
    CHECK_LT(index, 4);
    CHECK_GE(index, -4);
    
    if (index >= num_axes() || index < -num_axes()) {
      // Axis is out of range, but still in [0, 3] (or [-4, -1] for reverse
      // indexing) -- this special case simulates the one-padding used to fill
      // extraneous axes of legacy blobs.
      return 1;
    }

    return shape(index);
  }

  /**
   * offset 计算的方式也支持两种方式: 
   *    一种是指定 n,c,h,w   
   *    另一种是用 vector 指定 n,c,h,w   
   * 对应的偏移量 offset = ( (n * channels() + c) * height() + h ) * width() + w  
   *   
   * Note: 需要清楚内存中是按照什么轴序来排列的.  
   */
  inline int offset(const int n, const int c = 0, const int h = 0,
      const int w = 0) const {
    CHECK_GE(n, 0);
    CHECK_LE(n, num());
    CHECK_GE(channels(), 0);
    CHECK_LE(c, channels());
    CHECK_GE(height(), 0);
    CHECK_LE(h, height());
    CHECK_GE(width(), 0);
    CHECK_LE(w, width());

    return ((n * channels() + c) * height() + h) * width() + w;
  }

  inline int offset(const vector<int>& indices) const {
    CHECK_LE(indices.size(), num_axes());
    int offset = 0;
    for (int i = 0; i < num_axes(); ++i) {
      offset *= shape(i);
      if (indices.size() > i) {
        CHECK_GE(indices[i], 0);
        CHECK_LT(indices[i], shape(i));
        offset += indices[i];
      }
    }
    return offset;
  }

  /**
   * @brief 从一个 source Blob 拷贝数据到 gpu_data 或 cpu_data 
   *
   * @param source Blob
   * @param 如果 copy_diff = false 拷贝 data_; 反之拷贝 diff_
   * @param reshape if false, require this Blob to be pre-shaped to the shape
   *        of other (and die otherwise); if true, Reshape this Blob to other's
   *        shape if necessary
   */
  void CopyFrom(const Blob<Dtype>& source, bool copy_diff = false,
      bool reshape = false);

  /**
   * 下面这些函数可以通过给定的位置访问数据, 根据位置计算与数据起始地址之间的 
   * 的偏差 offset, 再通过 cpu_data* 指针获得地址 
   */  
  inline Dtype data_at(const int n, const int c, const int h,
      const int w) const {
    return cpu_data()[offset(n, c, h, w)];
  }
  inline Dtype diff_at(const int n, const int c, const int h,
      const int w) const {
    return cpu_diff()[offset(n, c, h, w)];
  }

  inline Dtype data_at(const vector<int>& index) const {
    return cpu_data()[offset(index)];
  }
  inline Dtype diff_at(const vector<int>& index) const {
    return cpu_diff()[offset(index)];
  }

  // 获取 data_ 指针
  inline const shared_ptr<SyncedMemory>& data() const {
    CHECK(data_);
    return data_;
  }

  // 获取 diff_ 指针 
  inline const shared_ptr<SyncedMemory>& diff() const {
    CHECK(diff_);
    return diff_;
  }

  ///
  // data 存储前向传递的数据, 而 diff 存储的是反向传播中的梯度   
  const Dtype* cpu_data() const;
  void set_cpu_data(Dtype* data);
  const int* gpu_shape() const;
  const Dtype* gpu_data() const;
  void set_gpu_data(Dtype* data);
  const Dtype* cpu_diff() const;
  const Dtype* gpu_diff() const;
  Dtype* mutable_cpu_data();   // mutable 是可变的, 即需要随时更新的
  Dtype* mutable_gpu_data();
  Dtype* mutable_cpu_diff();
  Dtype* mutable_gpu_diff();
  
  void Update();  // 即 data_ -= diff_;

  // 从 BlobProto 中恢复到 data_ 指向的 Blob 对象中 
  void FromProto(const BlobProto& proto, bool reshape = true); 
  // 将 data_ 指向的 Blob 序列化为 BlobProto, 编译保存为二进制文件   
  void ToProto(BlobProto* proto, bool write_diff = false) const; 

  /// @brief 计算 data_ 数据的绝对值之和(L1 norm).
  /// @brief 计算 diff_ 数据的绝对值之和(L1 norm).
  Dtype asum_data() const;
  Dtype asum_diff() const;
  /// @brief 计算 data_ 数据的平方和(L2 norm squared).
  /// @brief 计算 diff_ 数据的平方和(L2 norm squared).
  Dtype sumsq_data() const;
  Dtype sumsq_diff() const;

  /// @brief 使用常数因子对 data_ 和 diff_ 缩放.
  void scale_data(Dtype scale_factor);
  void scale_diff(Dtype scale_factor);

  /**
   * @brief 设置智能指针 data_ 指向 other Blob 中保存的数据内存空间(data_), 
   *        主要用于前向运算时 Layers 之间的拷贝操作.
   *
   * ShareData () 会释放 data_ 指针指向的内存空间, 因为对 data_ 智能指针使用 
   * "=" 赋值运算符会递减左侧指针的引用计数(递减后引用计数变为 0, 调用析构函数).  
   */
  void ShareData(const Blob& other);  // 本Blob共享other的data_  
  
  /**
   * @brief 设置智能指针 diff_ 指向 other Blob 中保存的数据内存空间(diff_), 
   *        主要用于前向运算时 Layers 之间的拷贝操作.
   *
   * ShareData () 会释放 diff_ 指针指向的内存空间, 因为对 diff_ 智能指针使用 
   * "=" 赋值运算符会递减左侧指针的引用计数(递减后引用计数变为 0, 调用析构函数).  
   */
  void ShareDiff(const Blob& other); // 本Blob共享other的diff_  

  bool ShapeEquals(const BlobProto& other);

 protected:
  ...

};  // class Blob
```

## blob.cpp 实现部分的重要代码    
对一些使用到的 tricks, 有难度的原理实现贴出其代码.   
1. ShareData(const Blob& other)   
智能指针的使用, 赋值之后, 左侧指针原先指向的内存空间被释放.   
```cpp 
template <typename Dtype>  
void Blob<Dtype>::ShareData(const Blob& other) {  
  CHECK_EQ(count_, other.count());  
  data_ = other.data();  
}  

template <typename Dtype>  
void Blob<Dtype>::ShareDiff(const Blob& other) {  
  CHECK_EQ(count_, other.count());  
  diff_ = other.diff();  
}  
```
2. Update()   
Update() 函数实现的就是学习过程中的参数更新过程. 即参数的更新(包括 weight，bias), 更新的方式就是梯度下降法(减去对应的导数).     
另外需要注意的是, 它只更新 CPU 或 GPU 上的数据.    
```cpp
/***
 *   Blobs 的参数是以 Blob<float> 或 Blob<double> 的数据形式存储的, 因此 
 *   我们不再定义 Blob<int> 或 Blob<unsigned int>.  
 */
template <typename Dtype>  
void Blob<Dtype>::Update() {  
  // 更新当前活动设备的 data_.  
  switch (data_->head()) {  
  case SyncedMemory::HEAD_AT_CPU:
    // 数据在 CPU 上，则在 CPU 上进行计算  
    caffe_axpy<Dtype>(count_, Dtype(-1),  
        static_cast<const Dtype*>(diff_->cpu_data()),  
        static_cast<Dtype*>(data_->mutable_cpu_data()));  
    break;  
  case SyncedMemory::HEAD_AT_GPU:  
  case SyncedMemory::SYNCED:  
#ifndef CPU_ONLY
    // 数据在 GPU 上，则在 GPU 上进行计算  
    caffe_gpu_axpy<Dtype>(count_, Dtype(-1),  
        static_cast<const Dtype*>(diff_->gpu_data()),  
        static_cast<Dtype*>(data_->mutable_gpu_data()));  
#else  
    NO_GPU;  
#endif  
    break;  
  default:  
    LOG(FATAL) << "Syncedmem not initialized.";  
  }  
}  
```
3. sumsq_data()   
计算 data_ 的元素平方和, 使用向量的内积实现.    
注意: `caffe_cpu_dot()` 和 `caffe_gpu_dot()` 函数的返回值类型是不同的.  
```cpp
template <typename Dtype>  
Dtype Blob<Dtype>::sumsq_data() const {  
  Dtype sumsq;  
  const Dtype* data;  
  if (!data_) { return 0; }  
  switch (data_->head()) {  
  case SyncedMemory::HEAD_AT_CPU:  
    data = cpu_data();  
    sumsq = caffe_cpu_dot(count_, data, data);  
    break;  
  case SyncedMemory::HEAD_AT_GPU:  
  case SyncedMemory::SYNCED:  
#ifndef CPU_ONLY  
    data = gpu_data();  
    caffe_gpu_dot(count_, data, data, &sumsq);  
#else  
    NO_GPU;  
#endif  
    break;  
  case SyncedMemory::UNINITIALIZED:  
    return 0;  
  default:  
    LOG(FATAL) << "Unknown SyncedMemory head state: " << data_->head();  
  }  
  return sumsq;  
}  
```
4. LegacyShape(int )   
返回 Blob 的维度值(n, c, h, w).    
**Note**:   
因为如果对于一维向量或者二维矩阵, 这维度的值就会小于 4. 当 Blob 的维度小于 4 时, 位于左边的维度被自动填充为 1.   
例如 bias Blob shape (1 x 1 x 1 x N), InnerProduct 的 weight Blob shape (1 x 1 x M x N). 
```cpp
// @brief 旧版本, data_ 维数不大于 4 时才能使用, 功能同 shape() 类似。
inline int LegacyShape(int index) const {
  CHECK_LE(num_axes(), 4)
      << "Cannot use legacy accessors on Blobs with > 4 axes.";
  CHECK_LT(index, 4);
  CHECK_GE(index, -4);
  
  if (index >= num_axes() || index < -num_axes()) {
    // index 越界, 但是仍然在 [0, 3](或者 [-4, -1]) 之间, 在这种特殊情况下,
    // 使用 1 填充 legacy blobs 的其他维度. 例如 bias Blob shape (1 x 1 x 1 x N), 
    // InnerProduct 的 weight Blob shape (1 x 1 x M x N). 
    return 1;
  }

  return shape(index);
}
```
5. ShapeEquals(const BlobProto& )
检查两个 Blob 的维度是否相等. 注意调用的是 `LegacyShape()` 函数.     
```cpp
// BlobProto 在 caffe.proto 中定义为一个 message，其字段有 
// data, diff, shape, num, channels, height, width  
template <typename Dtype>  
bool Blob<Dtype>::ShapeEquals(const BlobProto& other) {  
  if (other.has_num() || other.has_channels() ||  
      other.has_height() || other.has_width()) {  
    // 使用旧版本的 4D Blob 维度函数 --  
    // shape 是 (num, channels, height, width).  
    // Note: 不要使用 Blob::num(), Blob::channels()等这些方法, 因为它们的 index 是
    // 从 blob shape 的 beginning 开始计算的, 而 legacy 版本的 index 是从 blob shape 
    // 的 end 开始计算 shape 的. (e.g., bias Blob shape (1 x 1 x 1 x N),  
    // IP layer weight Blob shape (1 x 1 x M x N)).  
    return shape_.size() <= 4 &&  
           LegacyShape(-4) == other.num() &&  
           LegacyShape(-3) == other.channels() &&  
           LegacyShape(-2) == other.height() &&  
           LegacyShape(-1) == other.width();  
  }  
  vector<int> other_shape(other.shape().dim_size());  
  for (int i = 0; i < other.shape().dim_size(); ++i) {  
    other_shape[i] = other.shape().dim(i);  
  }  
  return shape_ == other_shape;  
}
```
6. ToProto(BlobProto* , bool ) const    
protobuf 的使用. 比如如何向一个 `repeated` 修饰的变量中添加值.   
```
template <typename Dtype>  
void Blob<Dtype>::ToProto(BlobProto* proto, bool write_diff) const {  
  proto->clear_shape();  
  for (int i = 0; i < shape_.size(); ++i) {  
    proto->mutable_shape()->add_dim(shape_[i]);  
  }  
  proto->clear_data();  
  proto->clear_diff();  
  const Dtype* data_vec = cpu_data();  
  for (int i = 0; i < count_; ++i) {  
    proto->add_data(data_vec[i]);    // 将 data 写入 proto  
  }  
  if (write_diff) {  
    const Dtype* diff_vec = cpu_diff();  
    for (int i = 0; i < count_; ++i) {  
      proto->add_diff(diff_vec[i]);  // 将 diff 写入 proto  
    }  
  }  
}  
```

## 和 Blob 相关的 Proto  
1. BlobProto 
用于保存为 protobuf 可以解析的二进制数据.     
```proto
message BlobProto {
  optional BlobShape shape = 7;
  repeated float data = 5 [packed = true];
  repeated float diff = 6 [packed = true];
  repeated double double_data = 8 [packed = true];
  repeated double double_diff = 9 [packed = true];

  // 4D dimensions -- deprecated.  Use "shape" instead.
  optional int32 num = 1 [default = 0];
  optional int32 channels = 2 [default = 0];
  optional int32 height = 3 [default = 0];
  optional int32 width = 4 [default = 0];
}
```
2. BlobShape   
用于指定一个 Blob 的 shape (dimensions).   
```proto
message BlobShape {
  repeated int64 dim = 1 [packed = true];
}
```