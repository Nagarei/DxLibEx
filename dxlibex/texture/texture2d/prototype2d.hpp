/*=============================================================================
  Copyright (C) 2015-2016 DxLibEx project
  https://github.com/Nagarei/DxLibEx/

  Distributed under the Boost Software License, Version 1.0.
  (See http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#ifndef DXLE_INC_TEXTURE_TEXTURE2D_PROTOTYPE2D_HPP_
#define DXLE_INC_TEXTURE_TEXTURE2D_PROTOTYPE2D_HPP_

#include "dxlibex/config/no_min_max.h"
#if defined(__c2__) &&  __clang_major__ == 3 && __clang_minor__ == 8
//To avoid compile error
//C:\Program Files (x86)\Windows Kits\8.1\Include\um\combaseapi.h(229,21): error : unknown type name 'IUnknown'
//          static_cast<IUnknown*>(*pp);    // make sure everyone derives from IUnknown
struct IUnknown;
#endif
#include "DxLib.h"
#include "dxlibex/basic_types.hpp"

namespace dxle
{
	//! 2Dグラフィック
	namespace graph2d
	{
		class texture2d;
		class screen;

		class animation_graph;

		class shared_texture2d;

		template<size_t N>
		class static_derivative_texture2d;
		class derivative_texture2d;


		namespace gr_impl
		{
			template<typename BuffT, typename OutFunc>
			void LoadDivGraph_impl(OutFunc&& out_func, BuffT* HandleBuf, const TCHAR *FileName, int AllNum, const dxle::sizei& Num, const dxle::sizei& Size, bool NotUse3DFlag);
		}
	}
	using namespace graph2d;
}

#endif
