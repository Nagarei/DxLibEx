#ifndef DXLIBEX_LINK_H_2012015060603206321
#define DXLIBEX_LINK_H_2012015060603206321

//TOOLSET
#  if defined(_MSC_VER) && (_MSC_VER < 1800)
// Note: no compilers before 1800 are supported
#error no compilers before 1800 are supported
#elif defined(_MSC_VER) && (_MSC_VER < 1900)
// vc12:
#	define DXLIBEX_LIB_TOOLSET "_v120"
#elif defined(_MSC_VER)
// vc14:
#	define DXLIBEX_LIB_TOOLSET "_v140"
#elif
//other
#	define DXLIBEX_LIB_TOOLSET ""
#endif

#ifdef _MSC_VER
#	ifdef _WIN64
#		define DXLIBEX_PLATFORM_TARGET "_x64"
#	else
#		define DXLIBEX_PLATFORM_TARGET "_x86"
#	endif
#else
#	define DXLIBEX_PLATFORM_TARGET ""
#endif

#if defined(_DEBUG) || !defined(NDEBUG)
#	define DXLIBEX_CONFIGURATION "_d"
#else
#	define DXLIBEX_CONFIGURATION "_r"
#endif
#ifdef _MSC_VER
#	ifdef UNICODE
#		define DXLIBEX_CHARACTERSET "_Unicode"
#	else
#		define DXLIBEX_CHARACTERSET "_MultiByte"
#	endif
#else
#	define DXLIBEX_CHAR_CODE ""
#endif

#pragma comment(lib,"DXLIBEX" DXLIBEX_LIB_TOOLSET DXLIBEX_CHARACTERSET DXLIBEX_PLATFORM_TARGET DXLIBEX_CONFIGURATION ".lib")

#endif
