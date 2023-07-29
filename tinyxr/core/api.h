#ifndef TINYXR_CORE_API_H
#define TINYXR_CORE_API_H

#include "tinyxr/core/export.h"

// clang-format off

#if defined(TINYXR_EXPORT)
  #define TINYXR_API TINYXR_API_EXPORT
#else
  #define TINYXR_API TINYXR_API_IMPORT
#endif

// clang-format on

#endif // TINYXR_CORE_API_H
