project(visionalgo)

# 宏定义
add_definitions(-VISION_ALGO_EXPORTS)

# 收集头文件
file(GLOB_RECURSE INCLUDE_FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/include/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp"
)

# 收集源文件
file(GLOB_RECURSE SRC_FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cc"
)

# 调试信息
message("SRC_FILES = ${SRC_FILES}")
message("INCLUDE_FILES = ${INCLUDE_FILES}")

# 包含目录
include_directories(
    ${OpenCV_INCLUDE_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/macro
)

# 在链接库之前添加库目标定义
add_library(visionalgo SHARED ${SRC_FILES} ${INCLUDE_FILES})

# 链接库
    target_link_libraries(visionalgo
    PRIVATE
    ${OpenCV_LIBS}
)

# 创建 Visual Studio 筛选器
source_group("include" FILES ${INCLUDE_FILES})
source_group("src" FILES ${SRC_FILES})

# 设置输出目录
set_target_properties(visionalgo PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build/unittest
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build/unittest
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build/unittest
)

