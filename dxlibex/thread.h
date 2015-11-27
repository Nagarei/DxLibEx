#ifndef DXLE_INC_THREAD_H_
#define DXLE_INC_THREAD_H_
#include <mutex>
#include "DxLib.h"
#include "dxlibex/config/defines.h"

#ifdef DX_THREAD_SAFE
#define DXLE_GET_LOCK(mtx) (std::lock_guard<std::mutex> lock(mtx);)
#define DXLE_NOEXCEPT_OR_NOTHROW_SINGLE
#else
#define DXLE_GET_LOCK(mtx)
#define DXLE_NOEXCEPT_OR_NOTHROW_SINGLE DXLE_NOEXCEPT_OR_NOTHROW
#endif

//インクルードガード
#endif
