cmake_minimum_required(VERSION 2.8)
project( topline )
SET( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11" )
find_package( OpenCV REQUIRED )

# Add the source in project root directory
aux_source_directory(src DIRSRCS)
aux_source_directory(src/tools DIRSRCS)
aux_source_directory(. DIRSRCS)

# Add header file include directories
include_directories(include)

# Target
message("source files: ${DIRSRCS}")

add_executable( topline ${DIRSRCS})
target_link_libraries( topline ${OpenCV_LIBS} -lcblas ) 