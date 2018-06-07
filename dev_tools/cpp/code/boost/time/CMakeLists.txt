cmake_minimum_required(VERSION 2.8)
project( boost_time )
SET( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11" )

SET( CMAKE_BUILD_TYPE Release )
#SET( CMAKE_BUILD_TYPE Debug )

set(LINKER_LIBS "")  

# Add the source in project root directory
aux_source_directory(. DIRSRCS)

# user defined
include_directories(include)

# Target
message("source files: ${DIRSRCS}")

add_executable( boost_time ${DIRSRCS})
target_link_libraries( boost_time ${LINKER_LIBS} ) 