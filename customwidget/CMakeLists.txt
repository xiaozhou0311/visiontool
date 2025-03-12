project(customwidget)

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

# 收集UI文件
file(GLOB_RECURSE UI_FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/ui/*.ui"
)

# 收集几何相关文件
file(GLOB_RECURSE GEOMETRY_FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/geometry/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/geometry/*.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/geometry/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/geometry/*.cc"
)

# 添加临时源文件
file(WRITE "${CMAKE_CURRENT_SOURCE_DIR}/src/dummy.cpp" "")

# 创建动态库
add_library(customwidget SHARED 
    ${INCLUDE_FILES} 
    ${SRC_FILES} 
    ${UI_FILES}
    ${GEOMETRY_FILES}
)

target_include_directories(customwidget
    PRIVATE
    ${CMAKE_SOURCE_DIR}/macro
    ${OpenCV_INCLUDE_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/geometry
    ${CMAKE_SOURCE_DIR}/visionalgo/include
    ${CMAKE_SOURCE_DIR}/thirdparty/MIPP-master/include
)

# 链接库
target_link_libraries(customwidget
    PRIVATE
    ${OpenCV_LIBS}
)

# 创建 Visual Studio 筛选器
source_group("Include" FILES ${INCLUDE_FILES})
source_group("Source" FILES ${SRC_FILES})
source_group("UI" FILES ${UI_FILES})
source_group("Geometry" FILES ${GEOMETRY_FILES})
