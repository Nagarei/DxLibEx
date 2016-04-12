/*=============================================================================
  Copyright (C) 2015 DxLibEx project
  https://github.com/Nagarei/DxLibEx/

  Distributed under the Boost Software License, Version 1.0.
  (See http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#include "dxlibex/graph2d.h"

namespace dxle{
namespace Graph2D {

#ifdef DX_THREAD_SAFE
		std::mutex screen_mutex_c::mtx;
#endif

//namespaces
}}