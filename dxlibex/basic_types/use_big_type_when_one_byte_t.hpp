/*=============================================================================
  Copyright (C) 2015-2016 DxLibEx project
  https://github.com/Nagarei/DxLibEx/

  Distributed under the Boost Software License, Version 1.0.
  (See http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#ifndef DXLE_INC_BASIC_TYPES_ARTHMETIC_T_HPP_
#define DXLE_INC_BASIC_TYPES_ARTHMETIC_T_HPP_
#include "dxlibex/type_traits/first_enabled.hpp"
#include "dxlibex/type_traits/enable_if.hpp"
namespace dxle {
	namespace detail {
		template<typename T, bool is_arithmetic>
		struct use_big_type_when_one_byte_impl {
			using type = T;
		};
		template<typename T>
		struct use_big_type_when_one_byte_impl<T, true> : first_enabled<
			enable_if<1 != sizeof(T), T>,
			enable_if<std::is_signed<T>::value, int>,
			unsigned int
		>
		{};
	}
	template<typename T>
	struct use_big_type_when_one_byte : dxle::detail::use_big_type_when_one_byte_impl<T, std::is_arithmetic<T>::value> {};
	//! for int8_t/uint8_t
	template<typename T>
	using use_big_type_when_one_byte_t = typename use_big_type_when_one_byte<T>::type;
}
#endif //DXLE_INC_BASIC_TYPES_ARTHMETIC_T_HPP_
