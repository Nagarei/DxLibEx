/*=============================================================================
Copyright (C) 2015-2016 DxLibEx project
https://github.com/Nagarei/DxLibEx/

Distributed under the Boost Software License, Version 1.0.
(See http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#ifndef DXLE_INC_BASIC_TYPES_BEL_HPP_
#define DXLE_INC_BASIC_TYPES_BEL_HPP_
#include <limits>
#include <ratio>
#include <utility>
#include "dxlibex/type_traits/is_ratio.hpp"
#include "dxlibex/type_traits/enable_if.hpp"
namespace dxle {
	typedef std::ratio<1, 10000> myrio;
	namespace sound_units {
		template<typename T, typename Period, enable_if<is_ratio<Period>::value, nullptr_t> = nullptr>
		class bel {

		};
	}
}
#endif //DXLE_INC_BASIC_TYPES_BEL_HPP_
