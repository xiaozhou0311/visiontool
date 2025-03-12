message( STATUS "------------------ General configuration for ${PROJECT_NAME} ${VERSION} -----------------")
message( STATUS )
message( STATUS "CMake Generator =       ${CMAKE_GENERATOR}")
message( STATUS "Compiler =              ${CMAKE_CXX_COMPILER_ID}")
message( STATUS "Compiler Version =      ${CMAKE_CXX_COMPILER_VERSION}")
# 输出找到的 Qt 版本信息
message(STATUS "Found Qt version = ${Qt5_VERSION}")
message(STATUS "Found Qt DIR = ${Qt5_DIR}")
message( STATUS "OPENCV_VERSION =  ${OPENCVVERSION}" )
message( STATUS "CUDA_VERSION =  ${CUDAVERSION}" )
message( STATUS "CMAKE_SOURCE_DIR =  ${CMAKE_SOURCE_DIR}" )
message( STATUS "PROJECT_BINARY_DIR =  ${PROJECT_BINARY_DIR}" )

get_directory_property( DirDefs DIRECTORY ${CMAKE_SOURCE_DIR} COMPILE_DEFINITIONS )
foreach( d ${DirDefs} )
    message( STATUS "Found Define: " ${d} )
endforeach()

