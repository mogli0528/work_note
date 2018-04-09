# synced_memory 同步内存的实现
> host 指的是宿主设备(CPU / GPU).    

## 目录   
- synced_memory 的作用;  
- synced_memory 的实现;  

## synced_memory 内存的作用    
Caffe 中内存分配和底层数据的切换(cpu 模式和 gpu 模式)需要用到内存同步模块. 这类个类的代码比较少, 但是作用是非常明显的.   

## synced_memory 的实现    
1. 分配空间  -->  CaffeMallocHost()    
如果在 GPU 模式下 CUDA 是可用的, 那么使用 cudaMallocHost() 函数分配得到的是`钳制的`内存. 它避免了 DMA 传输模式下的动态钳制. 单 GPU 情况下并不会提高程序性能, 但是并行训练的时候会显著提高性能. 在多 GPU 情况下大模型的性能会更加显著的提升.   
我们知道内存里面是由页作为基本的管理单元, 分配的内存可以常驻在内存空间中对效率是有帮助的, 空间不会被别的进程所抢占.    
```cpp
inline void CaffeMallocHost(void** ptr, size_t size, bool* use_cuda) {
#ifndef CPU_ONLY
  if (Caffe::mode() == Caffe::GPU) {
    CUDA_CHECK(cudaMallocHost(ptr, size));
    *use_cuda = true;
    return;
  }
#endif
#ifdef USE_MKL
  *ptr = mkl_malloc(size ? size:1, 64);
#else
  *ptr = malloc(size);
#endif
  *use_cuda = false;
  CHECK(*ptr) << "host allocation of size " << size << " failed";
}
```
2. 释放空间  --> CaffeFreeHost()   
```cpp
inline void CaffeFreeHost(void* ptr, bool use_cuda) {
#ifndef CPU_ONLY
  if (use_cuda) {
    CUDA_CHECK(cudaFreeHost(ptr));
    return;
  }
#endif
#ifdef USE_MKL
  mkl_free(ptr);
#else
  free(ptr);
#endif
}

```
3. SyncedMemory 类的实现.   
以下是`syncedmem.hpp`的代码：    
```cpp
class SyncedMemory {
public:
  
  SyncedMemory();    // 简单初始化
  explicit SyncedMemory(size_t size);  // 只设置 size，并未申请内存
  ~SyncedMemory();

  // 分别是获取 cpu，gpu 中数据的指针，需要说明的一点是，该过程会调用 to_xpu() 同步数据
  const void* cpu_data();
  const void* gpu_data();

  // 有 get 就有 set: set_cpu_data() 和 set_gpu_data()
  // 注意：set_xpu_data() 后就不再拥有该数据，即 own_cpu_data 或
  // own_gpu_data 就变为 false，因为还有 data 指向该数据。
  // 一般来说, 只有当同步后才会为 true, 也即调用 to_cpu() 或者 to_gpu() 后。
  void set_cpu_data(void* data);   
  void set_gpu_data(void* data);

  // 获取实时更新的 cpu 数据, 并改变数据的状态为 HEAD_AT_CPU, 这样就可以保证 
  // to_gpu() 函数同步数据到 GPU 时会重新拷贝 cpu 中的数据.  
  void* mutable_cpu_data(); 
  void* mutable_gpu_data();
  
  // SymceHead 有四种状态: [无数据, 数据在 cpu, gpu, 或者在 cpu 和 gpu 中都有]  
  enum SyncedHead { UNINITIALIZED, HEAD_AT_CPU, HEAD_AT_GPU, SYNCED };
  SyncedHead head() { return head_; }
  size_t size() { return size_; }

#ifndef CPU_ONLY
  void async_gpu_push(const cudaStream_t& stream);
#endif

private:
  void check_device();  // 主要是针对 GPU 设备检查. 
  
  /**
   * 功能: 把数据放到 cpu 上, 分以下几种情形处理:    
   *  1.如果数据未初始化，则在 cpu 中申请内存(初始化为 0)。此时状态为 HEAD_AT_CPU
   *  2.如果数据本来在 gpu，则从 gpu 拷贝内存到 cpu 。此时状态为 SYNCED
   *  3.如果数据本来在 cpu，不做处理
   */  
  void to_cpu();

  /**
   * 功能：把数据放到 gpu 上
   *  1.数据未初始化，在 gpu 中申请内存(初始化为 0)。此时状态为 HEAD_AT_GPU
   *  2.数据在 cpu，从 cpu 拷贝到 gpu。此时状态为 SYNCED
   *  3.数据在 gpu，不做操作。
   */
  void to_gpu();

  void* cpu_ptr_;  // 指向 cpu 的数据指针
  void* gpu_ptr_;  // 指向 gpu 的数据指针
  size_t size_;    // 数据大小
  SyncedHead head_;//数据存放的位置，枚举值之一
  
  //
  // own_gpu_data_ 和 own_cpu_data_这两个成员变量的作用是:
  // 表示是否独占该数据(只是是否可以释放这片内存)，也即在 cpu 或 gpu 中是否有其他指针
  // 指向该数据, 如果有, 系统就不能释放这片内存空间.  
  bool own_cpu_data_;
  bool own_gpu_data_;
  bool cpu_malloc_use_cuda_;    // 默认为 false
  int device_;      // GPU 设备号
 
  /**
   * DISABLE_COPY_AND_ASSIGN 宏禁用该类的拷贝构造函数和 "=" 操作符
   * 如果想让你的类不能使用 copy 构造函数和赋值操作符， 只要将该类的
   * 拷贝构造函数和"="操作符函数定义为 private, 并且只声明不实现.   
   */  
  DISABLE_COPY_AND_ASSIGN(SyncedMemory);
};  // class SyncedMemory
```