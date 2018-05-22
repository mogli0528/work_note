cmake_minimum_required( VERSION 2.8 )
project( map_instance )

set( CMAKE_BUILD_TYPE "Release" )
set( CMAKE_CXX_FLAGS "-std=c++11 -O3" )
set( LINKER_LIBS "" )  

# glog  
# 这是从 caffe 中摘取的  
include("cmake/FindGlog.cmake")  
include_directories(SYSTEM ${GLOG_INCLUDE_DIRS})  
list(APPEND LINKER_LIBS ${GLOG_LIBRARIES})  
  
# gflags  
# 这是从caffe中获取的  
include("cmake/FindGFlags.cmake")  
include_directories(SYSTEM ${GFLAGS_INCLUDE_DIRS})  
list(APPEND LINKER_LIBS ${GFLAGS_LIBRARIES})  

message(STATUS "LINKER_LIBS: ${LINKER_LIBS}")

add_executable( map_instance map_instance.cpp sales_data.cpp sales_data.h)
target_link_libraries(map_instance ${LINKER_LIBS})  
