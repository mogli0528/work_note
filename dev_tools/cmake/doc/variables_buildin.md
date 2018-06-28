# cmake 编译系统的内置变量  

## cmake 系统内置的变量定义   
1. CMake PROJECT_BINARY_DIR 和 PROJECT_SOURCE_DIR 区别    
网上的文章都说 PROJECT_BINARY_DIR 和 PROJECT_SOURCE_DIR 是等价的。其实不然。   
因为一般来说，都是这样用:   
```bash
cmake ./
# 或
cmake   
```
这样 PROJECT_BINARY_DIR 和 PROJECT_SOURCE_DIR 是等价的, 即都是当前源码的目录。   
但是如果执行 cmake 的时候，并不在源码的路径的话，比如可以在src的同级目录下建立build目录, 然后在build目录下执行 cmake ../src :   
```
$ mkdir build
$ cd build
$ cmake ../src
```
这样的好处是 cmake 生成的文件和编译出来的东西就不放在源码路径下了,而是放在了 build/ 目录下，保证了源码路径的干净整洁。  
回到主题, 这时的 PROJECT_BINARY_DIR 和 PROJECT_SOURCE_DIR 就不同了,分别是:   
```
PROJECT_BINARY_DIR = 全路径/build
PROJECT_SOURCE_DIR = 全路径/src 
```
2. CMAKE_CURRENT_SOURCE_DIR   
当前正在处理的源码目录.   
The path to the source directory currently being processed.

This the full path to the source directory that is currently being processed by cmake.

## 指定编译结果的输出路径  

```cmake
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ./exercise/)
add_executable( hello_ext ${DIRSRCS})

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ../src/python)
add_library(hello_ext SHARED ${DIRSRCS})
```

## 编译动态库  

```cmake
add_library(hello_ext SHARED ${DIRSRCS})
```

使用 `set_target_properties(hello_ext PROPERTIES PREFIX "" OUTPUT_NAME "hello_ext")` 可以使编译出的动态库没有 `lib` 前缀 - `libhello_ext.so`;  