# 算法运行时间统计   

> caffe/include/caffe/util/benchmark.hpp   

caffe 提供了每个 layer 前向运算和反向传播所需的时间统计类, 即 Timer 类.   

```cpp
#include <boost/date_time/posix_time/posix_time.hpp>

boost::posix_time::ptime start_cpu_;
boost::posix_time::ptime stop_cpu_;

// 获取当前时间
start_cpu_ = boost::posix_time::microsec_clock::local_time();
stop_cpu_ = boost::posix_time::microsec_clock::local_time();

// 返回毫秒数
float elapsed_milliseconds_ = (stop_cpu_ - start_cpu_).total_milliseconds();

// 返回微秒数
float elapsed_microseconds_ = (stop_cpu_ - start_cpu_).total_microseconds();
```