/*=============================================================================
  Copyright (c) 2011-2015 Bolero MURAKAMI
  https://github.com/bolero-MURAKAMI/Sprout
  Copyright (C) 2015-2016 DxLibEx project
  https://github.com/Nagarei/DxLibEx/

  Distributed under the Boost Software License, Version 1.0.
  (See http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#ifndef DXLE_INC_MATH_FABS_HPP_
#define DXLE_INC_MATH_FABS_HPP_
#include "dxlibex/config/defines.h"
#include <cmath>
#include "dxlibex/type_traits/enable_if.hpp"
#include "dxlibex/math/copysign.hpp"
#ifdef DXLE_NO_CXX11_CONSTEXPR_CLASS
namespace dxle{
	namespace math{
		using std::fabs;
	}
	using dxle::math::fabs;
}
#else //DXLE_NO_CXX11_CONSTEXPR_CLASS
namespace dxle {
	namespace math {
		template<typename FloatType, enable_if_t<std::is_floating_point<FloatType>::value, std::nullptr_t> = nullptr>
		inline DXLE_CONSTEXPR FloatType fabs(FloatType x) DXLE_NOEXCEPT_OR_NOTHROW
		{
			return
				dxle::math::isnan(x) ? std::numeric_limits<FloatType>::quiet_NaN()
				: x == 0 ? FloatType(0)
				: dxle::math::copysign(x, FloatType(0))
				;
		}
		template<typename IntType, enable_if_t<std::is_integral<IntType>::value, std::nullptr_t> = nullptr>
		inline DXLE_CONSTEXPR double fabs(IntType x) DXLE_NOEXCEPT_OR_NOTHROW
		{
			return dxle::math::fabs(static_cast<double>(x));
		}
	}	// namespace math

	using dxle::math::fabs;
}	// namespace dxle
#endif //DXLE_NO_CXX11_CONSTEXPR_CLASS
#endif //DXLE_INC_MATH_FABS_HPP_
