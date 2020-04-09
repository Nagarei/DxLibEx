/*=============================================================================
  Copyright (C) 2015-2017 DxLibEx project
  https://github.com/Nagarei/DxLibEx/

  Distributed under the Boost Software License, Version 1.0.
  (See http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#ifndef DXLE_INC_ALGORITHM_ABS_DIFF_HPP_
#define DXLE_INC_ALGORITHM_ABS_DIFF_HPP_
#include "dxlibex/config/defines.h"
#include "dxlibex/type_traits/enable_if.hpp"
#include "dxlibex/type_traits/ignore.hpp"
#include "dxlibex/type_traits/result_of.hpp"
namespace dxle {
	template <typename T1, typename T2, enable_if_t<
		std::is_same<decltype(std::declval<T1>() - std::declval<T2>()), decltype(std::declval<T2>() - std::declval<T2>())>::value
		&& std::is_same<bool, decltype(std::declval<T1>() < std::declval<T2>())>::value,
		nullptr_t
	> = nullptr>
	static inline DXLE_CONSTEXPR auto abs_diff(const T1& a, const T2& b) 
		DXLE_NOEXCEPT_IF(
			DXLE_NOEXCEPT_EXPR(std::declval<T1>() - std::declval<T2>()) 
			&& DXLE_NOEXCEPT_EXPR(std::declval<T2>() - std::declval<T1>())
			&& DXLE_NOEXCEPT_EXPR(std::declval<T1>() < std::declval<T2>())
		)
		-> decltype(std::declval<T1>() - std::declval<T2>()) 
	{
		return (a < b) ? b - a : a - b;
	}
	template <typename T1, typename T2, typename Compare, enable_if_t<
		std::is_same<decltype(std::declval<T1>() - std::declval<T2>()), decltype(std::declval<T2>() - std::declval<T2>())>::value
		&& ignore_type<decltype(std::declval<Compare>()(std::declval<T1>(), std::declval<T2>()))>::value
		&& std::is_same<bool, result_of_t<Compare(T1, T2)>>::value,
		nullptr_t
	> = nullptr>
	static inline DXLE_CONSTEXPR auto abs_diff(const T1& a, const T2& b, const Compare& comp)
		DXLE_NOEXCEPT_IF(
			DXLE_NOEXCEPT_EXPR(std::declval<T1>() - std::declval<T2>()) 
			&& DXLE_NOEXCEPT_EXPR(std::declval<T2>() - std::declval<T1>())
			&& DXLE_NOEXCEPT_EXPR(std::declval<Compare>()(std::declval<T1>(), std::declval<T2>()))
		)
		-> decltype(std::declval<T1>() - std::declval<T2>())
	{
		return comp(a, b) ? b - a : a - b;
	}
	template <typename T1, typename T2, typename Compare, typename Difference, enable_if_t<
		std::is_same<decltype(std::declval<T1>() - std::declval<T2>()), decltype(std::declval<T2>() - std::declval<T2>())>::value
		&& ignore_type<decltype(std::declval<Difference>()(std::declval<T1>(), std::declval<T2>()))>::value
		&& ignore_type<decltype(std::declval<Difference>()(std::declval<T2>(), std::declval<T1>()))>::value
		&& std::is_same<result_of_t<Difference(T1, T2)>, result_of_t<Difference(T2, T1)>>::value
		&& ignore_type<decltype(std::declval<Compare>()(std::declval<T1>(), std::declval<T2>()))>::value
		&& std::is_same<bool, result_of_t<Compare(T1, T2)>>::value,
		nullptr_t
	> = nullptr>
	static inline DXLE_CONSTEXPR result_of<Difference(T1, T2)> abs_diff(const T1& a, const T2& b, const Compare& comp, const Difference& diff)
		DXLE_NOEXCEPT_IF(
			DXLE_NOEXCEPT_EXPR(std::declval<Difference>()(std::declval<T1>(), std::declval<T2>()))
			&& DXLE_NOEXCEPT_EXPR(std::declval<Difference>()(std::declval<T2>(), std::declval<T1>()))
			&& DXLE_NOEXCEPT_EXPR(std::declval<Compare>()(std::declval<T1>(), std::declval<T2>()))
		)
	{
		return comp(a, b) ? diff(b, a) : diff(a, b);
	}
}
#endif //DXLE_INC_ALGORITHM_ABS_DIFF_HPP_
