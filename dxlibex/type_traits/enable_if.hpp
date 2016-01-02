/*=============================================================================
  Copyright (C) 2015-2016 DxLibEx project
  https://github.com/Nagarei/DxLibEx/

  Distributed under the Boost Software License, Version 1.0.
  (See http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#ifndef DXLE_INC_TYPE_TRAITS_ENABLE_IF_HPP_
#define DXLE_INC_TYPE_TRAITS_ENABLE_IF_HPP_
#include <type_traits>
namespace dxle {
	using std::enable_if;
	template<bool B, typename T = void>
	using enable_if_t = typename dxle::enable_if<B, T>::type;
}
#endif //DXLE_INC_TYPE_TRAITS_ENABLE_IF_HPP_
