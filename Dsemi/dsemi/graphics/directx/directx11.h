#ifndef HEADER_DSEMI_GRAPHICS_DIRECTX11
#define HEADER_DSEMI_GRAPHICS_DIRECTX11

#define GFX_USING_DX11
#include "Dsemi/Core/DsemiWindows.h"

// Library dependencies
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "d2d1")
#pragma comment(lib, "dwrite")

#include <d3d11.h>
#include <dwrite_3.h>
#include <DirectXMath.h>
#include <d3dcompiler.h>

using Microsoft::WRL::ComPtr;


namespace dsemi
{
	namespace graphics
	{

	} // namespace graphics
} // namespace dsemi

#endif
