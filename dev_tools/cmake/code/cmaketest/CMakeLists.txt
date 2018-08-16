#设定最小版本号
CMAKE_MINIMUM_REQUIRED( VERSION 2.8 ) 

#project name  
PROJECT(cmake_test)  

# 设置编译器和编译选项
SET(CMAKE_C_COMPILER g++)
SET(CMAK_CXX_COMPILER g++)
# 允许c++11标准、O3优化、多线程。match选项可避免一些cpu上的问题
SET( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -march=native -O3 -pthread" )

# 设置用debug还是release模式。debug允许断点，而release更快
#SET( CMAKE_BUILD_TYPE Debug )
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
find_package(OpenCV REQUIRED)    
TARGET_LINK_LIBRARIES(bin/test ${LIBRARIES} ${OpenCV_LIBS})  
