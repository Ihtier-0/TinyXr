#ifndef TINYXR_CORE_API_H
#define TINYXR_CORE_API_H

#include "tinyxr/core/export.h"

#if defined(TINYXR_EXPORT)
#define TINYXR_API TINYXR_API_EXPORT
#else
#define TINYXR_API TINYXR_API_IMPORT
#endif

#endif  // TINYXR_CORE_API_H
