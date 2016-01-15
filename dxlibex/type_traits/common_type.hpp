/*=============================================================================
  Copyright (C) 2015-2016 DxLibEx project
  https://github.com/Nagarei/DxLibEx/

  Distributed under the Boost Software License, Version 1.0.
  (See http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#ifndef DXLE_INC_TYPE_TRAITS_COMMON_TYPE_HPP_
#define DXLE_INC_TYPE_TRAITS_COMMON_TYPE_HPP_
#include <type_traits>
namespace dxle {
//!inline
namespace type_traits {
	using std::common_type;
	template<typename... T> using common_type_t = typename common_type<T...>::type;
}//namespace
using namespace type_traits;
}
#endif //DXLE_INC_TYPE_TRAITS_COMMON_TYPE_HPP_
