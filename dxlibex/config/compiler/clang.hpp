/*=============================================================================
  Copyright (C) 2015-2016 DxLibEx project
  https://github.com/Nagarei/DxLibEx/

  Distributed under the Boost Software License, Version 1.0.
  (See http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#ifndef DXLE_INC_CONFIG_COMPILER_CLANG_HPP_
#define DXLE_INC_CONFIG_COMPILER_CLANG_HPP_

/*
C++11
*/
#if !__has_feature(cxx_constexpr)
#	define DXLE_NO_CXX11_CONSTEXPR
#endif

#if !__has_feature(cxx_noexcept)
#	define DXLE_NO_CXX11_NOEXCEPT
#	define DXLE_NO_CXX11_NOEXCEPT_EXPRESSION
#endif

#if !__has_feature(cxx_unicode_literals)
#	define DXLE_NO_CXX11_UNICODE_LITERALS
#endif

#if __clang_major__ < 3 || (__clang_major__ == 3 && __clang_minor__ < 3)
#	define DXLE_NO_CXX11_ATTRIBUTES
#endif

#if __clang_major__ < 3 || (__clang_major__ == 3 && __clang_minor__ < 1)
#	define DXLE_NO_CXX11_USER_DEFINED_LITERALS
#endif

#if __clang_major__ < 2 || (__clang_major__ == 2 && __clang_minor__ < 9)
#	define DXLE_NO_CXX11_REF_QUALIFIERS
#endif

/*
C++14
*/
#if !__has_feature(cxx_relaxed_constexpr)
#	define DXLE_NO_CXX14_CONSTEXPR
#endif

#if !__has_feature(cxx_variable_templates)
#	define DXLE_NO_CXX14_VARIABLE_TEMPLATES
#endif

#if __clang_major__ < 3 || (__clang_major__ == 3 && __clang_minor__ < 4)
#	if __clang_major__ == 3 && __clang_minor__ < 7
#		if __cplusplus < 201103//clang3.6までは-std=c++11でも使える模様
#			define DXLE_NO_CXX14_ATTRIBUTE_DEPRECATED
#		endif
#	else//warning: use of the 'deprecated' attribute is a C++14 extension [-Wc++14-extensions]
#		if __cplusplus < 201402//clang3.7からは-std=c++14が必要
#			define DXLE_NO_CXX14_ATTRIBUTE_DEPRECATED
#		endif
#	endif
#	if __cplusplus < 201305//-std=c++1yが必要
#		define DXLE_NO_CXX14_UDLS_FOR_STRING_AND_CHRONO
#	endif
#endif

/*
Proprietary extension
*/
#if (__clang_major__ > 3 || (__clang_major__ == 3 && __clang_minor__ >= 0))
#	define DXLE_HAS_CONSTEXPR_COPYSIGN_FUNCTION
#endif
#if (__clang_major__ > 3 || (__clang_major__ == 3 && __clang_minor__ >= 3))//3.2以前はドキュメントが見つからない
#define DXLE_HAS_GNU_DEPRECATED_WITH_MESSAGE_SUPPORT
#endif


/*
C++17
*/
#if __clang_major < 3 || (__clang_major__ == 3 && __clang_minor__ < 9) || __has_cpp_attribute(maybe_unused) < 201603
#	define DXLE_NO_CXX17_UNUSED
#endif

#endif	// #ifndef DXLE_INC_CONFIG_COMPILER_CLANG_HPP_
