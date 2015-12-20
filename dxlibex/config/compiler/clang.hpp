/*=============================================================================
  Copyright (C) 2015 DxLibEx project
  https://github.com/Nagarei/DxLibEx/

  Distributed under the Boost Software License, Version 1.0.
  (See http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#ifndef DXLE_INC_CONFIG_COMPILER_CLANG_HPP_
#define DXLE_INC_CONFIG_COMPILER_CLANG_HPP_

#if !__has_feature(cxx_constexpr)
#	define DXLE_NO_CXX11_CONSTEXPR
#	define DXLE_NO_CXX11_CONSTEXPR_CLASS
#endif

#if !__has_feature(cxx_noexcept)
#	define DXLE_NO_CXX11_NOEXCEPT
#	define DXLE_NO_CXX11_NOEXCEPT_EXPRESSION
#endif

#if !__has_feature(cxx_unicode_literals)
#	define DXLE_NO_CXX11_UNICODE_LITERALS
#endif

#if !__has_feature(cxx_relaxed_constexpr)
#	define DXLE_NO_CXX14_CONSTEXPR
#endif

#if !__has_feature(cxx_variable_templates)
#	define DXLE_NO_CXX14_VARIABLE_TEMPLATES
#endif

#if (__clang_major__ > 3 || (__clang_major__ == 3 && __clang_minor__ >= 0))
#	define DXLE_HAS_CONSTEXPR_COPYSIGN_FUNCTION
#endif

#endif	// #ifndef DXLE_INC_CONFIG_COMPILER_CLANG_HPP_
