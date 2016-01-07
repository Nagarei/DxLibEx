/*=============================================================================
  Copyright (C) 2015-2016 DxLibEx project
  https://github.com/Nagarei/DxLibEx/

  Distributed under the Boost Software License, Version 1.0.
  (See http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#ifndef DXLE_INC_ALGORITHM_SAFE_DIST_HPP_
#define DXLE_INC_ALGORITHM_SAFE_DIST_HPP_
#include "dxlibex/config/defines.h"
#include "dxlibex/type_traits/enable_if.hpp"
namespace dxle {
	template<typename T1, typename T2, enable_if_t<std::is_arithmetic<T1>::value && std::is_arithmetic<T2>::value, std::nullptr_t> = nullptr>
	auto safe_dist(T1 n1, T2 n2) DXLE_NOEXCEPT_OR_NOTHROW -> decltype(n1 - n2)
	{
		return (n1 < n2) ? n2 - n1 : n1 - n2;
	}
}
#endif //DXLE_INC_ALGORITHM_SAFE_DIST_HPP_
