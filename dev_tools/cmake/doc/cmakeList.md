# `CMakeList.txt`语法详解
`CMake`是一种跨平台编译工具，比`make`更为高级，使用起来要方便得多。`CMake`主要是编写`CMakeLists.txt`文件，然后用`cmake`命令将`CMakeLists.txt`文件转化为 `make` 所需要的  `Makefile` 文件，最后用 `make` 命令编译源码生成可执行程序或共享库。因此 `CMake` 的编译基本就两个步骤： `cmake && make `。   
`cmake` 指向 `CMakeLists.txt` 所在的目录，例如 `cmake ..` 表示 `CMakeLists.txt` 在当前目录的上一级目录。`cmake` 后会生成很多编译的中间文件以及 `Makefile` 文件，所以一般建议新建一个新的目录，专门用来编译，例如：   
```
$ mkdir build
$ cd build
$ cmake ..
$ make
```
make 根据生成 makefile 文件，编译程序。   

## 一个`CMakeList.txt`实例
```cmake
# 这是一个测试函数的程序的CMakeLists.txt，"#"后面为注释的内容，CMake的命令全部为大写

#设定最小版本号
CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

# 指定生成的工程名为 ”cmake_test“
PROJECT(cmake_test)  

# 设置编译器和编译选项
SET(CMAKE_C_COMPILER g++)
SET(CMAK_CXX_COMPILER g++)
# 允许c++11标准、O3优化、多线程。match选项可避免一些cpu上的问题
SET( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -march=native -O3 -pthread" )

# 设置用debug还是release模式。debug允许断点，而release更快
#SET( CMAKE_BUILD_TYPE Debug )
SET( CMAKE_BUILD_TYPE Release )

# 指定头文件目录为 “include”

INCLUDE_DIRECTORIES(  
include  
)  

# 指定源文件目录为 "src"，并将其赋值给环境变量 "DIR_SRCS"
AUX_SOURCE_DIRECTORY(src DIR_SRCS)  

# 设定环境变量 "TEST_CMAKE" 的值为环境变量 "DIR_SRCS" 的值
# 此处并没有什么特别的作用，只是用于显示如何用环境变量对环境变量进行赋值
SET(TEST_CMAKE 
${DIR_SRCS}  
) 

# 将外部链接库赋值给环境变量 "LIBRARIES" ，(当然也可以不用这个环境变量，而在后面直接使用该库名)
SET(LIBRARIES  
#libm.so  
)  

# 指定生成文件，将环境变量 "TEST_CMAKE" 目录下的所有文件编译生成 "bin" 目录下的可执行文件 "test" 
# 但是 "bin" 目录应该是存在的
ADD_EXECUTABLE(bin/test ${TEST_CMAKE})  

# 指定 “bin/test" 执行时的链接库为环境变量 "LIBRARIES" 的值 "libm.so"
TARGET_LINK_LIBRARIES(bin/test ${LIBRARIES})  
```
以下是不带注释的`CMakeList.txt`文件： 

```cmake
CMAKE_MINIMUM_REQUIRED( VERSION 2.8 ) 

#project name  
PROJECT(cmake_test)  

SET(CMAKE_C_COMPILER g++)
SET(CMAK_CXX_COMPILER g++)
SET( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -march=native -O3 -pthread" )

SET( CMAKE_BUILD_TYPE Release )

#head file path  
INCLUDE_DIRECTORIES(  
include  
)  

#source directory  
AUX_SOURCE_DIRECTORY(src DIR_SRCS)  

#set environment variable  
SET(TEST_CMAKE 
${DIR_SRCS}  
)  
#set extern libraries  
SET(LIBRARIES  
#libm.so  
)  

link_directories(  
/usr/local/cuda/lib64
) 

# add executable file  
ADD_EXECUTABLE(bin/test ${TEST_CMAKE})  

#add link library  
TARGET_LINK_LIBRARIES(bin/test ${LIBRARIES})  

```    


## `cmake c++11`          
`CMakeLists.txt`中添加      
```
include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
if(COMPILER_SUPPORTS_CXX11)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
elseif(COMPILER_SUPPORTS_CXX0X)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
else()
        message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
endif()
```


CMakeListserv.txt 的写法

1. 要求 CMake 根据指定的源文件生成可执行文件   
```cmake
# Demo1:
add_executable(hello main.cpp)
```
这将从 main.cpp 源码文件创建一个叫 “hello” (Windows下叫“hello.exe”)的可执行文件。你可以根据自己的需要将 C 和 C++ 文件混合。在同一个 CMakeLists.txt 可以有多个可执行文件和库。同一个源码文件可以用于不同的目的，源码可以从其他目标中为每个目的独立的编译。   
```cmake
# Demo2:
add_executable(demo main.cpp main.h main.rc)
```
指示程序将使用 main.cpp 源文件, main.h 文件, main.rc 文件构造可执行文件。至于如何使用这些文件, CMake 比我们都清楚。   
2. 调试 CMakeLists.txt 的办法   
这个是调试 CMakeLists.txt 的一个手段啦。不得不学习哦。   
```cmake
# Demo 1:
MESSAGE("俺们正在生成项目文件")
```
会显示一个警告框。   
```cmake
# Demo 2:
MESSAGE(STATUS "俺们正在创建项目文件")
```
遇到这条指令,会把文字显示在状态栏里面(一闪而过，不容易发现)。   
```cmake
# Demo3:
MESSAGE(FATAL_ERROR "严重错误，俺们搞不定啦")
```
这条指令会提示出错，并退出。   
3. 使用标准模块   
cmake 提供了很多标准模块，扩展名都是 txt. 我们可以直接包含进来。就像使用 C 语言的 #include 指令一般。比如:    
```cmake
INCLUDE(FindBoost)
```
一句话,就告诉了 CMake “我们的程序需要 Boost ”。   
4. 使用变量   
```cmake
SET( MY_SOURCES main.cpp widget.cpp)
MESSAGE(STATUS "my sources: ${MY_SOURCES}")
```
使用 SET() 命令来为变量设置值。如果你列出了一个以上的字符串，变量将是串列表。列表是一列由分号隔开的字符串。如果只设置个一项，那么这项只有一个值。可以通过 ${VAR} 获得变量的值。可以使用 FOREACH() 来迭代一份列表：    
```cmake
FOREACH(next_ITEM ${MY_SOURCES})
   MESSAGE(STATUS "next item: ${next_ITEM}")
ENDFOREACH(next_ITEM ${MY_SOURCES})
```
CMake中的`命令`是大小写无关的。`变量名和参数名`是大小写相关的。   
5. 测试平台相关信息    
```cmake
# Demo1: (这个代码没有检验过哦) 
IF (UNIX)
   MESSAGE("这个是UNIX操作系统")
ENDIF (UNIX)
IF (MSVC)
   MESSAGE("这个需要 VC 的项目文件")
ENDIF (MSVC)
```
```cmake
# Demo 2: (这个测试过)
IF (${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
 SET(option WIN32)
 SET(win32_LIBRARIES comctl32.lib shlwapi.lib shell32.lib odbc32.lib odbccp32.lib  kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib)
 #SET(defs -DUNICODE -D_UNICODE)
ENDIF (${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
```
6. 要求 CMake 根据指定的源文件生成库文件   
ADD_LIBRARY: Add a library to the project using the specified source files.   
```cmake
ADD_LIBRARY(libname [SHARED | STATIC | MODULE] [EXCLUDE_FROM_ALL]    source1 source2 ... sourceN)   
Adds a library target. 
SHARED, STATIC or MODULE 关键字用来设置库文件的类型. 如果是 MODULE , the library type is set to MH_BUNDLE on systems which use dyld. On systems without dyld, MODULE is treated like SHARED. If no keywords appear as the second argument, the type defaults to the current value of BUILD_SHARED_LIBS. If this variable is not set, the type defaults to STATIC.
```
If EXCLUDE_FROM_ALL is given the target will not be built by default. It will be built only if the user explicitly builds the target or another target that requires the target depends on it.   
7. 添加查找头文件的路径   
INCLUDE_DIRECTORIES: 添加头文件的包含路径.  
```cmake
INCLUDE_DIRECTORIES([AFTER|BEFORE] [SYSTEM] dir1 dir2 ...)   
将给定的目录添加到 compiler 的头文件搜索路径.   
使用 BEFORE or AFTER 选项可以选择在当前目录列表之前或是之后添加, 如果没有使用,则默认情况是在目录列表之后追加.可以通过设置 CMAKE_INCLUDE_DIRECTORIES_BEFORE 为 ON 来改变这种默认行为.   
使用 SYSTEM 选项可以指定包含的目录是系统目录.    
```
8. 添加库文件的搜索路径   
LINK_DIRECTORIES: 指定编译器的 libraries 文件搜索路径.   
```cmake
LINK_DIRECTORIES(directory1 directory2 ...)   
```
9. 显式指定链接时需要的库文件   
为每个目标分别指定需要链接的库文件(指定部分目标专用的库文件)    
TARGET_LINK_LIBRARIES: Link a target to given libraries.
```cmake
TARGET_LINK_LIBRARIES(target library1   <debug | optimized> library2  ...)
将一系列指定的库文件链接到 target 文件中.   
debug and optimized 选项用来指示下一个列出的库文件将会以何种类型 build.   
```
10. 为所有目标统一指定需要的库文件(指定所有目标都用的库文件)   
LINK_LIBRARIES: Link libraries to all targets added later.    
为接下来的 targets 文件指定一系列链接文件(通常使用 ADD_EXECUTABLE or ADD_LIBRARY 命令指定targets).这个命令会给所有的子目录传递下去.   
```cpp
LINK_LIBRARIES(library1 <debug | optimized> library2 ...)
这是一个用于指定链接库的 old CMake 命令. 除非你必须为每个 target 都指定相同的链接库, 否则就使用 TARGET_LINK_LIBRARIES .
```
11. 显式实施宏定义   
ADD_DEFINITIONS: Adds -D define flags to the command line of C and C++ compilers.   
```cmake
ADD_DEFINITIONS(-DFOO -DBAR ...)
```
为  C/C++ 编译器添加 flags, 因为通常大多数 C/C++ 编译器都支持 -D flag .   
```cmake
# demo1 (文本宏):
ADD_DEFINITIONS(-DDEBUG) 
# demo2 (常量宏):
ADD_DEFINITIONS(-DVERSION=1) 
```
