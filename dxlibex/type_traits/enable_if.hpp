/*=============================================================================
  Copyright (C) 2015 DxLibEx project
  https://github.com/Nagarei/DxLibEx/

  Distributed under the Boost Software License, Version 1.0.
  (See http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#include <type_traits>
namespace dxle {
	using std::enable_if;
	template<bool B, typename T = void>
	using enable_if_t = typename dxle::enable_if<B, T>::type;
}
