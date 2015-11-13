#ifndef DXLIBEX_THREAD_H_201511132146_85168523652
#define DXLIBEX_THREAD_H_201511132146_85168523652
#include <mutex>
#include "DxLib.h"
#include "DxLibEx_Defines.h"

#ifdef DX_THREAD_SAFE
#define DXLIBEX_GET_LOCK() (std::lock_guard<std::mutex> lock(mtx);)
#define DXLIBEX_NOEXCEPT_SINGLE
#else
#define DXLIBEX_GET_LOCK()
#define DXLIBEX_NOEXCEPT_SINGLE DXLIBEX_NOEXCEPT
#endif

//インクルードガード
#endif
