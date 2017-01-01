/*=============================================================================
  Copyright (C) 2015-2017 DxLibEx project
  https://github.com/Nagarei/DxLibEx/

  Distributed under the Boost Software License, Version 1.0.
  (See http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#ifndef DXLE_INC_ALGORITHM_SAFE_DIST_HPP_
#define DXLE_INC_ALGORITHM_SAFE_DIST_HPP_
#include "dxlibex/config/defines.h"
#include "dxlibex/type_traits/enable_if.hpp"
namespace dxle {
	template<typename T1, typename T2>
	DXLE_DEPRECATED_MESSAGE("use abs_diff() instead.") static inline DXLE_CONSTEXPR auto safe_dist(const T1& n1, const T2& n2) DXLE_NOEXCEPT_IF_EXPR((n1 - n2)) -> decltype(n1 - n2)
	{
		return (n1 < n2) ? n2 - n1 : n1 - n2;
	}
}
#endif //DXLE_INC_ALGORITHM_SAFE_DIST_HPP_
