# math_functions —— Caffe 中的矩阵计算   
> math_functions.cpp 属于 Caffe 中的 util, util 中包含很多有用的工具, 这里我们就只分析和求解器直接相关的工具. `Caffe` 矩阵运算使用的库是 `CBLAS`库, 其头文件为 `<cblas.h>`.   

## 目录   
- cuda 的设计技巧; 
- Caffe 中的矩阵计算函数

## Caffe 中的矩阵计算函数    
这些函数的实现是在 `caffe/src/caffe/util/math_functions.cpp` 文件中。  
1. 基础知识   
计算矩阵乘法的函数之一是 `cblas_sgemm()`，该函数使用单精度实数，另外还有对应的双精度实数，
单精度复数和双精度复数的函数。
```cpp
void cblas_sgemm( const enum CBLAS_ORDER Order, const enum CBLAS_TRANSPOSE TransA,
                  const enum CBLAS_TRANSPOSE TransB, const int M, const int N,
                  const int K, const float alpha, const float  *A,
                  const int lda, const float  *B, const int ldb,
                  const float beta, float  *C, const int ldc )
```
得到的结果是:   
`C = alpha*op( A ) * op( B ) + beta * C`.   
**参数说明**:  
Order: 是数据的存储形式， `CBLAS` 的函数中, 无论一维还是二维数据都是用一维数组存储，这就要涉及是行主序还是列主序. `C 语言`中数组是用行主序，`fortran` 中是列主序。如果是行主序, 这个参数是用 `CblasRowMajor`; 如果是列主序的话就是 `CblasColMajor`;   
TransA, TransB: 是否对矩阵 A, B 进行转置;  
M: 矩阵 A 的行数， 矩阵 C 的行;   
N: 矩阵 B 的列数， 矩阵 C 的列;   
K: 矩阵 A 的列数， 矩阵 B 的行;   
alpha, beta: 计算公式中的两个参数值，如果只是计算 `C = A * B`，则 `alpha = 1, beta = 0`;  
A, B, C: 矩阵 A B C 的数据;   
lda, ldb, ldc: 在 CBLAS 的文档里，这三个参数分别为 A B C 的行数; 但是实际使用发现，应该是列数;   
2. 矩阵乘法 --> caffe_cpu_gemm()   
功能: `C = alpha * A * B + beta * C`.   
如果要计算 `C = A * B`, 则 `alpha = 1, beta = 0`;   
```cpp
template<>
void caffe_cpu_gemm<float>( const CBLAS_TRANSPOSE TransA,
                            const CBLAS_TRANSPOSE TransB, 
                            const int M, const int N, const int K,
                            const float alpha, const float beta,
                            const float* A, const float* B, float* C) {
  int lda = (TransA == CblasNoTrans) ? K : M;
  int ldb = (TransB == CblasNoTrans) ? N : K;
  cblas_sgemm( CblasRowMajor, TransA, TransB, 
               M, N, K, 
               alpha, A, lda, 
               B, ldb, beta, 
               C, N);
}
template<>
void caffe_cpu_gemm<double>( const CBLAS_TRANSPOSE TransA,
                             const CBLAS_TRANSPOSE TransB,
                             const int M, const int N, const int K,
                             const double alpha, const double beta,
                             const double* A, const double* B, double* C) {
  int lda = (TransA == CblasNoTrans) ? K : M;
  int ldb = (TransB == CblasNoTrans) ? N : K;
  cblas_dgemm(CblasRowMajor, TransA, TransB, M, N, K, alpha, A, lda, B,
      ldb, beta, C, N);
}
```
3. 矩阵和向量乘法 --> cblas_sgemv()   
功能： `y = alpha * A * x + beta * y`   
参数:   
x 和 y 是向量, A 是矩阵;   
M: A 的行数;
N: A 的列数;
```cpp
void cblas_sgemv(const enum CBLAS_ORDER order,
                 const enum CBLAS_TRANSPOSE TransA, const int M, const int N,
                 const float alpha, const float *A, const int lda,
                 const float *X, const int incX, const float beta,
                 float *Y, const int incY);
```
`cblas_sgemv()` 中的参数 `incX = 1` 和参数 `incY = 1` 表示对 x 和 y 的每个元素都进行操作, 步长为 1, 可理解为一步一步的增长;   
```cpp
template <>
void caffe_cpu_gemv<float>(const CBLAS_TRANSPOSE TransA, const int M,
    const int N, const float alpha, const float* A, const float* x,
    const float beta, float* y) {
  cblas_sgemv(CblasRowMajor, TransA, M, N, alpha, A, N, x, 1, beta, y, 1);
}
template <>
void caffe_cpu_gemv<double>(const CBLAS_TRANSPOSE TransA, const int M,
    const int N, const double alpha, const double* A, const double* x,
    const double beta, double* y) {
  cblas_dgemv(CblasRowMajor, TransA, M, N, alpha, A, N, x, 1, beta, y, 1);
}
```
4. 矩阵加法   
4.1 矩阵加法 --> caffe_axpy()   
功能： `Y = alpha * X + Y`   
N: 为 X 和 Y 中 element 的个数    
```cpp
template <>
void caffe_axpy<float>(const int N, const float alpha, const float* X,
    float* Y) { cblas_saxpy(N, alpha, X, 1, Y, 1); }
template <>
void caffe_axpy<double>(const int N, const double alpha, const double* X,
    double* Y) { cblas_daxpy(N, alpha, X, 1, Y, 1); }
```
4.2  矩阵加法 --> caffe_cpu_axpby()   
功能: `Y = alpha * X + beta * Y`.   
```cpp
template <>
void caffe_cpu_axpby<float>(const int N, const float alpha, const float* X,
                            const float beta, float* Y) {
  cblas_saxpby(N, alpha, X, 1, beta, Y, 1);
}

template <>
void caffe_cpu_axpby<double>(const int N, const double alpha, const double* X,
                             const double beta, double* Y) {
  cblas_daxpby(N, alpha, X, 1, beta, Y, 1);
}
```
5. 向量初始化 --> caffe_set()    
功能： 用常数 alpha 对 Y 进行初始化.   
使用 `memset()` 函数来初始化数组或者结构体比其他初始化方法更快一点.   
```cpp
template <typename Dtype>
void caffe_set(const int N, const Dtype alpha, Dtype* Y) {
  if (alpha == 0) {
    memset(Y, 0, sizeof(Dtype) * N);  // NOLINT(caffe/alt_fn)
    return;
  }
  for (int i = 0; i < N; ++i) {
    Y[i] = alpha;
  }
}
template void caffe_set<int>(const int N, const int alpha, int* Y);
template void caffe_set<float>(const int N, const float alpha, float* Y);
template void caffe_set<double>(const int N, const double alpha, double* Y);
```
区分 alpha 是否为 0, 选择不同的初始化方式(memset() 或是 for 循环), 因为使用 `memset()` 非零初始化会出现字节拼接现象;   
6. 矩阵加上常量 --> caffe_add_scalar()  
功能: `Y = Y + alpha`, 给 Y 的每个 element 加上常数 alpha   
```cpp
template <>
void caffe_add_scalar(const int N, const float alpha, float* Y) {
  for (int i = 0; i < N; ++i) {
    Y[i] += alpha;
  }
}
template <>
void caffe_add_scalar(const int N, const double alpha, double* Y) {
  for (int i = 0; i < N; ++i) {
    Y[i] += alpha;
  }
}
```
7. 矩阵拷贝初始化 --> caffe_copy()   
功能: 使用一个已知矩阵通过拷贝操作初始化矩阵.   
memcpy() 用来拷贝 src 所指的内存内容前 n 个字节到 dest 所指的内存地址上。   
与 strcpy() 不同的是, memcpy() 会完整的复制 n 个字节, 不会因为遇到字符串结束 '\0' 而结束.   
```cpp
template <typename Dtype>
void caffe_copy(const int N, const Dtype* X, Dtype* Y) {
  if (X != Y) {
    if (Caffe::mode() == Caffe::GPU) {
#ifndef CPU_ONLY
      // NOLINT_NEXT_LINE(caffe/alt_fn)
      CUDA_CHECK(cudaMemcpy(Y, X, sizeof(Dtype) * N, cudaMemcpyDefault));
#else
      NO_GPU;
#endif
    } else {
      memcpy(Y, X, sizeof(Dtype) * N);  // NOLINT(caffe/alt_fn)
    }
  }
}
template void caffe_copy<int>(const int N, const int* X, int* Y);
template void caffe_copy<unsigned int>(const int N, const unsigned int* X,
    unsigned int* Y);
template void caffe_copy<float>(const int N, const float* X, float* Y);
template void caffe_copy<double>(const int N, const double* X, double* Y);
```
8. 矩阵元素缩放 --> caffe_scal()   
功能: `X = alpha * X`.    
N: X 中 element 的个数.  
`void cblas_sscal(const int N, const float alpha, float *X, const int incX);` 中 `incX` 参数指示计算的元素个数的步长.   
```cpp
template <>
void caffe_scal<float>(const int N, const float alpha, float *X) {
  cblas_sscal(N, alpha, X, 1);
}

template <>
void caffe_scal<double>(const int N, const double alpha, double *X) {
  cblas_dscal(N, alpha, X, 1);
}
```
9. 矩阵四则运算.   
功能： 这四个函数分别实现 element-wise 的加减乘除 (y[i] = a[i] + - * \ b[i]).    
```cpp
template <>
void caffe_add<float>(const int n, const float* a, const float* b,
    float* y) {
  vsAdd(n, a, b, y);
}
template <>
void caffe_add<double>(const int n, const double* a, const double* b,
    double* y) {
  vdAdd(n, a, b, y);
}

template <>
void caffe_sub<float>(const int n, const float* a, const float* b,
    float* y) {
  vsSub(n, a, b, y);
}
template <>
void caffe_sub<double>(const int n, const double* a, const double* b,
    double* y) {
  vdSub(n, a, b, y);
}

template <>
void caffe_mul<float>(const int n, const float* a, const float* b,
    float* y) {
  vsMul(n, a, b, y);
}
template <>
void caffe_mul<double>(const int n, const double* a, const double* b,
    double* y) {
  vdMul(n, a, b, y);
}

template <>
void caffe_div<float>(const int n, const float* a, const float* b,
    float* y) {
  vsDiv(n, a, b, y);
}
template <>
void caffe_div<double>(const int n, const double* a, const double* b,
    double* y) {
  vdDiv(n, a, b, y);
}
```
10. 幂运算, 平方, 指数运算, 对数运算, 开方和 L1 距离.   
功能: 同样是 element-wise 操作.  
分别是:   
```
y[i] = a[i] ^ b    
y[i] = a[i] ^ 2
y[i] = sqrt(a[i])    
y[i] = exp(a[i])    
y[i] = |a[i]|
```
```cpp
template <>
void caffe_powx<float>(const int n, const float* a, const float b,
    float* y) {
  vsPowx(n, a, b, y);
}
template <>
void caffe_powx<double>(const int n, const double* a, const double b,
    double* y) {
  vdPowx(n, a, b, y);
}

template <>
void caffe_sqr<float>(const int n, const float* a, float* y) {
  vsSqr(n, a, y);
}
template <>
void caffe_sqr<double>(const int n, const double* a, double* y) {
  vdSqr(n, a, y);
}

template <>
void caffe_sqrt<float>(const int n, const float* a, float* y) {
  vsSqrt(n, a, y);
}
template <>
void caffe_sqrt<double>(const int n, const double* a, double* y) {
  vdSqrt(n, a, y);
}

template <>
void caffe_exp<float>(const int n, const float* a, float* y) {
  vsExp(n, a, y);
}
template <>
void caffe_exp<double>(const int n, const double* a, double* y) {
  vdExp(n, a, y);
}

template <>
void caffe_log<float>(const int n, const float* a, float* y) {
  vsLn(n, a, y);
}
template <>
void caffe_log<double>(const int n, const double* a, double* y) {
  vdLn(n, a, y);
}

template <>
void caffe_abs<float>(const int n, const float* a, float* y) {
    vsAbs(n, a, y);
}
template <>
void caffe_abs<double>(const int n, const double* a, double* y) {
    vdAbs(n, a, y);
}
```
11. 生成随机数 --> caffe_rng_rand()   
功能: 返回一个随机数.   
```cpp
unsigned int caffe_rng_rand() {
  return (*caffe_rng())();
}
```
12. 最接近的数值 ---> caffe_nextafter()    
功能: 返回 b 最大方向上可以表示的最接近的数值。
```cpp
template <typename Dtype>
Dtype caffe_nextafter(const Dtype b) {
  return boost::math::nextafter<Dtype>( b, 
                                        std::numeric_limits<Dtype>::max());
}

template
float caffe_nextafter(const float b);
template
double caffe_nextafter(const double b);
```
13. 生成符合均匀分布的数据 --> caffe_rng_uniform()   
功能: 返回符合均匀分布的数据。   
```cpp
template <typename Dtype>
void caffe_rng_uniform(const int n, const Dtype a, const Dtype b, Dtype* r) {
  CHECK_GE(n, 0);
  CHECK(r);
  CHECK_LE(a, b);
  boost::uniform_real<Dtype> random_distribution(a, caffe_nextafter<Dtype>(b));
  boost::variate_generator<caffe::rng_t*, boost::uniform_real<Dtype> >
      variate_generator(caffe_rng(), random_distribution);
  
  for (int i = 0; i < n; ++i) {
    r[i] = variate_generator();
  }
}

template
void caffe_rng_uniform<float>(const int n, const float a, const float b,
                              float* r);
template
void caffe_rng_uniform<double>(const int n, const double a, const double b,
                               double* r);
```
14. 返回符合高斯分布的数据 --> caffe_rng_gaussian()    
功能: 返回符合高斯分布(正态分布)的数据。   
```cpp
template <typename Dtype>
void caffe_rng_gaussian(const int n, const Dtype a,
                        const Dtype sigma, Dtype* r) {
  CHECK_GE(n, 0);
  CHECK(r);
  CHECK_GT(sigma, 0);
  boost::normal_distribution<Dtype> random_distribution(a, sigma);
  boost::variate_generator<caffe::rng_t*, boost::normal_distribution<Dtype> >
      variate_generator(caffe_rng(), random_distribution);
  for (int i = 0; i < n; ++i) {
    r[i] = variate_generator();
  }
}

template
void caffe_rng_gaussian<float>(const int n, const float mu,
                               const float sigma, float* r);
template
void caffe_rng_gaussian<double>(const int n, const double mu,
                                const double sigma, double* r);
```
15. 生成符合伯努利分布的数据  --> caffe_rng_bernoulli()   
功能: 返回符合伯努利分布的数据。  
伯努利分布是一个离散型几率分布，是二项分布的特殊情况.    
```cpp
template <typename Dtype>
void caffe_rng_bernoulli(const int n, const Dtype p, int* r) {
  CHECK_GE(n, 0);
  CHECK(r);
  CHECK_GE(p, 0);
  CHECK_LE(p, 1);
  boost::bernoulli_distribution<Dtype> random_distribution(p);
  boost::variate_generator<caffe::rng_t*, boost::bernoulli_distribution<Dtype> >
      variate_generator(caffe_rng(), random_distribution);
  for (int i = 0; i < n; ++i) {
    r[i] = variate_generator();
  }
}
template
void caffe_rng_bernoulli<double>(const int n, const double p, int* r);
template
void caffe_rng_bernoulli<float>(const int n, const float p, int* r);
template <typename Dtype>
void caffe_rng_bernoulli(const int n, const Dtype p, unsigned int* r) {
  CHECK_GE(n, 0);
  CHECK(r);
  CHECK_GE(p, 0);
  CHECK_LE(p, 1);
  boost::bernoulli_distribution<Dtype> random_distribution(p);
  boost::variate_generator<caffe::rng_t*, boost::bernoulli_distribution<Dtype> >
      variate_generator(caffe_rng(), random_distribution);
  for (int i = 0; i < n; ++i) {
    r[i] = static_cast<unsigned int>(variate_generator());
  }
}
template
void caffe_rng_bernoulli<double>(const int n, const double p, unsigned int* r);
template
void caffe_rng_bernoulli<float>(const int n, const float p, unsigned int* r);
```
16. 内积操作 -->caffe_cpu_dot()   
也称点积操作.   
功能: 返回 vector X 和 vector Y 的内积.   
incx, incy: 步长, 即每隔 incx 或 incy 个 element 进行操作.   
```cpp
template <>
float caffe_cpu_strided_dot<float>(const int n, const float* x, const int incx,
    const float* y, const int incy) {
  return cblas_sdot(n, x, incx, y, incy);
}
template <>
double caffe_cpu_strided_dot<double>(const int n, const double* x,
    const int incx, const double* y, const int incy) {
  return cblas_ddot(n, x, incx, y, incy);
}
template <typename Dtype>
Dtype caffe_cpu_dot(const int n, const Dtype* x, const Dtype* y) {
  return caffe_cpu_strided_dot(n, x, 1, y, 1);
}

template
float caffe_cpu_dot<float>(const int n, const float* x, const float* y);
template
double caffe_cpu_dot<double>(const int n, const double* x, const double* y);
```
17. 向量中元素绝对值之和 --> caffe_cpu_asum()     
功能: 计算 vector x 的所有 element 的绝对值之和.   
```cpp
template <>
float caffe_cpu_asum<float>(const int n, const float* x) {
  return cblas_sasum(n, x, 1);
}
template <>
double caffe_cpu_asum<double>(const int n, const double* x) {
  return cblas_dasum(n, x, 1);
}
```
18. 矩阵元素缩放拷贝  --> caffe_cpu_scale()     
```cpp
template <>
void caffe_cpu_scale<float>(const int n, const float alpha, const float *x,
                            float* y) {
  cblas_scopy(n, x, 1, y, 1);
  cblas_sscal(n, alpha, y, 1);
}
template <>
void caffe_cpu_scale<double>(const int n, const double alpha, const double *x,
                             double* y) {
  cblas_dcopy(n, x, 1, y, 1);
  cblas_dscal(n, alpha, y, 1);
}
}  // namespace caffe
```

## cuda 中一个很实用的设计技巧    
在 `common.hpp` 中有一个宏定义 `CUDA_KERNEL_LOOP`.   
它采取的线程格和线程块的维数设计, (blockDim.x * gridDim.x) 表示的是该线程格所有线程的数量, (n) 表示 `CUDA_KERNEL_LOOP` 函数总共要处理的元素个数.   
```cpp
#define CUDA_KERNEL_LOOP(i, n) \
  for (int i = blockIdx.x * blockDim.x + threadIdx.x; \
       i < (n); \
       i += blockDim.x * gridDim.x)
```
先看看 Caffe 采取的线程格和线程块的维数设计， 从 common.hpp 可以看到:     
```cpp
CAFFE_CUDA_NUM_THREADS
CAFFE_GET_BLOCKS(const int N)
```
明显都是一维的. 整理一下`CUDA_KERNEL_LOOP`格式看看:     
- `blockDim.x * gridDim.x`表示的是该线程格所有线程的数量;   
- `n`表示核函数总共要处理的元素个数;   
有时候，`n` 会大于 `blockDim.x * gridDim.x`，因此并不能一个线程处理一个元素. 由此通过上面的方法， 让一个线程串行(`for循环`)处理几个元素.   
再来看一下这个函数的使用:       
```cpp
template <typename Dtype>
__global__ void mul_kernel(const int n, const Dtype* a,
    const Dtype* b, Dtype* y) {
  CUDA_KERNEL_LOOP(index, n) {
    y[index] = a[index] * b[index];
  }
}
```
就是算两个向量的点积了. 向量的维数可能大于该`kernel函数`线程格的总线程数量.     
