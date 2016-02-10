/*=============================================================================
  Copyright (C) 2015-2016 DxLibEx project
  https://github.com/Nagarei/DxLibEx/

  Distributed under the Boost Software License, Version 1.0.
  (See http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#ifndef DXLE_INC_TYPE_TRAITS_IS_NOTHROW_HPP_
#define DXLE_INC_TYPE_TRAITS_IS_NOTHROW_HPP_
#include "dxlibex/config/no_min_max.h"
#include <type_traits>

namespace dxle {
//!inline
namespace type_traits {

	template<typename From, typename To>
	struct is_nothrow_convertable
		: std::conditional<std::is_nothrow_move_constructible<From>::value
						&& std::is_nothrow_constructible<To, From>::value
		, std::true_type, std::false_type>::type
	{};

}//namespace
using namespace type_traits;
}

#endif
