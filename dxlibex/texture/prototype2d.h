/*=============================================================================
  Copyright (C) 2015-2016 DxLibEx project
  https://github.com/Nagarei/DxLibEx/

  Distributed under the Boost Software License, Version 1.0.
  (See http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#ifndef DXLE_INC_TEXTURE_PROTOTYPE2D_H_
#define DXLE_INC_TEXTURE_PROTOTYPE2D_H_

#include "dxlibex/config/no_min_max.h"
#include "DxLib.h"
#include "dxlibex/basic_types.hpp"

namespace dxle
{
	//! 2Dグラフィック
	namespace graph2d
	{
		class texture2d;
		class screen;

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
