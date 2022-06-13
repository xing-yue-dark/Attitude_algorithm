# 版本限制
cmake_minimum_required(VERSION 2.8)

project(Demo)


# 设置指定的C++编译器版本是必须的，如果不设置，或者为OFF，则指定版本不可用时，会使用上一版>本。
set(CMAKE_CXX_STANDARD_REQUIRED ON)
# 指定为C++11 版本
set(CMAKE_CXX_STANDARD 17)

# 添加opencv库
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})


# 添加自定义类头文件
include_directories(.)
add_library(MY_PNP pnp_algorithm.h pnp_algorithm.cpp)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR})
add_executable(Demo main.cpp)

target_link_libraries(Demo ${OpenCV_LIBS})
target_link_libraries(Demo MY_PNP)





