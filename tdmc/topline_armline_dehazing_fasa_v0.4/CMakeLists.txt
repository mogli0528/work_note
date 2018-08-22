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
aux_source_directory(src SRC_FILES)
aux_source_directory(src/detector SRC_FILES)
aux_source_directory(src/dehaze SRC_FILES)
aux_source_directory(src/fasa SRC_FILES)

# Target
message("Source Files: " ${SRC_FILES} )
message("Header Files: " ${OpenCV_INCLUDE_DIRS} )
message("Lib Files: " ${LINKER_LIBS} )

add_executable( topline ${SRC_FILES})
target_link_libraries( topline ${LINKER_LIBS} -lcblas )
