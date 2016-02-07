/*=============================================================================
  Copyright (C) 2015-2016 DxLibEx project
  https://github.com/Nagarei/DxLibEx/

  Distributed under the Boost Software License, Version 1.0.
  (See http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#ifndef DXLE_INC_TYPE_TRAITS_IS_WELL_FORMAT_HPP_
#define DXLE_INC_TYPE_TRAITS_IS_WELL_FORMAT_HPP_
#include <type_traits>
namespace dxle {
//!inline
namespace type_traits {

	template<typename T, typename = std::nullptr_t>
	struct has_operator_notequal_to_zero_impl : std::false_type {};
	template<typename T>
	struct has_operator_notequal_to_zero_impl < T, typename enable_if<ignore_type<decltype(std::declval<T>() != 0)>::value, std::nullptr_t>::type >
		: std::true_type {};
	template<typename T>
	struct has_operator_notequal_to_zero : has_operator_notequal_to_zero_impl<T> {};
	
	template<typename T, typename = std::nullptr_t>
	struct has_operator_notequal_to_this_impl : std::false_type {};
	template<typename T>
	struct has_operator_notequal_to_this_impl < T, typename enable_if<ignore_type<decltype(std::declval<T>() != std::declval<T>())>::value, std::nullptr_t>::type >
		: std::true_type {};
	template<typename T>
	struct has_operator_notequal_to_this : has_operator_notequal_to_this_impl<T> {};

}//namespace
using namespace type_traits;
}
#endif //DXLE_INC_TYPE_TRAITS_IS_WELL_FORMAT_HPP_
