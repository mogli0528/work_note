# gflag 
gflags 是 google 开源的用于处理命令行参数的项目。   

## 1. gflag 支持的参数类型    
gflags 一共支持 5 种类型的命令行参数定义：   
```
DEFINE_bool: 布尔类型
DEFINE_int32: 32 位整数
DEFINE_int64: 64 位整数
DEFINE_uint64: 无符号 64 位整数
DEFINE_double: 浮点类型 double
DEFINE_string: C++ string 类型
```
Note:   
(1). 对于 bool 参数，在命令行可以不指定值也可以指定值.   
假如我们定义了一个 bool 参数 debug_switch，可以在命令行这样指定：   
```
$ ./gflags_test -debug_switch  # 这样就是 true
$ ./gflags_test -debug_switch=true # 这样也是 true
$ ./gflags_test -debug_switch=1 # 这样也是 true
$ ./gflags_test -debug_switch=false # 0 也是 false
```

## 2. gflags 变量的访问   
所有我们定义的 gflags 变量都可以通过 `FLAGS_` 前缀加参数名访问，gflags 变量也可以被自由修改：   
```cpp
if (FLAGS_consider_made_up_languages)
    FLAGS_languages += ",klingon";
if (FLAGS_languages.find("finish") != string::npos)
    HandleFinnish();
```

## 3. 定义规范   
如果你想要访问在另一个文件定义的 gflags 变量呢？使用 `DECLARE_`，它的作用就相当于用 `extern` 声明变量。   
为了方便的管理变量，我们推荐在 .cc 或者 .cpp 文件中 `DEFINE_` 变量，然后只在对应 .h 中或者单元测试中 `DECLARE_` 变量。   
比如可以在 foo.cc 定义了一个 gflags 变量 `DEFINE_string(name, 'bob', '')`，假如你需要在其他文件中使用该变量， 那么在 `foo.h` 中声明 `DECLARE_string(name)`，然后在使用该变量的文件中 `include "foo.h"` 就可以(当然，这只是为了更好地管理文件关联，如果你不想遵循也是可以的).   

## 4. 参数检查   
如果你定义的 gflags 参数很重要，希望检查其值是否有效，那么就可以定义并注册(用 gflags::RegisterFlagValidator() 函数)一个检查函数。   
采用 static 全局变量来确保检查函数会在 main 开始时被注册，可以保证注册会在 ParseCommandLineFlags 函数之前。如果默认值检查失败，那么 ParseCommandLineFlags 将会使程序退出。如果之后使用 SetCommandLineOption() 来改变参数的值，那么检查函数也会被调用，但是如果验证失败，只会返回 false，然后参数保持原来的值，程序不会结束。   
看下面的程序示例：    
```
#include <stdint.h>
#include <stdio.h>
#include <iostream>

#include <gflags/gflags.h>

// 定义对 FLAGS_port 的检查函数
static bool ValidatePort(const char* name, int32_t value) {
    if (value > 0 && value < 32768) {
        return true;
    }
    printf("Invalid value for --%s: %d\n", name, (int)value);
    return false;
}

/**
 *  设置命令行参数变量
 *  默认的主机地址为 127.0.0.1，变量解释为 'the server host'
 *  默认的端口为 12306，变量解释为 'the server port'
 */
DEFINE_string(host, "127.0.0.1", "the server host");
DEFINE_int32(port, 12306, "the server port");

// 使用全局 static 变量来注册函数，static 变量会在 main 函数开始时就调用
static const bool port_dummy = gflags::RegisterFlagValidator(&FLAGS_port, &ValidatePort);

int main(int argc, char** argv) {
    // 解析命令行参数，一般都放在 main 函数中开始位置
    gflags::ParseCommandLineFlags(&argc, &argv, true);
    std::cout << "The server host is: " << FLAGS_host
        << ", the server port is: " << FLAGS_port << std::endl;

    // 使用 SetCommandLineOption 函数对参数进行设置才会调用检查函数
    gflags::SetCommandLineOption("port", "-2");
    std::cout << "The server host is: " << FLAGS_host
        << ", the server port is: " << FLAGS_port << std::endl;
    return 0;
}
```
让我们运行一下程序，看看怎么样：   
```
#命令行指定非法值，程序解析参数时直接退出
$ ./gflags_test -port -2 
Invalid value for --port: -2
ERROR: failed validation of new value '-2' for flag 'port'
# 这里参数默认值合法，但是 SetCommandLineOption 指定的值不合法，程序不退出，参数保持原来的值 
$ ./gflags_test        
The server host is: 127.0.0.1, the server port is: 12306
Invalid value for --port: -2
The server host is: 127.0.0.1, the server port is: 12306
```

## 5. 使用 flagfile   
如果我们定义了很多参数， 那么每次启动时都在命令行指定对应的参数显然是不合理的。gflags 库已经很好的解决了这个问题。你可以把 flag 参数和对应的值写在文件中，然后运行时使用 -flagfile 来指定对应的 flag 文件就好。文件中的参数定义格式与通过命令行指定是一样的。    
例如，我们可以定义这样一个文件，文件后缀名没有关系，为了方便管理可以使用后缀为 .flags 的文件：
```
--host=10.123.14.11
--port=23333 
```
然后命令行指定：
```
$ ./gflags_test --flagfile server.flags 
The server host is: 10.123.14.11, the server port is: 23333
```
棒！以后再也不用担心参数太多了.   

## 6. gflag 和 glog 的使用步骤   
6.1 头文件包含;  
```cpp
#include <gflags/gflags.h>
#include <glog/logging.h>
```
6.2 更改命名空间;   
```cpp
namespace gflags = google;
```
6.3 定义命令行参数;  
```cpp
DEFINE_string(gpus, "", 
	"Optional; run in GPU mode on given device IDs separated by ','."
    "Use '-gpu all' to run on all available GPUs. The effective training "
    "batch size is multiplied by the number of devices.");
DEFINE_int32(iterations, 50, 
	"The number of iterations to run.");
```
6.4 gflags 和 glog 系统的初始化   
```cpp
// Google flags.
::gflags::ParseCommandLineFlags(pargc, pargv, true);
// Google logging.
::google::InitGoogleLogging(*(pargv)[0]);
// Provide a backtrace on segfault.
::google::InstallFailureSignalHandler();
}
```
6.5 将输出也定向到 stderr( 在输出 logging 的同时 )   
```cpp 
FLAGS_alsologtostderr = 1;
```
6.6 显示参数的使用方法   
```cpp
gflags::SetUsageMessage("command line parameters. \n"
      "usage: caffe <command> <args>\n\n"
      "commands:\n"
      "  train           train or finetune a model\n"
      "  test            score a model\n"
      "  device_query    show GPU diagnostic information\n"
      "  time            benchmark model execution time");
```
6.7 显示命令行参数的使用方法   
```cpp
gflags::ShowUsageWithFlagsRestrict(argv[0], "tools/caffe");
```

## 7. glog 的日志等级    
7.1 LOG(INFO)   
普通打印消息.   
7.2 LOG(ERROR)   
打印错误消息.  
7.3 LOG(FATAL)   
程序报错终止.   
7.4 LOG(WARNING)   
打印警告消息.   
