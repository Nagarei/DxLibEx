/*=============================================================================
  Copyright (C) 2015-2016 DxLibEx project
  https://github.com/Nagarei/DxLibEx/

  Distributed under the Boost Software License, Version 1.0.
  (See http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#ifndef DXLE_CONFIG_COMPILER_INTEL_HPP_
#define DXLE_CONFIG_COMPILER_INTEL_HPP_

#include "common_edg.hpp"

#if defined(__INTEL_COMPILER)
#	define DXLE_INTEL_CXX_VERSION __INTEL_COMPILER
#elif defined(__ICL)
#	define DXLE_INTEL_CXX_VERSION __ICL
#elif defined(__ICC)
#	define DXLE_INTEL_CXX_VERSION __ICC
#elif defined(__ECC)
#	define DXLE_INTEL_CXX_VERSION __ECC
#endif

#if (!(defined(_WIN32) || defined(_WIN64)) && defined(__STDC_HOSTED__) && (__STDC_HOSTED__ && (DXLE_INTEL_CXX_VERSION <= 1200))) || defined(__GXX_EXPERIMENTAL_CPP0X__) || defined(__GXX_EXPERIMENTAL_CXX0X__)
#	define DXLE_INTEL_STDCXX11
#endif
#if defined(_MSC_VER) && (_MSC_VER >= 1600)
#	define DXLE_INTEL_STDCXX11
#endif

#if defined(SPROUT_INTEL_STDCXX11) && (SPROUT_INTEL_CXX_VERSION >= 1200)
#	undef DXLE_NO_CXX11_DELETED_FUNCTIONS
#	undef DXLE_NO_CXX11_DEFAULTED_FUNCTIONS
#endif

#if defined(SPROUT_INTEL_STDCXX11) && (SPROUT_INTEL_CXX_VERSION > 1200)
#	undef DXLE_NO_CXX11_TEMPLATE_ALIASES
#endif

#if defined(SPROUT_INTEL_STDCXX11) && (SPROUT_INTEL_CXX_VERSION >= 1400) && !defined(_MSC_VER)
#	undef DXLE_NO_CXX11_UNICODE_LITERALS
#	undef DXLE_NO_CXX11_CONSTEXPR
#endif

#if defined(_MSC_VER) && (_MSC_VER <= 1700)
#	define DXLE_NO_CXX11_DELETED_FUNCTIONS
#	define DXLE_NO_CXX11_DEFAULTED_FUNCTIONS
#	define DXLE_NO_CXX11_TEMPLATE_ALIASES
#endif

#endif	// #ifndef DXLE_CONFIG_COMPILER_INTEL_HPP_
