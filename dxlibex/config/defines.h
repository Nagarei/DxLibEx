#ifndef DXLE_INC_CONFIG_DEFINES_H_
#define DXLE_INC_CONFIG_DEFINES_H_
#include "dxlibex/config/no_min_max.h"
#include "suffix.hpp"
#if 0
//実装用ファイルです
//開発者以外がここの機能を使うのはお勧めできません

//ここのdefineは完全ではありません
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


#ifdef _MSC_VER
#	if (_MSC_FULL_VER >= 190023026)
		//VC2015
#		define DXLE_NOEXCEPT noexcept
#		define DXLE_CONSTEXPR constexpr
#	elif (_MSC_VER >= 1700)
		//VC2013,VC2012
#		define DXLE_NOEXCEPT throw()
#		define DXLE_CONSTEXPR
#	endif
//#ifdef _MSC_VER
#else
#	if defined (_clang__)
#		if __has_feature(cxx_noexcept)
#			define DXLE_NOEXCEPT noexcept
#		endif
#		if __has_feature(cxx_constexpr)
#			define DXLE_CONSTEXPR constexpr
#		endif
#	elif defined(SPROUT_INTEL_CXX_VERSION)
#		if defined(SPROUT_INTEL_STDCXX11) && (SPROUT_INTEL_CXX_VERSION >= 1400) && !defined(_MSC_VER)
#			define DXLE_CONSTEXPR constexpr
#		endif
#	elif defined(__GNUC__)
#		if (__GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 6) || defined(__GXX_EXPERIMENTAL_CXX0X__))
#			define DXLE_CONSTEXPR constexpr
#			define DXLE_NOEXCEPT noexcept
#		endif
#	endif
#endif

//インクルードガード
#endif
#endif
