/*=============================================================================
  Copyright (C) 2015-2016 DxLibEx project
  https://github.com/Nagarei/DxLibEx/

  Distributed under the Boost Software License, Version 1.0.
  (See http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#ifndef DXLE_INC_TYPE_TRAITS_RESULT_OF_HPP_
#define DXLE_INC_TYPE_TRAITS_RESULT_OF_HPP_
#include <type_traits>
#include "enable_if.hpp"
namespace dxle {
//!inline
namespace type_traits {
	using std::result_of;
	template <class T>
	using result_of_t = typename result_of<T>::type;
}//namespace
using namespace type_traits;
}
#endif //DXLE_INC_TYPE_TRAITS_RESULT_OF_HPP_
