#pragma once

#include "dsemi/util/logger.h"
#define GFX_LOG_PREFIX_API L"[NOAPI] "
#define GFX_LOG_DEBUG(x)	LOG_DEBUG(GFX_LOG_PREFIX_API x)
#define GFX_LOG_TRACE(x)	LOG_TRACE(GFX_LOG_PREFIX_API x)
#define GFX_LOG_INFO(x)		LOG_INFO(GFX_LOG_PREFIX_API x)
#define GFX_LOG_WARN(x)		LOG_WARN(GFX_LOG_PREFIX_API x)
#define GFX_LOG_ERROR(x)	LOG_ERROR(GFX_LOG_PREFIX_API x)
#define GFX_LOG_CRITICAL(x) LOG_CRITICAL(GFX_LOG_PREFIX_API x)

#if (defined _WIN32 || defined _WIN64)
#include "dsemi/graphics/directx/directx11.h"

#undef	GFX_LOG_PREFIX_API
#define GFX_LOG_PREFIX_API L"[DX11] "

#else

#endif
