
find_path(CV2D_DIR cv2d
    PATHS "${DIR_PROJECT_CURRENT}thirdparty/"
    NO_DEFAULT_PATH
)

if (CV2D_DIR)
    message(STATUS "Found cv2d in ${CV2D_DIR}")
    set(CV2D_INCLUDE_DIR ${CV2D_DIR}/cv2d/)
    set(CV2D_INCLUDE_DIRS
        # ${CV2D_INCLUDE_DIR}binarization
        ${CV2D_INCLUDE_DIR}commenthead
        ${CV2D_INCLUDE_DIR}cuda
        ${CV2D_INCLUDE_DIR}definition
        ${CV2D_INCLUDE_DIR}detection
        ${CV2D_INCLUDE_DIR}fit
        ${CV2D_INCLUDE_DIR}pretreatment
        ${CV2D_INCLUDE_DIR}statistics
        ${CV2D_INCLUDE_DIR}tool
        ${CV2D_INCLUDE_DIR}geometry/definition
        ${CV2D_INCLUDE_DIR}geometry/draw
        ${CV2D_INCLUDE_DIR}geometry/process
        ${CV2D_INCLUDE_DIR}geometry/functional
        ${CV2D_INCLUDE_DIR}

        ${CV2D_INCLUDE_DIR}include/commenthead
        ${CV2D_INCLUDE_DIR}include/cuda
        ${CV2D_INCLUDE_DIR}include/definition
        ${CV2D_INCLUDE_DIR}include/detection
        ${CV2D_INCLUDE_DIR}include/fit
        ${CV2D_INCLUDE_DIR}include/pretreatment
        ${CV2D_INCLUDE_DIR}include/statistics
        ${CV2D_INCLUDE_DIR}include/tool
        ${CV2D_INCLUDE_DIR}include/geometry/definition
        ${CV2D_INCLUDE_DIR}include/geometry/draw
        ${CV2D_INCLUDE_DIR}include/geometry/process
        ${CV2D_INCLUDE_DIR}include/geometry/functional
    )
    set(CV2D_LIBRARY_DIR ${CV2D_DIR}/cv2d/)
    set(CV2D_LIBRARY_DEBUG_DIR ${CV2D_LIBRARY_DIR}/Debug/)
    set(CV2D_LIBRARY_RELEASE_DIR ${CV2D_LIBRARY_DIR}/Release/)
    set(CV2D_LIBRARY_DIRS ${CV2D_LIBRARY_DEBUG_DIR} ${CV2D_LIBRARY_RELEASE_DIR})
    
    add_library(cv2d SHARED IMPORTED)
    set_property(TARGET cv2d APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG RELEASE)
    set_target_properties(cv2d PROPERTIES
        IMPORTED_IMPLIB_DEBUG "${CV2D_LIBRARY_DEBUG_DIR}cv2dd.lib"
        IMPORTED_LOCATION_DEBUG "${CV2D_LIBRARY_DEBUG_DIR}cv2dd.dll"
    )
    set_target_properties(cv2d PROPERTIES
        IMPORTED_IMPLIB_RELEASE "${CV2D_LIBRARY_RELEASE_DIR}cv2d.lib"
        IMPORTED_LOCATION_RELEASE "${CV2D_LIBRARY_RELEASE_DIR}cv2d.dll"
    )
    # 设置 RELWITHDEBINFO 配置
    set_property(TARGET cv2d APPEND PROPERTY IMPORTED_CONFIGURATIONS RELWITHDEBINFO)
    set_target_properties(cv2d PROPERTIES
        IMPORTED_IMPLIB_RELWITHDEBINFO "${CV2D_LIBRARY_RELEASE_DIR}cv2d.lib"
        IMPORTED_LOCATION_RELWITHDEBINFO "${CV2D_LIBRARY_RELEASE_DIR}cv2d.dll"
    )
    
    # 设置 MINSIZEREL 配置
    set_property(TARGET cv2d APPEND PROPERTY IMPORTED_CONFIGURATIONS MINSIZEREL)
    set_target_properties(cv2d PROPERTIES
        IMPORTED_IMPLIB_MINSIZEREL "${CV2D_LIBRARY_RELEASE_DIR}cv2d.lib"
        IMPORTED_LOCATION_MINSIZEREL "${CV2D_LIBRARY_RELEASE_DIR}cv2d.dll"
    )

    set(CV2D_LIBRARIES cv2d)
else()
    message(STATUS "Could not find cv2d")
endif()