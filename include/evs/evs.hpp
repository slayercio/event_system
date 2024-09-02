#pragma once

#ifndef _EVS_HPP
#define _EVS_HPP

#if defined(_MSC_VER) && !defined(EVS_EXPORT) && !defined(EVS_IMPORT)
#   define EVS_EXPORT __declspec(dllexport)
#   define EVS_IMPORT __declspec(dllimport)
#endif

#if defined(__GNUC__) || defined(__clang__) && !defined(EVS_EXPORT) && !defined(EVS_IMPORT)
#   define EVS_EXPORT __attribute__((visibility("normal")))
#   define EVS_IMPORT
#endif

#if defined(_EVS_BUILD_LIB)
#   define EVS_API EVS_EXPORT
#elif defined(_EVS_USE_LIB)
#   define EVS_API EVS_IMPORT
#else
#   define EVS_API
#endif

#endif
