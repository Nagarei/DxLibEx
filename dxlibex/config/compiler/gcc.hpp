/*=============================================================================
  Copyright (C) 2015-2016 DxLibEx project
  https://github.com/Nagarei/DxLibEx/

  Distributed under the Boost Software License, Version 1.0.
  (See http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#ifndef DXLE_INC_CONFIG_COMPILER_GCC_HPP_
#define DXLE_INC_CONFIG_COMPILER_GCC_HPP_

#if (__GNUC__ < 4 || (__GNUC__ == 4 && __GNUC_MINOR__ < 6) || !defined(__GXX_EXPERIMENTAL_CXX0X__))
#	define DXLE_NO_CXX11_CONSTEXPR
#	define DXLE_NO_CXX11_CONSTEXPR_CLASS
#endif

#if (__GNUC__ < 4 || (__GNUC__ == 4 && __GNUC_MINOR__ < 6) || !defined(__GXX_EXPERIMENTAL_CXX0X__))
#	define DXLE_NO_CXX11_NOEXCEPT
#	define DXLE_NO_CXX11_NOEXCEPT_EXPRESSION
#endif

#if (__GNUC__ < 4 || (__GNUC__ == 4 && __GNUC_MINOR__ < 5) || !defined(__GXX_EXPERIMENTAL_CXX0X__))
#	define DXLE_NO_CXX11_UNICODE_LITERALS
#endif

#if ((__GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 6)) && defined(__GXX_EXPERIMENTAL_CXX0X__))
#	define DXLE_HAS_CONSTEXPR_CMATH_FUNCTION
#endif

#if ((__GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 6)) && defined(__GXX_EXPERIMENTAL_CXX0X__))
#	define DXLE_HAS_CONSTEXPR_COPYSIGN_FUNCTION
#endif

#if ((__GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 6)) && defined(__GXX_EXPERIMENTAL_CXX0X__))
#	define DXLE_HAS_CONSTEXPR_BIT_OPERATION
#endif

#ifndef __cpp_user_defined_literals
#define DXLE_NO_CXX11_USER_DEFINED_LITERALS
#endif

#if !defined(__cpp_constexpr) || (__cpp_constexpr < 201304)
#  define DXLE_NO_CXX14_CONSTEXPR
#endif

#endif	// #ifndef DXLE_INC_CONFIG_COMPILER_GCC_HPP_
