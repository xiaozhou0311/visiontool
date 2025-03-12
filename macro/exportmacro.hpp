#ifndef EXPORTMACRO_HPP
#define EXPORTMACRO_HPP

#ifdef _WIN32
    #ifdef VISION_ALGO_EXPORTS
        #define VISION_ALGO_API __declspec(dllexport)
    #else
        #define VISION_ALGO_API __declspec(dllimport)
    #endif
#else
    #define VISION_ALGO_API
#endif

#endif

