﻿/*=============================================================================
  Copyright (C) 2015-2016 DxLibEx project
  https://github.com/Nagarei/DxLibEx/

  Distributed under the Boost Software License, Version 1.0.
  (See http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#ifndef DXLE_INC_MATH_ABS_HPP_
#define DXLE_INC_MATH_ABS_HPP_
#include "dxlibex/math/isnan.hpp"
#include "dxlibex/math/fabs.hpp"
namespace dxle {
	namespace math {
		template<typename FloatType, enable_if_t<std::is_floating_point<FloatType>::value, std::nullptr_t> = nullptr>
		inline DXLE_CONSTEXPR FloatType abs(FloatType x) DXLE_NOEXCEPT_OR_NOTHROW
		{
			return dxle::math::fabs(x);
		}
	}	// namespace math

	using dxle::math::abs;
}	// namespace dxle
#endif //DXLE_INC_MATH_ABS_HPP_