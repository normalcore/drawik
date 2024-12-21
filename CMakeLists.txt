cmake_minimum_required(VERSION 3.1)
project(kesha)

set(CMAKE_CXX_STANDARD 17)
SET(GCC_COMPILE_FLAGS "-flto -fopenmp -lpthread -O3")

# Set up the module path to find SDL2
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

# Find SDL2 package
find_package(SDL2 REQUIRED)

# Include SDL2 headers
include_directories(${SDL2_INCLUDE_DIRS})

SET(CMAKE_CXX_FLAGS "${GCC_COMPILE_FLAGS}")

# Add the executable
add_executable(main main.cpp canvas.cpp tools.cpp)

# Link SDL2 libraries
target_link_libraries(main ${SDL2_LIBRARIES})
