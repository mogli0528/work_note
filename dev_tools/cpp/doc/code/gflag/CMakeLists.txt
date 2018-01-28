cmake_minimum_required(VERSION 2.8)  
project(Disp)  
  
set(LINKER_LIBS "")  
  
# opencv  
find_package(OpenCV REQUIRED)  
include_directories(${OpenCV_INCLUDE_DIRS})  
list(APPEND LINKER_LIBS ${OpenCV_LIBS})  
  
# glog  
# 这是从caffe中获取的  
include("cmake/Modules/FindGlog.cmake")  
#find_package(lib_glog REQUIRED)  
include_directories(SYSTEM ${GLOG_INCLUDE_DIRS})  
list(APPEND LINKER_LIBS ${GLOG_LIBRARIES})  
  
# gflags  
# 这是从caffe中获取的  
include("cmake/Modules/FindGFlags.cmake")  
#find_package(GFlags REQUIRED)  
include_directories(SYSTEM ${GFLAGS_INCLUDE_DIRS})  
list(APPEND LINKER_LIBS ${GFLAGS_LIBRARIES})  
  
# 指定子目录下包含 CMakeLists.txt 的路径
# ---[ Subdirectories
# add_subdirectory(src/)

# 指定源码搜索路径
# 指定源文件目录为 "src"，并将其赋值给环境变量 "DIR_SRCS"
AUX_SOURCE_DIRECTORY(src, DIR_SRCS)
SET(DIR_SRCS
src/gflag.cpp
)
message(STATUS "source dirs: ${DIR_SRCS}")
message(STATUS "source dirs${MY_SOURCES}")

# 加入你自己编译成可执行文件的名称以及对应的代码 
add_executable(gflags_test ${DIR_SRCS})  
#add_executable(gflags_test src/gflag.cpp)  
# 链接时候的库文件  
target_link_libraries(gflags_test ${LINKER_LIBS})  