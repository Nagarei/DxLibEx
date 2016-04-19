/*=============================================================================
  Copyright (C) 2015-2016 DxLibEx project
  https://github.com/Nagarei/DxLibEx/

  Distributed under the Boost Software License, Version 1.0.
  (See http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#ifndef DXLE_INC_TYPE_TRAITS_IS_REPRESENTABLE_HPP_
#define DXLE_INC_TYPE_TRAITS_IS_REPRESENTABLE_HPP_
#include <type_traits>
#include "dxlibex/type_traits/common_type.hpp"
namespace dxle {
//!inline
namespace type_traits {
	template < typename from, typename to >
	struct is_representable
	    : std::is_same<typename common_type<from, to>::type, to >
	{ } ;
}//namespace
using namespace type_traits;
}
#endif //DXLE_INC_TYPE_TRAITS_IS_REPRESENTABLE_HPP_
