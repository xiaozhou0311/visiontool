#主要用于将运行时所需要的三方库的文件拷贝到运行目录下

file(COPY 

    ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/ort_install_1.18/lib/onnxruntime.dll
    ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/ort_install_1.18/lib/onnxruntime_providers_shared.dll


    DESTINATION ../bin/Debug 
)

file(COPY 

    ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/ort_install_1.18/lib/onnxruntime.dll
    ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/ort_install_1.18/lib/onnxruntime_providers_shared.dll

    DESTINATION ../bin/Release
)