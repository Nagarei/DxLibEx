/*=============================================================================
Copyright (C) 2015-2016 DxLibEx project
https://github.com/Nagarei/DxLibEx/

Distributed under the Boost Software License, Version 1.0.
(See http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#ifndef DXLE_INC_TYPE_TRAITS_IS_RATIO_HPP_
#define DXLE_INC_TYPE_TRAITS_IS_RATIO_HPP_
#include <ratio>
#include <type_traits>
#include <cstdint>
namespace dxle {
	template <typename T> struct is_ratio : std::false_type {};
	template <std::intmax_t N, std::intmax_t D> struct is_ratio<std::ratio<N, D>> : std::true_type {};
}
#endif //DXLE_INC_TYPE_TRAITS_IS_RATIO_HPP_
