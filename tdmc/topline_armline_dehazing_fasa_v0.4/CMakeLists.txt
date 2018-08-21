cmake_minimum_required(VERSION 2.8)
project( topline )
SET( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11" )

SET( CMAKE_BUILD_TYPE Release )

set(LINKER_LIBS "")

# user defined
include_directories(include)
# opencv  
find_package(OpenCV REQUIRED)  
include_directories(${OpenCV_INCLUDE_DIRS})  
list(APPEND LINKER_LIBS ${OpenCV_LIBS})  
  
# glog  
# 这是从 caffe 中摘取的  
include("cmake/FindGlog.cmake")  
include_directories(SYSTEM ${GLOG_INCLUDE_DIRS})  
list(APPEND LINKER_LIBS ${GLOG_LIBRARIES})  

# Add the source in project root directory
aux_source_directory(src DIRSRCS)
aux_source_directory(src/detector DIRSRCS)
aux_source_directory(src/dehaze DIRSRCS)
aux_source_directory(src/fasa DIRSRCS)

# Target
message("source files:  ${DIRSRCS}")

add_executable( topline ${DIRSRCS})
target_link_libraries( topline ${LINKER_LIBS} -lcblas )
