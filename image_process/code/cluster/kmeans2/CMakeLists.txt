cmake_minimum_required(VERSION 2.8)

PROJECT(kmeans)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set( LINKER_LIBS "")

include_directories("include")


# OpenCV
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})
list(APPEND LINKER_LIBS ${OpenCV_LIBS})

#boost filesystem
find_package(Boost REQUIRED COMPONENTS
system
filesystem
)
list(APPEND LINKER_LIBS ${Boost_LIBRARIES})

aux_source_directory(. DIR_SRCS)

add_executable(kmeans ${DIR_SRCS})
target_link_libraries(kmeans ${LINKER_LIBS})
