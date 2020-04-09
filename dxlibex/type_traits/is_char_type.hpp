/*=============================================================================
  Copyright (C) 2015-2017 DxLibEx project
  https://github.com/Nagarei/DxLibEx/

  Distributed under the Boost Software License, Version 1.0.
  (See http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#ifndef DXLE_INC_TYPE_TRAITS_IS_CHAR_TYPE_HPP_
#define DXLE_INC_TYPE_TRAITS_IS_CHAR_TYPE_HPP_
#include "dxlibex/config/defines.h"
#include <type_traits>
namespace dxle {
//!inline
namespace type_traits {
    template<typename T> struct is_char_type : public std::false_type {};
    template<typename T> struct is_char_type<T const> : public type_traits::is_char_type<T> {};
    template<typename T> struct is_char_type<T volatile> : public type_traits::is_char_type<T> {};
    template<typename T> struct is_char_type<T const volatile> : public type_traits::is_char_type<T> {};
    template<> struct is_char_type<char> : public std::true_type {};
    template<> struct is_char_type<wchar_t> : public std::true_type	{};
#if DXLE_USE_UNICODE_LITERALS
    template<> struct is_char_type<char16_t> : public std::true_type {};
    template<> struct is_char_type<char32_t> : public std::true_type {};
#endif //DXLE_USE_UNICODE_LITERALS
}//namespace
using namespace type_traits;
}
#endif //DXLE_INC_TYPE_TRAITS_IS_CHAR_TYPE_HPP_
