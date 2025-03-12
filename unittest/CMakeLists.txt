project(unittest)

# 在project()之后添加
if(MSVC)
    add_compile_options(/arch:AVX2)  # 改回AVX2
else()
    add_compile_options(-mavx2)      # GCC/Clang
endif()

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


# 创建可执行文件
add_executable(unittest ${INCLUDE_FILES} ${SRC_FILES})

target_link_directories(unittest
    PRIVATE
)

# 链接库
target_link_libraries(unittest
    PRIVATE
    visionalgo
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
source_group("Include" FILES ${INCLUDE_FILES})
source_group("Source" FILES ${SRC_FILES})
