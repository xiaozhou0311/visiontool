cmake_minimum_required(VERSION 3.28)

project(visiontool)

# 设置构建类型
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release")
endif()

# 设置c++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 编译器标志设置
if(MSVC)
    add_compile_options(/W4 /MP /openmp)
    set_property(GLOBAL PROPERTY USE_FOLDERS ON)
else()
    add_compile_options(-Wall -Wno-sign-compare -fopenmp)
endif()

add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")

# 平台相关的编译标志
if(${CMAKE_SYSTEM_PROCESSOR} MATCHES "arm")
    set(PLATFORM_COMPILE_FLAGS "-mfpu=neon")
elseif(NOT MSVC)
    set(PLATFORM_COMPILE_FLAGS "-march=native")
endif()

# 设置release和debug都为控制台
if(CMAKE_BUILD_TYPE STREQUAL "Release")
    set(CMAKE_BUILD_TYPE "Release") 
else()
    set(CMAKE_BUILD_TYPE "Debug")
endif()

# 查找依赖
include(cmake/findopencv.cmake) 
include(cmake/findqt.cmake)

# 添加子目录
add_subdirectory(visionalgo)
add_subdirectory(unittest)
add_subdirectory(apidemotest)
add_subdirectory(render2d)
add_subdirectory(visionengine)
add_subdirectory(customwidget)

