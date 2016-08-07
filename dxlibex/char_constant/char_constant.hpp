/*=============================================================================
  Copyright (C) 2015-2016 DxLibEx project
  https://github.com/Nagarei/DxLibEx/

  Distributed under the Boost Software License, Version 1.0.
  (See http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#ifndef DXLE_INC_CHAR_CONSTANT_CHAR_CONSTANT_HPP_
#define DXLE_INC_CHAR_CONSTANT_CHAR_CONSTANT_HPP_
#include "dxlibex/config/no_min_max.h"
#include "dxlibex/config/defines.h"
#include <cwchar>
#include <cuchar>
namespace dxle{
	namespace char_constant{
		template<typename CharType>
		DXLE_CONSTEXPR CharType comma();
		template<> DXLE_CONSTEXPR char comma() { return ','; }
		template<> DXLE_CONSTEXPR wchar_t comma() { return L','; }
	}
}
#endif //DXLE_INC_CHAR_CONSTANT_CHAR_CONSTANT_HPP_
