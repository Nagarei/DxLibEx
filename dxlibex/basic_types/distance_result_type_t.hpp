/*=============================================================================
  Copyright (C) 2015-2017 DxLibEx project
  https://github.com/Nagarei/DxLibEx/

  Distributed under the Boost Software License, Version 1.0.
  (See http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#ifndef DXLE_INC_BASIC_TYPES_DISTANCE_RESULT_TYPE_T_HPP_
#define DXLE_INC_BASIC_TYPES_DISTANCE_RESULT_TYPE_T_HPP_
#include "dxlibex/type_traits/first_enabled.hpp"
#include "dxlibex/type_traits/enable_if.hpp"
namespace dxle {
	namespace detail{
		template<typename T, bool is_integral = std::is_integral<T>::value, bool is_float = std::is_floating_point<T>::value>
		struct distance_result_type_impl;
		template<typename T>
		struct distance_result_type_impl<T, false, true> {
			typedef T type;
		};
		template<typename T>
		struct distance_result_type_impl<T, true, false> {
			typedef double type;//If any argument has integral type, it is cast to double.
		};
	}
	template<typename T1, typename T2> struct distance_result_type : dxle::detail::distance_result_type_impl<decltype(std::declval<std::remove_cv_t<T1>>() - std::declval<std::remove_cv_t<T2>>())> {};
	template<typename T1, typename T2>
	using distance_result_type_t = typename distance_result_type<T1, T2>::type;
}
#endif //DXLE_INC_BASIC_TYPES_DISTANCE_RESULT_TYPE_T_HPP_
