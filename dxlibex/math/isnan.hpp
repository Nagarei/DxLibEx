/*=============================================================================
  Copyright (C) 2015-2016 DxLibEx project
  https://github.com/Nagarei/DxLibEx/

  Distributed under the Boost Software License, Version 1.0.
  (See http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#ifndef DXLE_INC_MATH_ISNAN_HPP_
#define DXLE_INC_MATH_ISNAN_HPP_
#include "dxlibex/config/defines.h"
#include "dxlibex/type_traits.hpp"
namespace dxle {
	namespace math {
		template<typename FloatType, enable_if_t<std::is_floating_point<FloatType>::value, std::nullptr_t> = nullptr>
		inline DXLE_CONSTEXPR bool isnan(FloatType x) DXLE_NOEXCEPT_OR_NOTHROW {	return !(x == x); }
		template<typename IntType, enable_if_t<std::is_integral<IntType>::value> = nullptr>
		inline DXLE_CONSTEXPR bool isnan(IntType) DXLE_NOEXCEPT_OR_NOTHROW { return false; }
	}	// namespace math

	using dxle::math::isnan;
}
#endif //DXLE_INC_MATH_ISNAN_HPP_