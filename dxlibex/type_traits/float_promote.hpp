/*=============================================================================
  Copyright (C) 2015-2016 DxLibEx project
  https://github.com/Nagarei/DxLibEx/

  Distributed under the Boost Software License, Version 1.0.
  (See http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#ifndef DXLE_TYPE_TRAITS_FLOAT_PROMOTE_HPP
#define DXLE_TYPE_TRAITS_FLOAT_PROMOTE_HPP

#include <type_traits>
#include "dxlibex/config/defines.h"
#include "dxlibex/type_traits/first_enabled.hpp"

namespace dxle {
	namespace detail {
		template<typename T> struct float_promote1 : public std::conditional<std::is_floating_point<T>::value, T, double>
		{
			static_assert(
				std::is_arithmetic<T>::value,
				"float_promote requires arithmetic type."
			);
		};
		template<typename T, typename U, bool is_arithmetic = std::is_arithmetic<T>::value && std::is_arithmetic<U>::value>
		struct float_promote2 {
			static_assert(
				std::is_arithmetic<T>::value && std::is_arithmetic<U>::value,
				"float_promote requires arithmetic type."
				);
		};
		template<typename T, typename U>
		struct float_promote2<T, U, true>
		{
			using type = first_enabled_t<
				enable_if<(std::is_same<T, long double>::value || std::is_same<U, long double>::value), long double>,
				enable_if<(std::is_same<T, float>::value && std::is_same<U, float>::value), float>,
				double
			>;
		};

		template<typename... Types> struct float_promote_impl;
		template<typename T, typename U, typename... Tail>
		struct float_promote_impl<T, U, Tail...>
			: public dxle::detail::float_promote_impl<
				typename dxle::detail::float_promote2<T, U>::type,
				Tail...
			>
		{};
		template<typename T>
		struct float_promote_impl<T>
			: public dxle::detail::float_promote1<T>
		{};
	}	// namespace detail

	//
	// float_promote
	//
	template<typename... Types>
	struct float_promote
		: public dxle::detail::float_promote_impl<
			typename std::remove_cv<Types>::type...
		>
	{};

	template<typename... Types>
	using float_promote_t = typename dxle::float_promote<Types...>::type;
}	// namespace dxle

#endif	// #ifndef DXLE_TYPE_TRAITS_FLOAT_PROMOTE_HPP
