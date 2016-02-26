/*=============================================================================
  Copyright (c) 2011-2016 Bolero MURAKAMI
  https://github.com/bolero-MURAKAMI/Sprout
  Copyright (C) 2015-2016 DxLibEx project
  https://github.com/Nagarei/DxLibEx/

  Distributed under the Boost Software License, Version 1.0.
  (See http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#ifndef DXLE_INC_MATH_COPYSIGN_HPP_
#define DXLE_INC_MATH_COPYSIGN_HPP_
#include "dxlibex/config/defines.h"
#include <cmath>
#include "dxlibex/math/isnan.hpp"
#include <type_traits>
#include <limits>
#include "dxlibex/type_traits/float_promote.hpp"
#ifdef DXLE_NO_CXX11_CONSTEXPR_CLASS
namespace dxle{
	namespace math{
		using std::copysign;
	}
	using dxle::math::copysign;
}
#else //DXLE_NO_CXX11_CONSTEXPR_CLASS
namespace dxle {
	namespace math {
		namespace detail {
			template<typename FloatType> inline DXLE_CONSTEXPR bool broken_signbit(FloatType x) DXLE_NOEXCEPT_OR_NOTHROW
			{
				return !dxle::math::isnan(x) && x < 0;
			}
		}	// namespace detail
		//
		// copysign
		//
		// issue:
		//	copysign(±x, -0) returns -x for |x| is not 0 .
		//		# returns +x . ( same as copysign(±x, +0) )
		//	copysign(±x, -NaN) returns -x for |x| is not NaN .
		//		# returns +x . ( same as copysign(±x, +NaN) )
		//
		template<
			typename FloatType,
			enable_if_t<std::is_floating_point<FloatType>::value, std::nullptr_t> = nullptr
		>
		inline DXLE_CONSTEXPR FloatType copysign(FloatType x, FloatType y) DXLE_NOEXCEPT_OR_NOTHROW
		{
			return
				x == 0
					? y == 0 ? y
						: dxle::math::detail::broken_signbit(y) ? -FloatType(0)
						: FloatType(0)
				: dxle::math::isnan(x)
					? dxle::math::isnan(y) ? y
						: dxle::math::detail::broken_signbit(y) ? -std::numeric_limits<FloatType>::quiet_NaN()
						: std::numeric_limits<FloatType>::quiet_NaN()
				: dxle::math::detail::broken_signbit(y) != dxle::math::detail::broken_signbit(x) ? -x
				: x
				;
		}

		template<
			typename ArithmeticType1, typename ArithmeticType2,
			enable_if_t<std::is_arithmetic<ArithmeticType1>::value && std::is_arithmetic<ArithmeticType2>::value, std::nullptr_t> = nullptr
		>
		inline DXLE_CONSTEXPR dxle::float_promote_t<ArithmeticType1, ArithmeticType2>
		copysign(ArithmeticType1 x, ArithmeticType2 y) DXLE_NOEXCEPT_OR_NOTHROW  {
			using type = dxle::float_promote_t<ArithmeticType1, ArithmeticType2>;
			return dxle::math::copysign(static_cast<type>(x), static_cast<type>(y));
		}
	}	// namespace math

	using dxle::math::copysign;
}	// namespace dxle

#endif //DXLE_NO_CXX11_CONSTEXPR_CLASS
#endif //DXLE_INC_MATH_COPYSIGN_HPP_