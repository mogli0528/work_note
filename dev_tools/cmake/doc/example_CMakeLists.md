# 一个`CMakeList.txt`实例
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
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -march=native -O3 -pthread" )

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

#add executable file  
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

## 实例合并   
```cmake
cmake_minimum_required(VERSION 3.4.1)

### libgameplay
#头文件路径
include_directories( 
src_gameplay/src
src_gameplay/external-deps/include
)

link_directories(
src_gameplay/external-deps/lib
)

file(GLOB gameplay_src "src_gameplay/src/*.cpp") #声明字符串变量，此处字符串为源文件路径
file(GLOB gameplay_lua "src_gameplay/src/lua/*.cpp")
file(GLOB homura_src "src_gameplay/homura/*.cpp")

add_library( # Sets the name of the library.
             gameplay
             # Sets the library as a shared library.
             SHARED
             # Provides a relative path to your source file(s).
             # Associated headers in the same location as their source
             # file are automatically included.
             ${gameplay_src}
             ${gameplay_lua}
             ${homura_src} )

set_target_properties( gameplay PROPERTIES IMPORTED_LOCATION libs/${ANDROID_ABI}/libgameplay.so )
set(CMAKE_CXX_FLAGS "-Os -std=c++11 -frtti -Wno-switch-enum -Wno-switch -Wno-error=non-virtual-dtor -D__ANDROID__")

target_link_libraries( # Specifies the target library.
                       gameplay
                       # Denpendences
                       gameplay-deps
                       log
                       android
                       EGL
                       GLESv2
                       OpenSLES )
### libgameplay_jni
file(GLOB gameplay_jni "jni/game_jni/*.cpp")
add_library( # Sets the name of the library.
             gameplay_jni
             # Sets the library as a shared library.
             SHARED
             # Provides a relative path to your source file(s).
             # Associated headers in the same location as their source
             # file are automatically included.
             ${gameplay_jni} )

set_target_properties(gameplay_jni PROPERTIES IMPORTED_LOCATION libs/${ANDROID_ABI}/libgameplay_jni.so )
set(CMAKE_CXX_FLAGS "-Os -std=c++11 -frtti -Wno-switch-enum -Wno-switch -Wno-error=non-virtual-dtor -D__ANDROID__")

target_link_libraries( # Specifies the target library.
                       gameplay_jni
                       # Denpendences
                       gameplay
                       log
                       android )
```