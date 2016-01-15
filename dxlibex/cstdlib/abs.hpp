/*=============================================================================
  Copyright (C) 2015-2016 DxLibEx project
  https://github.com/Nagarei/DxLibEx/

  Distributed under the Boost Software License, Version 1.0.
  (See http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#ifndef DXLE_INC_CSTDLIB_ABS_HPP_
#define DXLE_INC_CSTDLIB_ABS_HPP_
#include "dxlibex/config/defines.h"
#include <type_traits>
namespace dxle {

	// 7.20.6.1 abs，labs，及び llabs 関数
	inline DXLE_CONSTEXPR int abs(int j) DXLE_NOEXCEPT_OR_NOTHROW
	{
		return j < 0 ? -j : j;
	}

	inline DXLE_CONSTEXPR long labs(long j) DXLE_NOEXCEPT_OR_NOTHROW
	{
		return j < 0 ? -j : j;
	}

	inline DXLE_CONSTEXPR long long llabs(long long j) DXLE_NOEXCEPT_OR_NOTHROW
	{
		return j < 0 ? -j : j;
	}

	inline DXLE_CONSTEXPR long abs(long j) DXLE_NOEXCEPT_OR_NOTHROW { return dxle::labs(j); }

	inline DXLE_CONSTEXPR long long abs(long long j) DXLE_NOEXCEPT_OR_NOTHROW { return dxle::llabs(j); }

	namespace {
		template<typename IntType, enable_if_t<std::is_integral<IntType>::value && std::is_signed<IntType>::value, std::nullptr_t> = nullptr>
		inline DXLE_CONSTEXPR IntType abs(IntType j) DXLE_NOEXCEPT_OR_NOTHROW
		{
			return j < 0 ? -j : j;
		}

		template<typename IntType, enable_if_t<std::is_integral<IntType>::value && std::is_unsigned<IntType>::value, std::nullptr_t> = nullptr>
		inline DXLE_CONSTEXPR IntType abs(IntType j) DXLE_NOEXCEPT_OR_NOTHROW
		{
			return j;
		}
	}	// unnamed namespace
}	// namespace dxle

#endif //DXLE_INC_CSTDLIB_ABS_HPP_