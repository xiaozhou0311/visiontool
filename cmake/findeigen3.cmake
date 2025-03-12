set(CMAKE_PREFIX_PATH ${DIR_PROJECT_CURRENT}thirdparty/eigen3/share)
find_package(Eigen3 3.3 REQUIRED)
if (EIGEN3_FOUND)
    message(STATUS "Eigen3 found at: ${EIGEN3_INCLUDE_DIR}")
endif()