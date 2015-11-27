#ifndef DXLE_INC_CONFIG_COMPILER_HPP
#define DXLE_INC_CONFIG_COMPILER_HPP

#if defined(__clang__)
#	include "compiler/clang.hpp"
#elif defined(__INTEL_COMPILER) || defined(__ICL) || defined(__ICC) || defined(__ECC)
#	include "compiler/intel.hpp"
# elif defined(__GNUC__)
#	include "compiler/gcc.hpp"
#elif defined(_MSC_VER)
#	include "compiler/visualc.hpp"
#endif

#endif	// #ifndef DXLE_INC_CONFIG_COMPILER_HPP
