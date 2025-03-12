project(algodemo)

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
    ${CMAKE_SOURCE_DIR}/thirdparty/MIPP-master/include

)

# 在链接库之前添加库目标定义
add_library(algodemo SHARED ${SRC_FILES} ${INCLUDE_FILES})

# 链接库
target_link_libraries(algodemo
    PRIVATE
    ${OpenCV_LIBS}
)

# 创建 Visual Studio 筛选器
source_group("Include" FILES ${INCLUDE_FILES})
source_group("Source" FILES ${SRC_FILES})

# 设置输出目录
set_target_properties(algodemo PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build/unittest
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build/unittest
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build/unittest
)

