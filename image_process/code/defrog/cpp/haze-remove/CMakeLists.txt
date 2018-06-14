cmake_minimum_required(VERSION 2.8)
project( haze-remove )
SET( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2  -std=c++11" )

SET( CMAKE_BUILD_TYPE Release )
# SET( CMAKE_BUILD_TYPE Debug )

set(LINKER_LIBS "")  
find_package( OpenCV REQUIRED )
list(APPEND LINKER_LIBS ${OpenCV_LIBS})

# set(NL_MEANS_SRCS src/haze-remove.cpp src/utils.cpp)  
# set(NL_MORPHO_SRCS src/nl_morpho.cpp src/utils.cpp)  

# Add the source in project root directory
aux_source_directory(src NL_MEANS_SRCS)
#aux_source_directory(src/tools DIRSRCS)
#aux_source_directory(. NL_MEANS_SRCS)

# user defined
include_directories(include)

# Target
message("source files: ${NL_MEANS_SRCS}")
message("LINKER_LIBS: ${LINKER_LIBS}")

add_executable( haze-remove ${NL_MEANS_SRCS})
target_link_libraries( haze-remove ${LINKER_LIBS}) 