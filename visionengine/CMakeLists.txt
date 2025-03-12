project(visionengine)

# 在project()之后添加
if(MSVC)
    add_compile_options(/arch:AVX2)  # 改回AVX2
else()
    add_compile_options(-mavx2)      # GCC/Clang
endif()

# 收集所有子目录的头文件
file(GLOB_RECURSE INCLUDE_FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/operator/include/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/operator/include/*.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/algoparamsparse/include/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/algoparamsparse/include/*.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/algoload/include/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/algoload/include/*.hpp"
)

# 收集所有子目录的源文件
file(GLOB_RECURSE SRC_FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/operator/src/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/operator/src/*.cc"
    "${CMAKE_CURRENT_SOURCE_DIR}/algoparamsparse/src/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/algoparamsparse/src/*.cc"
    "${CMAKE_CURRENT_SOURCE_DIR}/algoload/src/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/algoload/src/*.cc"
)

# 收集UI文件
file(GLOB_RECURSE UI_FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/ui/*.ui"
)

# 添加一个临时的源文件，确保项目可以编译
add_library(visionengine SHARED  ${INCLUDE_FILES} ${SRC_FILES} ${UI_FILES})


# 添加包含目录
target_include_directories(visionengine
    PRIVATE
    ${CMAKE_SOURCE_DIR}/macro
    ${OpenCV_INCLUDE_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR}/operator/include
    ${CMAKE_CURRENT_SOURCE_DIR}/algoparamsparse/include
    ${CMAKE_CURRENT_SOURCE_DIR}/algoload/include
    ${CMAKE_SOURCE_DIR}/visionalgo/include
    ${CMAKE_SOURCE_DIR}/thirdparty/MIPP-master/include
)

# 链接库
target_link_libraries(visionengine
    PRIVATE
    ${OpenCV_LIBS}
)

# 创建 Visual Studio 筛选器
source_group("include" FILES ${INCLUDE_DIRECTORIES_FILES})
source_group("src" FILES ${SRC_FILES})
source_group("ui" FILES ${UTIL_FILES}) 
source_group("geometry" FILES ${GEOMETRY_FILES})
