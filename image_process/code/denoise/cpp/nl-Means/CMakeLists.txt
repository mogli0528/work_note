cmake_minimum_required(VERSION 2.8)
project( nlmeans )
SET( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2  -std=c++11 -L/usr/X11R6/lib " )

#SET( CMAKE_BUILD_TYPE Release )
SET( CMAKE_BUILD_TYPE Debug )

set(LINKER_LIBS "")  
list(APPEND LINKER_LIBS -lpthread -lX11 -lpng -lm)

set(NL_MEANS_SRCS src/nlmeans.cpp src/utils.cpp)  
set(NL_MORPHO_SRCS src/nl_morpho.cpp src/utils.cpp)  

# Add the source in project root directory
#aux_source_directory(src NL_MEANS_SRCS)
#aux_source_directory(src/tools DIRSRCS)
#aux_source_directory(. NL_MEANS_SRCS)

# user defined
include_directories(include)

# Target
message("source files: ${NL_MEANS_SRCS}")
message("LINKER_LIBS: ${LINKER_LIBS}")

add_executable( nlmeans ${NL_MEANS_SRCS})
target_link_libraries( nlmeans ${LINKER_LIBS} -lpthread -lX11 -lpng -lm) 
add_executable( morpho ${NL_MORPHO_SRCS})
target_link_libraries( morpho ${LINKER_LIBS} -lpthread -lX11 -lpng -lm) 