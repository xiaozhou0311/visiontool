# Version for libraries CPP
SET(VERSION "2.0.0")
SET(OPENCVVERSION "4.8.1")
SET(CUDAVERSION "11.3")
#设置项目当前路径
Set(DIR_PROJECT_CURRENT "${CMAKE_CURRENT_SOURCE_DIR}/")

# 使用 execute_process 命令调用 git rev-parse 获取当前提交编号
execute_process(
    COMMAND git rev-parse HEAD
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    OUTPUT_VARIABLE GIT_COMMIT_HASH
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

# 提取前5位数字
string(SUBSTRING ${GIT_COMMIT_HASH} 0 8 GIT_COMMIT_SHORT_HASH)

# 设置 CMake 变量以在项目中使用前5位数字
set(PROJECT_COMMIT_SHORT_HASH ${GIT_COMMIT_SHORT_HASH})


 
#RC 信息
set(COMPANY_NAME    "JRS AOI2.0") 
set(PRODUCT_NAME    "AOI2.0")
set(LegalCopyRight  "Copyright(C) 2023 The JRS Company Ltd.")
set(VERSION_MAJOR   2)
set(VERSION_MINOR   0) 
set(VERSION_PATCH   1)
set(VERSION_REVISION ${PROJECT_COMMIT_SHORT_HASH}) 


if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(JRS_VERSIONINFO_RC "${DIR_PROJECT_CURRENT}bin/Debug/VersionInfo.rc")
else()
    set(JRS_VERSIONINFO_RC "${DIR_PROJECT_CURRENT}bin/Release/VersionInfo.rc")
endif()

configure_file("cmake/VersionInfo.rc.in"
    "${JRS_VERSIONINFO_RC}" 
)







