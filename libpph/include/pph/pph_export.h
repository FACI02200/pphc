/*
 * PPH Export - DLL/Framework export macros
 * Copyright (c) 2025 OpenPajak Contributors
 */

#ifndef PPH_EXPORT_H
#define PPH_EXPORT_H

/* Platform detection for DLL export/import */
#if defined(_WIN32) || defined(__CYGWIN__)
    /* Windows (MSVC, MinGW, OpenWatcom, Cygwin) */
    #if defined(PPH_BUILD_DLL)
        /* Building the DLL */
        #if defined(__GNUC__) || defined(__clang__)
            #define PPH_EXPORT __attribute__((dllexport))
        #else
            #define PPH_EXPORT __declspec(dllexport)
        #endif
    #elif defined(PPH_DLL)
        /* Using the DLL */
        #if defined(__GNUC__) || defined(__clang__)
            #define PPH_EXPORT __attribute__((dllimport))
        #else
            #define PPH_EXPORT __declspec(dllimport)
        #endif
    #else
        /* Static library */
        #define PPH_EXPORT
    #endif
#else
    /* Unix/Linux/macOS */
    #if defined(__GNUC__) || defined(__clang__)
        #if __GNUC__ >= 4 || defined(__clang__)
            #define PPH_EXPORT __attribute__((visibility("default")))
        #else
            #define PPH_EXPORT
        #endif
    #else
        #define PPH_EXPORT
    #endif
#endif

#endif /* PPH_EXPORT_H */
