/*=============================================================================
  Copyright (C) 2015 DxLibEx project
  https://github.com/Nagarei/DxLibEx/

  Distributed under the Boost Software License, Version 1.0.
  (See http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#include <type_traits>
namespace dxle {
	/**
	@brief	\~japanese 複数条件のあるTMP用if
	\~english multi-conditional if struct for TMP.
	\~japanese	std::enable_ifと組み合わせて使います。
	\~english	This class is used in conjunction with std::enable_if.
	\~
	@code
	template<typename T>
	using bar = first_enabled_t<
		std::enbale_if<cond1, type1>,
		std::enbale_if<cond2, type2>,
		std::enbale_if<cond3, type3>,
		default_type
	>;
	@endcode
	*/
	template<typename ...Args>
	struct first_enabled {};

	template<typename T, typename ...Args>
	struct first_enabled<std::enable_if<true, T>, Args...> { using type = T; };
	template<typename T, typename ...Args>
	struct first_enabled<std::enable_if<false, T>, Args...> : first_enabled<Args...> {};
	template<typename T, typename ...Args>
	struct first_enabled<T, Args...> { using type = T; };

	template<typename ...Args>
	using first_enabled_t = typename first_enabled<Args...>::type;
}
