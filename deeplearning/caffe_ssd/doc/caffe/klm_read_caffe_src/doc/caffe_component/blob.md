# BlobFlow  

> 听说Google出了TensorFlow，那么Caffe应该叫什么？
>　　　　　　　　　　　　　　　　　　　　　　　　　　——BlobFlow

 
Caffe 中贯穿整个网络中的数据都用 Blob 存储. 例如网络中正向和反向迭代时的数据和导数信息。  

Layer 是 Caffe 模型和计算的基本单元， Net 是一系列 layers 和其连接的集合. Blob 详细描述了信息是如何在 layer 和 net 中存储和交互的.   

> Blob 实现的头文件是`caffe/include/caffe/blob.hpp`.    
 
## 神经网络时代的传播数据结构  

### 1. 我的代码   

我最早手写神经网络的时候, Flow 结构是这样的：   

```cpp
struct Data
{
    vector<double> feature;
    int y;
    Data(vector<double> feature,int y):feature(feature),y(y) {}
};
vector<double> u_i, v_i, u_j, v_j;
```

很简陋的结构, 主要功能就是利用 vector 存一下每层正向传播的值。   

### 2. Word2Vec  

后来我看了 Google 的 Mikolov 大神的 Word2Vec 的源码, 它的 Flow 结构是这样的:   
```cpp
real *neu1 = (real *)calloc(doc->layer1_size, sizeof(real));
```
然后我吐槽了一下, 这功能不是比我还弱么, vector 起码还能提供 STL 的基础功能.  注： Word2Vec 源码是以 CPU 多线程和内存操作快而著称的, 简陋但速度快）.   

### 3. Theano

再后来, 我学习了 Theano, 它的 Flow 结构是这样的:   
```python
input=theano.tensor.matrix('x')

class DataLayer(object):
    def __init__(self,input,batch_size,size):
        self.batch_size=batch_size
        self.size=size
        self.input=input
        self.params=None

    def get_output(self):
        output=self.input
        if type(self.size) is tuple: # Mode: 2D
            output=output.reshape((self.batch_size,self.size[2],self.size[1],self.size[0]))
        else: #Mode: 1D
            output=output.reshape((self.batch_size,self.size))
        
        return output
```
Bengio 组模仿物理学的张量(Tensor)的概念, 创建了 Theano 的 Tensor 系统。Dim 为 0 的叫常量, Dim 为 1 的叫向量, Dim = 2的叫矩阵, Dim > 2 就没名字了, 且 Dim 可以无限扩大。  

Tensor 的出现,  很好地规避了机器学习研究者不会写代码的问题（比如上节出现的简陋结构）。同时, 随着 mini-batch, conv 等方法在深度学习中的大规模使用, 我们的 Flow 结构显然需要多维化。   

由于是操作多维空间, 经常需要维度切换, reshape() 函数自然成了 Tensor 的核心函数(reshape 的概念最早应该来自 Python 的科学计算库 numpy, Theano 的 Tensor 系统, 很大程度上在重写 numpy）.   

### 4. TensorFlow

再后来 Google 把 Andrew Ng 开发的一代深度学习框架 DistBelief 给换掉了, 第二代叫 TensorFlow. 按照官方的说法, 取名TensorFlow(2015) 的原因是因为系统里主要是 Tensor 在 Flow. 

推测一下 DistBelief(2011) 和 Theano(NIPS2012) 的公布时间, 我们大概推测, DistBelief 的 Flow 结构估计相当 Low. 按照Caffe(2013) 作者贾大神的说法, 他参与了 TensorFlow 的主体开发。所以, TensorFlow 里的 Tensor 结构, 不难看出来, 是借鉴了 Theano(2012) 和 Caffe(2013) 的综合体。   

### 5. 符号系统

尽管 Caffe(2013) 具有类似 Tensor 的 Blob 结构, 但是和 Theano(2012), TensorFlow(2015) 的 Tensor 相比, 还是比较弱的。核心原因是, Tensor 的出发点是建立在符号系统上的, 而 Caffe(2013) 只是最暴力的执行代码。  

按照 MXNet 的陈天奇大神在 MS 研究院内部的讲座说法：   
- Caffe(2013) 属于 Imperative Programme(命令程序), 白盒模式;   
- Theano(2012), TensorFlow(2015), MXNet(2015) 属于 Declaretive Programme(声明程序), 黑盒模式;  

符号系统需要内建一套数学式语法解析结构, 针对原始的命令语句做一个深度的 Wrapper, 从白盒变成黑盒。其难度和代码量还是有的。与之相比,  Blob 读起来, 还是要比 Tensor 简单地多。

## 浅析Blob设计原理  

### 1. 存储性质  

无论是 `正向传播的输出`,  还是 `反向传播的残差`, 还是 `神经元参数`, 这些都需要不同的结构去存储。  

Blob 广义上极力规避设计多种结构的问题, 这点上是参考 Tensor 的。你可以自由规划 1D, 2D, 3D, 4D 甚至 nD 的多维数组存储空间, 这种存储具有相当不错的灵活性。  

### 2. 功能性质

不幸的是, 操作多维数组在编程中是件麻烦事。朴素 C 语言提供的多维数组, 功能很弱, 比如你想获知大小(size)就是一件难事。使用 STL 是一个不错的选择, 嵌套 STL, 从数据结构角度就变成了广义表。 尽管广义表的功能较朴素 C 语言多维数组要多, 不过看起来也不尽如人意。   

另外, 最恼人的是 CUDA 不推荐 GPU 操作多维数组, 最多可以申请到 3 维数组的显存优化。如果不使用 CUDA 提供的多维数组内存对齐优化, 那么 IO 指令取址将会非常频繁, 导致 IO 速度严重退化。从内存角度理解, 显然线性内存空间访问便捷, nD 内存空间就十分糟糕了。   

从 SyncedMemory 的设计中, 几乎就可以推测 Caffe 为了速度, 完全使用线性内存/显存。因而, 为使线性内存模拟出 nD 内存, 就需要在内存访问上做点偏移(Offset)计算。  

Blob 的大部分功能, 便是扩展线性 SyncedMemory 的逻辑功能, 使其变成逻辑上的多维数组。   

### 3. 张量·轴设计

在早期神经网络编程中, 通常采用的是 1D 空间, 每个样本拥有一个输入向量。 上个世纪末, LeCun 等人倡导在 SGD 中, 替代单样本为mini-batch, 才使得轴设计得以派上用场。  

axis=0 用于 batch_size, batch 中每个样本的向量移到 axis=1。这种空间在今天的神经网络 NLP（NNNLP）任务中, 仍然被主要采用。  

上个世纪 90 年代初, LeCun 将 Fukushima 的神经机结合导师 Hinton 的 BP 算法, 演化成可以训练的 CNN, 使得轴进一步扩展。CNN 所扩展的轴, 称之为空间轴(spatial axes), 放置于 axis=2, ... 之后。   

原神经网络的 axis=1 轴, 结合图像文件的通道(channels)概念, CNN 的特征图概念, 被替换成 channels axis。这样在 Blob 中, 就构成了使用最频繁的 4 轴空间(batch_size, channels, height, width）。   

在 Caffe 中, batch_size 用 num 替代, 这个名字理解起来更泛性一点. 各轴都具有一定的轴长, 描述轴空间需要 shape 功能, 轴空间变形则需要 reshape 功能。   

## 代码实战

从 Blob 开始, 为了便于阅读, 代码将在不同章逐步扩展, 以下仅提供适用于本章的精简代码。  

建立blob.hpp

### 1. 数据结构

```cpp
template <typename Dtype>
class Blob{
public:
    Blob():data_(),diff_(),count_(0), capacity_(0) {}
    Blob(const vector<int>& shape) :count_(0),capacity_(0) { reshape(shape); }
    
    void reshape(int num, int channels, int height, int width);
    void reshape(vector<int> shape);
    void reshape(const BlobShape& blob_shape);
    void reshapeLike(const Blob& blob);
    
    const Dtype* cpu_data() const;
    const Dtype *gpu_data() const;
    const Dtype* cpu_diff() const;
    const Dtype* gpu_diff() const;
    // mutable 是可变的, 它不是常量成员函数 
    Dtype *mutable_cpu_data();  
    Dtype *mutable_gpu_data();
    Dtype *mutable_cpu_diff();
    Dtype *mutable_gpu_diff();

    int num() const { return shape(0); }
    int channels() const { return shape(1); }
    int height() const { return shape(2); }
    int width() const { return shape(3); }
    int count() const{ return count_; }
    int count(int start_axis, int end_axis) const {
        CHECK_GE(start_axis, 0);
        CHECK_LE(start_axis, end_axis);
        CHECK_LE(start_axis, num_axes());
        CHECK_LE(end_axis, num_axes());
        int cnt = 1;
        for (int i = start_axis; i < end_axis; i++) cnt *= shape(i);
        return cnt;
    }
    int count(int start_axis) const{ return count(start_axis, num_axes()); }
    const vector<int> &shape() const{ return shape_; }
    int shape(int axis) const{ return shape_[canonicalAxisIndex(axis)]; }
    int offset(const int n, const int c = 0, const int h = 0,
        const int w = 0){
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
    int num_axes() const { return shape_.size(); }

    // idx ranges [-axes,axes), idx(-1) means the last axis
    int canonicalAxisIndex(int axis) const{
        CHECK_GE(axis, -num_axes());
        CHECK_LT(axis, num_axes());
        if (axis < 0) return axis + num_axes();
        else return axis;
    }

    const boost::shared_ptr<SyncedMemory>& data() const { return data_; }
    const boost::shared_ptr<SyncedMemory>& diff() const { return diff_; }
    
    // change the shared_ptr object and will recycle the memory if need
    void shareData(const Blob& blob) {
        CHECK_EQ(count(), blob.count());
        data_ = blob.data(); 
    }
    void shareDiff(const Blob& blob) {
        CHECK_EQ(count(), blob.count());
        diff_ = blob.diff();
    }

    void FromProto(const BlobProto& proto, bool need_reshape = true);
    void ToProto(BlobProto* proto, bool write_diff = false);

    void Update();  // 即 data_ -= diff_;

    /// @brief 计算 data_ 数据的绝对值之和(L1 norm).
    /// @brief 计算 diff_ 数据的绝对值之和(L1 norm).
    /// @brief 计算 data_ 数据的平方和(L2 norm squared).
    /// @brief 计算 diff_ 数据的平方和(L2 norm squared).
    Dtype asum_data() const;
    Dtype asum_diff() const;
    Dtype sumsq_data() const;
    Dtype sumsq_diff() const;

    /// @brief 使用常数因子对 data_ 和 diff_ 缩放.
    void scale_data(Dtype scale_factor);
    void scale_diff(Dtype scale_factor);

    bool ShapeEquals(const BlobProto& other);

protected:
    boost::shared_ptr<SyncedMemory> data_, diff_;
    vector<int> shape_;
    int count_, capacity_;
};
```

先说说几个成员变量：  
- count, capacity 用于 reshape() 中的计算, 前者是新 reshape 的大小, 后者是历史 reshape 大小。Blob 的任何构造函数中, 一定要将这两个值置 0, 否则 reshape() 会失败。   
- 线性内存空间以 shared_ptr 绑定, 因此 Blob 不需要析构函数, Blob 销毁后, 指针空间会被自动回收。   
- 默认有2个线性内存空间, data, diff, 分别用于存储"前向数据/反向残差"。  
- TODO: shape_data_ 和 shape 的区别是什么?  
- vector<int> shape 用于存各个轴的轴长。  

然后看轴相关函数：
- num(), channels(), height(), width(), count(), shape() 都是简单的封装, 注意设成常量成员函数, 由于 Blob 会作为const 引用的参数, 比如 sharedData/sharedDiff, 这些访问接口必须保证 this 指针属性一致。   
- count() 和 shape() 都是重载函数, 提供不同的访问方式。  
- 轴访问 canonicalAxisIndex() 函数上, 借鉴了 Python 的负轴访问方式, 如果你没有 Python 的习惯, 可以写简单点。   

然后看 data 相关函数：
cpu_data(), gpu_data(), cpu_diff(), gpu_diff(), mutable_cpu_data(), mutable_gpu_data(), mutable_cpu_diff(), mutable_gpu_diff() 是对 SyncedMemory 中相应接口的封装, 主要目的是将 void* 型内存(SyncedMemory 中定义的)转换为计算类型(float / double 类型)的内存。  

void* 型内存以数组下标方式访问时, 每个单元占用 8 Bit（1字节）, 这种单元内存是不能直接使用的. 因为一个 int/float 单元占 32Bit(4字节), 一个 double 单元占 64Bit(8字节). C/C++通过对数组首元素指针的强制转换, 可以改变下标索引的单元访问模式。  

reshape() 函数看起来重载了很多, 实际上主体设在 `void reshape(vector<int> shape)` 里。

`offset()` 函数是非常重要的, 它目的是计算相对偏移量, 将 1D 空间结构形成逻辑上的 nD 多维空间结构。  

在 DataLayer 中, 由 Datum 组织 Blob 一个例子如下：   
```cpp
Dtype* top_data = batch->data_.mutable_cpu_data();  // blob 首地址  
...
for (int item_id = 0; item_id < batch_size; ++item_id) {
    // get a datum
    Datum& datum = *(reader_.full().pop("Waiting for data"));
    // 遍历所有的 batch, Apply data transformations (mirror, scale, crop...)
    int offset = batch->data_.offset(item_id);  
    this->transformed_data_.set_cpu_data(top_data + offset);
    this->data_transformer_->Transform(datum, &(this->transformed_data_));
    // Copy label.
    if (this->output_labels_) {
      top_label[item_id] = datum.label();
    }

    reader_.free().push(const_cast<Datum*>(&datum));
}
```

在这里, 对 batch 里的每一个样本, 每次偏移 `channels*height*width` 个单位, 就会立刻跳转到下一张图的首元素。更一般的, 令 `offset = batch->data_.offset(0,1)`,  就跳转到了下一个 channel 的首元素。由于线性空间是连续的, 这种偏移仅仅需要加法器一次运算, 就能模拟出多维空间, 十分廉价。  

两个 share 函数用于直接替换掉 data_, diff_,  由于使用了 shared_ptr, data_, diff_ 之前指向的 SyncedMemory 空间会自动释放。共享数据的场景可以是: 当神经网络需要交叉验证时,  没有必要从训练网络 copy 参数到测试网络, 此时只要将训练网络的全部参数的 Blob, 一一对应 share 给测试网络即可。   

FromProto() 和 ToProto() 用于反序列化/序列化至 protobuf 格式。唯一用处是对神经网络的参数 Blob 进行 snapshot(截图), 以便继续训练或者离线测试。   

### 2. 实现

给出几个比较重要的实现。  

```cpp
template <typename Dtype>  
void Blob<Dtype>::Update() {  
  switch (data_->head()) {  
  case SyncedMemory::HEAD_AT_CPU:
    caffe_axpy<Dtype>(count_, Dtype(-1),  
        static_cast<const Dtype*>(diff_->cpu_data()),  
        static_cast<Dtype*>(data_->mutable_cpu_data()));  
    break;  
  case SyncedMemory::HEAD_AT_GPU:  
  case SyncedMemory::SYNCED:  
#ifndef CPU_ONLY
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
Update() 函数负责学习模型过程中的参数的更新(包括 weight，bias), 更新的方式就是梯度下降法(减去对应的梯度值).     

```cpp
template<typename Dtype>
void Blob<Dtype>::reshape(vector<int> shape){
    count_ = 1;
    shape_.resize(shape.size());
    for (int i = 0; i < shape.size(); ++i) {
        count_ *= shape[i];
        shape_[i] = shape[i];
    }
    if (count_ > capacity_) {
        capacity_ = count_;
        data_.reset(new SyncedMemory(capacity_ * sizeof(Dtype)));
        diff_.reset(new SyncedMemory(capacity_ * sizeof(Dtype)));
    }
}
```

可以看到, reshape() 为 SyncedMemory 准备了 capacity*sizeof(Dtype) 个字节单元。同时, 你需要回忆一下,  SyncedMemory(size) 并不会立刻启动状态转移自动机申请内存/显存。只有在执行 Blob:: cpu_data()/gpu_data()/mutable_cpu_data()/mutable_gpu_data() 时才会申请。   

这有点像函数式编程里的 Lazy 思想, 胡乱写 Blob 其实问题不大, 只要该 Blob 没有使用, 就不会有内存空间损耗。     

```cpp
template<typename Dtype>
void Blob<Dtype>::ToProto(BlobProto* proto, bool write_diff){
    proto->clear_shape();
    proto->clear_data();
    proto->clear_diff();
    //do not use proto->shape() cause it is a const method
    for (int i = 0; i < shape_.size(); i++)  proto->mutable_shape()->add_dim(shape_[i]);
    const Dtype *data = cpu_data();
    const Dtype *diff = cpu_diff();
    for (int i = 0; i < count_; i++)  proto->add_data(data[i]);
    if (write_diff)
        for (int i = 0; i < count_; i++)  proto->add_diff(diff[i]);
}
```

ToProto 里, 首次出现了如何向 protobuf 结构写数据的例子。以 proto->mutable_shape() 为例, 切记不要写成 proto->shape(), 因为 proto->shape() 是常量成员函数, 其内部不能修改. 这点同 Blob::cpu_data()/mutable_cpu_data() 的原理是一致的。  

对于 message 的 repeated 类型, 使用 add_xxx() 函数可以填充数组数据。   

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
智能指针的使用, 赋值之后, 左侧指针原先指向的内存空间被释放.   


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
计算 data_ 的元素平方和, 使用向量的内积实现. 注意: `caffe_cpu_dot()` 和 `caffe_gpu_dot()` 函数的返回值类型是不同的.  


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
LegacyShape(int) 返回 Blob 的维度值(n, c, h, w). 对于一维向量或者二维矩阵, 维度的轴的个数小于 4. 但是为了保持计算接口的统一性, 如果 Blob 维度的轴数小于 4 时, 位于左边的维度被自动填充为 1. 例如 bias Blob shape (1 x 1 x 1 x N), InnerProduct 的 weight Blob shape (1 x 1 x M x N).   

```cpp
// BlobProto 在 caffe.proto 中定义为一个 message，其字段有 
// data, diff, shape, num, channels, height, width  
template <typename Dtype>  
bool Blob<Dtype>::ShapeEquals(const BlobProto& other) {  
  if (other.has_num() || other.has_channels() ||  
      other.has_height() || other.has_width()) {  
 
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
检查两个 Blob 的维度是否相等. 注意调用的是 `LegacyShape()` 函数.     


## Caffe 可做的精简   
- 移除 SyncedMemory 形式的 shape_data, 与 vector<int> shape_ 作用重复
- 移除基本没什么用的 CopyFrom 函数

