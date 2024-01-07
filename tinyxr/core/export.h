#ifndef TINYXR_CORE_EXPORT_H
#define TINYXR_CORE_EXPORT_H

#include "tinyxr/core/os.h"

#if defined(TINYXR_OS_WINDOWS)
#define TINYXR_API_EXPORT __declspec(dllexport)
#define TINYXR_API_IMPORT __declspec(dllimport)
#define TINYXR_API_HIDDEN
#elif defined(TINYXR_OS_LINUX)
#define TINYXR_API_EXPORT __attribute__((visibility("default")))
#define TINYXR_API_IMPORT
#define TINYXR_API_HIDDEN __attribute__((visibility("hidden")))
#else
#define TINYXR_API_EXPORT
#define TINYXR_API_IMPORT
#define TINYXR_API_HIDDEN
#endif

#endif  // TINYXR_CORE_EXPORT_H
