/*=============================================================================
  Copyright (C) 2015-2016 DxLibEx project
  https://github.com/Nagarei/DxLibEx/

  Distributed under the Boost Software License, Version 1.0.
  (See http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#ifndef DXLE_INC_CONFIG_COMPILER_HPP_
#define DXLE_INC_CONFIG_COMPILER_HPP_

#if defined(__clang__)
#	include "compiler/clang.hpp"
#elif defined(__INTEL_COMPILER) || defined(__ICL) || defined(__ICC) || defined(__ECC)
#	include "compiler/intel.hpp"
# elif defined(__GNUC__)
#	include "compiler/gcc.hpp"
#elif defined(_MSC_VER)
#	include "compiler/visualc.hpp"
#endif

#ifndef DXLE_NO_CXX11_USER_DEFINED_LITERALS
#define DXLE_SUPPORT_CXX11_USER_DEFINED_LITERALS
#endif
#endif	// #ifndef DXLE_INC_CONFIG_COMPILER_HPP
