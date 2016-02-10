/*=============================================================================
  Copyright (C) 2015-2016 DxLibEx project
  https://github.com/Nagarei/DxLibEx/

  Distributed under the Boost Software License, Version 1.0.
  (See http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#ifndef DXLE_INC_TYPE_TRAITS_IGNORE_HPP_
#define DXLE_INC_TYPE_TRAITS_IGNORE_HPP_
#include <type_traits>

namespace dxle {
//!inline
namespace type_traits{

	/**
	@brief
	\~japanese 渡された型にかかわらず常にtrueなvalueを持つクラス。
	\~english Trait class whose `value` is true regardless of what `T` is.
	\~japanese	decltype式にSFINAEを効かせたいときに使います。
	\~english	we can use it, when we want to let SFINAE work for a decltype expressions.
	\~
	@code
	template<typename T>
	//return int
	auto add(int a, T b)->std::enable_if_t<dxle::ignore_type<decltype(a + b)>::value, int>
	{
		return a + b;
	}

	int main()
	{
		add(0, 0);//OK
		//add(0, nullptr);//compile error
		return 0;
	}
	@endcode
	*/
	template<typename>
	struct ignore_type : std::true_type {};

	template<typename T, typename...>
	struct first_type {
		typedef T type;
	};
	template<typename T, typename... args>
	using first_type_t = first_type<T, args...>;

}//namespace
using namespace type_traits;
}//namespace
#endif //DXLE_INC_TYPE_TRAITS_IGNORE_HPP_
