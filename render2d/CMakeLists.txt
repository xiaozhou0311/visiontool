project(render2d)

# 在project()之后添加
if(MSVC)
    add_compile_options(/arch:AVX2)  # 改回AVX2
else()
    add_compile_options(-mavx2)      # GCC/Clang
endif()

# 收集头文件
file(GLOB_RECURSE GEOMETRY_FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/geometry/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/geometry/*.hpp"
)

# 收集源文件
file(GLOB_RECURSE SRC_FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cc"
)

file(GLOB_RECURSE INCLUDE_DIRECTORIES_FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/include/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp"
)

file(GLOB_RECURSE UTIL_FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/ui/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/ui/*.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/ui/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/ui/*.ui"

)

# 创建可执行文件
add_library(render2d SHARED  ${INCLUDE_DIRECTORIES_FILES} ${SRC_FILES} ${UTIL_FILES} ${GEOMETRY_FILES})

target_link_directories(render2d
    PRIVATE
)

# 链接库
target_link_libraries(render2d
    PRIVATE
    ${OpenCV_LIBS}
)

include_directories(
    ${CMAKE_SOURCE_DIR}/macro

    ${OpenCV_INCLUDE_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/visionalgo/include
    ${CMAKE_SOURCE_DIR}/thirdparty/MIPP-master/include
)
# 创建 Visual Studio 筛选器
source_group("include" FILES ${INCLUDE_DIRECTORIES_FILES})
source_group("src" FILES ${SRC_FILES})
source_group("ui" FILES ${UTIL_FILES}) 
source_group("geometry" FILES ${GEOMETRY_FILES})

# 设置输出目录
set_target_properties(render2d PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build/render2d
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build/render2d
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build/render2d
)