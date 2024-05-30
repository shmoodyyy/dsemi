#pragma once
#include "dsemi/util/logger.h"

#define GFX_LOG_DEBUG(x) LOG_DEBUG(L"[GFX]"x)
#define GFX_LOG_TRACE(x) LOG_TRACE(L"[GFX]"x)
#define GFX_LOG_INFO(x) LOG_INFO(L"[GFX]"x)
#define GFX_LOG_WARN(x) LOG_WARN(L"[GFX]"x)
#define GFX_LOG_ERROR(x) LOG_ERROR(L"[GFX]"x)
#define GFX_LOG_CRITICAL(x) LOG_CRITICAL(L"[GFX]"x)
