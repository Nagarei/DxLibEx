/*=============================================================================
  Copyright (C) 2015-2016 DxLibEx project
  https://github.com/Nagarei/DxLibEx/

  Distributed under the Boost Software License, Version 1.0.
  (See http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#ifndef DXLE_INC_CONFIG_COMPILER_VISUALC_HPP_
#define DXLE_INC_CONFIG_COMPILER_VISUALC_HPP_

/*
C++11
*/
#if _MSC_FULL_VER < 190023506//Visual Studio 2015 Update 1
//Visual Studio 2013 November CTP and Visual Stduio 2015 has too many bugs for constexpr. We will never support these.
#	define DXLE_NO_CXX11_CONSTEXPR
#endif

#if _MSC_FULL_VER < 190023026//Visual Studio 2015
#	define DXLE_NO_CXX11_NOEXCEPT_EXPRESSION
#	define DXLE_NO_CXX11_USER_DEFINED_LITERALS
#	define DXLE_NO_CXX11_UNICODE_LITERALS
#	define DXLE_NO_CXX11_ATTRIBUTES
#endif
#if (_MSC_VER < 1900) && (_MSC_FULL_VER != 180021114)//Visual Studio 2013 Nobemver CTP or Visual Studio2015 or later
#	define DXLE_NO_CXX11_NOEXCEPT
#	define DXLE_NO_CXX11_REF_QUALIFIERS
#endif

/*
C++14
*/
#if !defined(__cpp_constexpr) || (__cpp_constexpr < 201304)
#  define DXLE_NO_CXX14_CONSTEXPR
#endif

#if _MSC_FULL_VER < 190023824//Visual Studio 2015 Update2 RC
#  define DXLE_NO_CXX14_VARIABLE_TEMPLATES
#endif

#if _MSC_FULL_VER < 190023026//Visual Studio 2015
#	define DXLE_NO_CXX14_ATTRIBUTE_DEPRECATED
#	define DXLE_NO_CXX14_UDLS_FOR_STRING_AND_CHRONO
#endif

/*
C++17
*/
#if _MSC_FULL_VER <= 191024629 || 201402 == _MSVC_LANG
#define DXLE_NO_CXX17_UNUSED
#endif

#endif	// #ifndef DXLE_INC_CONFIG_COMPILER_VISUALC_HPP_
