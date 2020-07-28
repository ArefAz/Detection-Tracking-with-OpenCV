cmake_minimum_required(VERSION 3.10)
project(tracking)

set(CMAKE_CXX_STANDARD 17)

find_package(OpenCV REQUIRED)

INCLUDE_DIRECTORIES(${OpenCV_INCLUDE_DIRS})
include_directories(include)
include_directories(lib)

add_executable(tracking main.cpp src/MyTrackers.cpp include/MyTrackers.h src/MyDetector.cpp include/MyDetector.h src/MyTracker.cpp include/MyTracker.h lib/cxxopts.hpp)

target_link_libraries(tracking ${OpenCV_LIBS})