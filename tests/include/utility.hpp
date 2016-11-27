/*=============================================================================
Copyright (C) 2015-2016 DxLibEx project
https://github.com/Nagarei/DxLibEx/

Distributed under the Boost Software License, Version 1.0.
(See http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#ifndef DXLE_TESTS_INCLUDE_UTILITY_HPP_
#define DXLE_TESTS_INCLUDE_UTILITY_HPP_
#include "dxlibex/config/defines.h"
template<typename T>
DXLE_CONSTEXPR bool is_l_zero(T&& val)
{
	return static_cast<T>(0) == val;
}
template<typename T, typename... Rest>
DXLE_CONSTEXPR bool is_l_zero(T&& val, Rest&&... rest)
{
	return static_cast<T>(0) == val && is_l_zero(std::forward<Rest>(rest)...);
}

#endif //DXLE_TESTS_INCLUDE_UTILITY_HPP_
