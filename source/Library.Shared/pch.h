#pragma once

// Standard Library
#include <cassert>
#include <cctype>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <functional>
#include <limits>
#include <locale>
#include <memory>
#include <regex>
#include <sstream>
#include <stdexcept>
#include <string>
#include <thread>
#include <utility>
#include <exception>
#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <stack>
#include <iomanip>
#include <codecvt>
#include <algorithm>
#include <filesystem>

#if defined(DEBUG) || defined(_DEBUG)
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#endif

// Guidelines Support Library
#include <gsl\gsl>


#define NOMINMAX

// Windows
#include <windows.h>
#include <winrt\Windows.Foundation.h>

// DirectX
#include <d3dcompiler.h>
#include <d3d11_4.h>
#include <dxgi1_6.h>
#include <DirectXMath.h>
#include <DirectXPackedVector.h>
#include <DirectXColors.h>
#include <DirectXTK\DDSTextureLoader.h>
#include <DirectXTK\WICTextureLoader.h>
#include <DirectXTK\SpriteBatch.h>
#include <DirectXTK\SpriteFont.h>
#include <DirectXTK\GamePad.h>
#include <DirectXTK\Keyboard.h>
#include <DirectXTK\Mouse.h>

#ifndef UNREFERENCED_LOCAL
#define UNREFERENCED_LOCAL(L) (L)
#endif
