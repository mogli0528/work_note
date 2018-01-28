# ssd 中用到的 boost api  

```cpp
#include "boost/algorithm/string.hpp"
```

1. boost::split()
```cpp
boost::split(strings, FLAGS_gpu, boost::is_any_of(","));
```
字符串切分.对 FLAGS_gpu 字符串以 "," 进行切分, 最后切分得到的字符串存放在 strings 变量中.   
2. boost::lexical_cast()
模板类函数. 
```cpp  
boost::lexical_cast<int>(strings[i]));
```
一个轻量级的类型转换.将 strings[i] 中的字符转换为 int 类型.   
```cpp
boost::lexical_cast<string>(solver_param.device_id()); 
boost::lexical_cast<string>(0)
```
将数字转换为字符串.    
3. boost::thread_specific_ptr<xxx>   
```cpp
boost::thread_specific_ptr<Caffe> thread_instance_;   
```


