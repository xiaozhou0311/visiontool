
find_path(J3D_DIR j3dvision
    PATHS "${DIR_PROJECT_CURRENT}thirdparty/"
    NO_DEFAULT_PATH
)

if (J3D_DIR)
    message(STATUS "Found j3dvision in ${J3D_DIR}")
    set(J3D_INCLUDE_DIR ${J3D_DIR}/j3dvision/include/)
    set(J3D_INCLUDE_DIRS
        ${J3D_INCLUDE_DIR}
    )
    set(J3D_LIBRARY_DIR ${J3D_DIR}/j3dvision/lib)
    set(J3D_LIBRARY_DEBUG_DIR ${J3D_LIBRARY_DIR}/debug/)
    set(J3D_LIBRARY_RELEASE_DIR ${J3D_LIBRARY_DIR}/release/)
    set(J3D_LIBRARY_DIRS ${J3D_LIBRARY_DEBUG_DIR} ${J3D_LIBRARY_RELEASE_DIR})
    
    add_library(j3dvision SHARED IMPORTED)
    set_property(TARGET j3dvision APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
    set_target_properties(j3dvision PROPERTIES
        IMPORTED_IMPLIB_DEBUG "${J3D_LIBRARY_DEBUG_DIR}J3DVision.lib"
        IMPORTED_LOCATION_DEBUG "${J3D_LIBRARY_DEBUG_DIR}J3DVision.dll"
    )
    set_property(TARGET j3dvision APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
    set_target_properties(j3dvision PROPERTIES
        IMPORTED_IMPLIB_RELEASE "${J3D_LIBRARY_RELEASE_DIR}J3DVision.lib"
        IMPORTED_LOCATION_RELEASE "${J3D_LIBRARY_RELEASE_DIR}J3DVision.dll"
    )
    # 设置 RELWITHDEBINFO 配置
    set_property(TARGET j3dvision APPEND PROPERTY IMPORTED_CONFIGURATIONS RELWITHDEBINFO)
    set_target_properties(j3dvision PROPERTIES
        IMPORTED_IMPLIB_RELWITHDEBINFO "${J3D_LIBRARY_RELEASE_DIR}J3DVision.lib"
        IMPORTED_LOCATION_RELWITHDEBINFO "${J3D_LIBRARY_RELEASE_DIR}J3DVision.dll"
    )
    
    # 设置 MINSIZEREL 配置
    set_property(TARGET j3dvision APPEND PROPERTY IMPORTED_CONFIGURATIONS MINSIZEREL)
    set_target_properties(j3dvision PROPERTIES
        IMPORTED_IMPLIB_MINSIZEREL "${J3D_LIBRARY_RELEASE_DIR}J3DVision.lib"
        IMPORTED_LOCATION_MINSIZEREL "${J3D_LIBRARY_RELEASE_DIR}J3DVision.dll"
    )

    set(J3D_LIBS j3dvision)
else()
    message(STATUS "Could not find j3dvision")
endif()