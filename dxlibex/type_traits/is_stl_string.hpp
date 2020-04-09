/*=============================================================================
  Copyright (C) 2015-2017 DxLibEx project
  https://github.com/Nagarei/DxLibEx/

  Distributed under the Boost Software License, Version 1.0.
  (See http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#ifndef DXLE_INC_TYPE_TRAITS_IS_STL_STRING_HPP_
#define DXLE_INC_TYPE_TRAITS_IS_STL_STRING_HPP_
#include "dxlibex/type_traits/is_char_type.hpp"
#include <type_traits>
#include <string>
namespace dxle {
//!inline
namespace type_traits {
    template<typename T>
    struct is_stl_string : std::conditional_t<std::is_reference<T>::value, is_stl_string<std::remove_reference_t<T>>, std::false_type> {};
    template<typename T> struct is_stl_string<T const> : is_stl_string<T> {};
    template<typename T> struct is_stl_string<T volatile> : is_stl_string<T> {};
    template<typename T> struct is_stl_string<T const volatile> : is_stl_string<T> {};
    template<typename CharType>
    struct is_stl_string<std::basic_string<CharType>> : std::integral_constant<bool, is_char_type<std::remove_cv_t<CharType>>::value> {};
}//namespace
using namespace type_traits;
}
#endif //DXLE_INC_TYPE_TRAITS_IS_STL_STRING_HPP_
