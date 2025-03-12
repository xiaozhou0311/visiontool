IF(OPENCV_DIR)
SET(OPENCV_FIND_QUIETLY TRUE)
ENDIF(OPENCV_DIR)

if(WIN32)
find_path(OPENCV_DIR opencvfind.txt
        $ENV{PROGRAMFILES} 
        $ENV{SYSTEMDRIVE}
)
endif(WIN32)

if(OPENCV_DIR)
    set(OPENCV_FOUND TRUE)
endif(OPENCV_DIR)

if(OPENCV_FOUND)
    message(STATUS "Found OpenCV: ${OPENCV_DIR}")

    # 设置lib,这样之后只需要导入OPENCV_LIBS,不需要按照模式配置三遍
    set(OPENCV_INCLUDE_DIR ${OPENCV_DIR}/include)
    set(OpenCV_INCLUDE_DIRS ${OPENCV_DIR}/include)
    include_directories(${OpenCV_INCLUDE_DIR})
    set(OPENCV_LIB_DIR ${OPENCV_DIR}/lib/)
    set(OPENCV_DEBUG_DIR ${OPENCV_DIR}/lib/debug/)
    set(OPENCV_RELEASE_DIR ${OPENCV_DIR}/lib/release/)
    
    add_library(opencv_world SHARED IMPORTED)
    set_property(TARGET opencv_world APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
    set_target_properties(opencv_world PROPERTIES
        IMPORTED_IMPLIB_DEBUG "${OPENCV_DEBUG_DIR}/opencv_world4100d.lib"
        IMPORTED_LOCATION_DEBUG "${OPENCV_DEBUG_DIR}/opencv_world4100d.dll"
    )
    set_property(TARGET opencv_world APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
    set_target_properties(opencv_world PROPERTIES
        IMPORTED_IMPLIB_RELEASE "${OPENCV_RELEASE_DIR}opencv_world4100.lib"
        IMPORTED_LOCATION_RELEASE "${OPENCV_RELEASE_DIR}opencv_world4100.dll"
    )
    # 设置 RELWITHDEBINFO 配置
    set_property(TARGET opencv_world APPEND PROPERTY IMPORTED_CONFIGURATIONS RELWITHDEBINFO)
    set_target_properties(opencv_world PROPERTIES
        IMPORTED_IMPLIB_RELWITHDEBINFO "${OPENCV_RELEASE_DIR}opencv_world4100.lib"
        IMPORTED_LOCATION_RELWITHDEBINFO "${OPENCV_RELEASE_DIR}opencv_world4100.dll"
    )
    
    # 设置 MINSIZEREL 配置
    set_property(TARGET opencv_world APPEND PROPERTY IMPORTED_CONFIGURATIONS MINSIZEREL)
    set_target_properties(opencv_world PROPERTIES
        IMPORTED_IMPLIB_MINSIZEREL "${OPENCV_RELEASE_DIR}opencv_world4100.lib"
        IMPORTED_LOCATION_MINSIZEREL "${OPENCV_RELEASE_DIR}opencv_world4100.dll"
    )
    set(OPENCV_LIBS opencv_world)
    set(OpenCV_LIBS opencv_world)

# set(OPENCV_INCLUDE_DIR ${OPENCV_DIR}/include)
# set(OPENCV_DEBUG_DIR ${OPENCV_DIR}/lib/debug/)
# set(OPENCV_RELEASE_DIR ${OPENCV_DIR}/lib/release/)

# file(COPY 
#     ${OPENCV_DEBUG_DIR}/opencv_world4100d.dll 
#     #${OPENCV_DEBUG_DIR}/tbb12_debug.dll
#     #${OPENCV_DEBUG_DIR}/hdf5.dll
#     DESTINATION ../bin/Debug 
# )

# file(COPY 
#     ${OPENCV_RELEASE_DIR}/opencv_world4100.dll 
#     #${OPENCV_RELEASE_DIR}/tbb12.dll
#     #${OPENCV_RELEASE_DIR}/hdf5.dll
#     DESTINATION ../bin/Release
# )

# file(COPY 
#     ${OPENCV_RELEASE_DIR}/opencv_world4100.dll 
#     #${OPENCV_RELEASE_DIR}/tbb12.dll
#     #${OPENCV_RELEASE_DIR}/hdf5.dll
#     DESTINATION ../bin/RelWithDebInfo
# )

else(OPENCV_FOUND)
    message(FATAL_ERROR "Could not find OpenCV")
endif(OPENCV_FOUND)
