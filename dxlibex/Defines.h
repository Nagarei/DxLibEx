#ifndef DX_LIB_EX_DEFINES_H_2015_10_11_1714_185961646566518
#define DX_LIB_EX_DEFINES_H_2015_10_11_1714_185961646566518

//実装用ファイルです
//開発者以外がここの機能を使うのはお勧めできません

//ここのdefineは完全ではありません
#if defined(__INTEL_COMPILER)
#	define DXLIBEX_INTEL_CXX_VERSION __INTEL_COMPILER
#elif defined(__ICL)
#	define DXLIBEX_INTEL_CXX_VERSION __ICL
#elif defined(__ICC)
#	define DXLIBEX_INTEL_CXX_VERSION __ICC
#elif defined(__ECC)
#	define DXLIBEX_INTEL_CXX_VERSION __ECC
#endif
#if (!(defined(_WIN32) || defined(_WIN64)) && defined(__STDC_HOSTED__) && (__STDC_HOSTED__ && (DXLIBEX_INTEL_CXX_VERSION <= 1200))) || defined(__GXX_EXPERIMENTAL_CPP0X__) || defined(__GXX_EXPERIMENTAL_CXX0X__)
#	define DXLIBEX_INTEL_STDCXX11
#endif
#if defined(_MSC_VER) && (_MSC_VER >= 1600)
#	define DXLIBEX_INTEL_STDCXX11
#endif


#ifdef _MSC_VER
#	if (_MSC_FULL_VER >= 190023026)
		//VC2015
#		define DXLIBEX_NOEXCEPT noexcept
#		define DXLIBEX_CONSTEXPR constexpr
#	elif (_MSC_VER >= 1700)
		//VC2013,VC2012
#		define DXLIBEX_NOEXCEPT throw()
#		define DXLIBEX_CONSTEXPR
#	endif
//#ifdef _MSC_VER
#else
#	if defined (_clang__)
#		if __has_feature(cxx_noexcept)
#			define DXLIBEX_NOEXCEPT noexcept
#		endif
#		if __has_feature(cxx_constexpr)
#			define DXLIBEX_CONSTEXPR constexpr
#		endif
#	elif defined(SPROUT_INTEL_CXX_VERSION)
#		if defined(SPROUT_INTEL_STDCXX11) && (SPROUT_INTEL_CXX_VERSION >= 1400) && !defined(_MSC_VER)
#			define DXLIBEX_CONSTEXPR constexpr
#		endif
#	elif defined(__GNUC__)
#		if (__GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 6) || defined(__GXX_EXPERIMENTAL_CXX0X__))
#			define DXLIBEX_CONSTEXPR constexpr
#			define DXLIBEX_NOEXCEPT noexcept
#		endif
#	endif
#endif

//インクルードガード
#endif
