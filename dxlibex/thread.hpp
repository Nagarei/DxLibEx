/*=============================================================================
  Copyright (C) 2015-2017 DxLibEx project
  https://github.com/Nagarei/DxLibEx/

  Distributed under the Boost Software License, Version 1.0.
  (See http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#ifndef DXLE_INC_THREAD_HPP_
#define DXLE_INC_THREAD_HPP_

#include "dxlibex/config/no_min_max.h"
#if defined(__c2__) &&  __clang_major__ == 3 && __clang_minor__ == 8
//To avoid compile error
//C:\Program Files (x86)\Windows Kits\8.1\Include\um\combaseapi.h(229,21): error : unknown type name 'IUnknown'
//          static_cast<IUnknown*>(*pp);    // make sure everyone derives from IUnknown
struct IUnknown;
#endif
#include <mutex>
#include "DxLib.h"
#include "dxlibex/config/defines.h"

#ifdef DX_THREAD_SAFE
#define DXLE_GET_LOCK(mtx) std::lock_guard<decltype(mtx)> lock(mtx);
#define DXLE_NOEXCEPT_OR_NOTHROW_SINGLE
#else
#define DXLE_GET_LOCK(mtx)
#define DXLE_NOEXCEPT_OR_NOTHROW_SINGLE DXLE_NOEXCEPT_OR_NOTHROW
#endif

//インクルードガード
#endif
