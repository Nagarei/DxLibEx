/*=============================================================================
  Copyright (C) 2015-2017 DxLibEx project
  https://github.com/Nagarei/DxLibEx/

  Distributed under the Boost Software License, Version 1.0.
  (See http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#ifndef DXLE_INC_TYPE_TRAITS_IS_C_STR_HPP_
#define DXLE_INC_TYPE_TRAITS_IS_C_STR_HPP_
#include "dxlibex/type_traits/is_char_type.hpp"
#include <type_traits>
namespace dxle {
//!inline
namespace type_traits {
    template<typename T>
    struct is_c_str : std::integral_constant<bool, std::is_pointer<T>::value && dxle::type_traits::is_char_type<std::remove_pointer_t<T>>::value> {
        using char_type = std::remove_cv_t<std::remove_pointer_t<T>>;
    };
    template<typename T, std::size_t N> struct is_c_str<T[N]> : std::integral_constant<bool, is_char_type<T>::value> {
        using char_type = std::remove_cv_t<T>;
    };
}//namespace
using namespace type_traits;
}
#endif //DXLE_INC_TYPE_TRAITS_IS_C_STR_HPP_
