cmake_minimum_required (VERSION 2.6)
project (SFAV)

# Version Number    
set (LSim_VERSION_MAJOR 1)
set (LSim_VERSION_MINOR 0)

find_package (Threads)

find_package(PkgConfig REQUIRED)
pkg_check_modules(JSONCPP jsoncpp)
link_libraries(${JSONCPP_LIBRARIES})
#include_directories("${JSONCPP_INCLUDE_DIRS}")

# =================================================
# Add the directories containing the Header files
include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/include/mavlink/include/mavlink/v2.0)
include_directories(${CMAKE_SOURCE_DIR}/include/ptask/src)
include_directories(${CMAKE_SOURCE_DIR}/include/DynModel_grt_rtw)

# Directory to compile the Matlab generated code
#include_directories(/usr/local/MATLAB/R2016a/simulink/include)
#include_directories(/usr/local/MATLAB/R2016a/extern/include)
include_directories(${CMAKE_SOURCE_DIR}/include/simulink/include)
include_directories(${CMAKE_SOURCE_DIR}/include/extern/include)

get_property(inc_dirs DIRECTORY PROPERTY INCLUDE_DIRECTORIES)
message("Included Directories = \n${inc_dirs}\n\n")

# =================================================
# List the source files

# Basic components
file(GLOB_RECURSE src_files "src/*.cpp")
message("Found Basic Source Files = \n${src_files}\n\n")


# Matlab Generated Code
file(GLOB_RECURSE matlab_src_files "${CMAKE_SOURCE_DIR}/include/DynModel_grt_rtw/*.c")
message("Found Matlab Generated Source Files = \n${matlab_src_files}\n\n")

# Ptask Code
file(GLOB_RECURSE ptask_src "include/ptask/src/*.c")
message("Found Ptask Source Files = \n${ptask_src}\n\n")

# =================================================
# Creating Libraries
#set (Ptask libptask.a)
#link_directories(~/include/ptask/src)

add_library(MatlabLib STATIC ${matlab_src_files})
add_library(BasicLib STATIC ${src_files})
add_library(PtaskLib STATIC ${ptask_src})

# Add the executable

set(SOURCES main.cpp ${src_files})

#add_executable(SFAV main.cpp)
add_executable(SFAV ${SOURCES})

# =================================================
# Linking Libraries

# Link Basic Library
target_link_libraries(SFAV BasicLib)
# Link Ptask library
target_link_libraries(SFAV PtaskLib)
# Link the Matlab Library
target_link_libraries(SFAV MatlabLib)
# Link Pthread Libraries
target_link_libraries (SFAV ${CMAKE_THREAD_LIBS_INIT})
# Link the JsonCpp Library
target_link_libraries(SFAV ${JSONCPP_LIBRARIES})

#target_link_libraries(SFAV ExternLib)

