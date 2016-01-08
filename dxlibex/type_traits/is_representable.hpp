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
	template < typename from, typename to >
	struct is_representable
	    : std::integral_constant<bool, std::is_same<common_type_t<from, to>, to >::value >
	{ } ;
}
#endif //DXLE_INC_TYPE_TRAITS_IS_REPRESENTABLE_HPP_
