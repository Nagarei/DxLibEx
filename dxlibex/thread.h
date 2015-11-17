#ifndef DXLE_INC_THREAD_H_
#define DXLE_INC_THREAD_H_
#include <mutex>
#include "DxLib.h"
#include "dxlibex/Defines.h"

#ifdef DX_THREAD_SAFE
#define DXLE_GET_LOCK() (std::lock_guard<std::mutex> lock(mtx);)
#define DXLE_NOEXCEPT_SINGLE
#else
#define DXLE_GET_LOCK()
#define DXLE_NOEXCEPT_SINGLE DXLE_NOEXCEPT
#endif

//インクルードガード
#endif
