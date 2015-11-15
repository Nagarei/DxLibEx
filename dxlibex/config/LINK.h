#ifndef DXLE_LINK_H_2012015060603206321
#define DXLE_LINK_H_2012015060603206321

//TOOLSET
#  if defined(_MSC_VER) && (_MSC_VER < 1800)
// Note: no compilers before 1800 are supported
#error no compilers before 1800 are supported
#elif defined(_MSC_VER) && (_MSC_VER < 1900)
// vc12:
#	define DXLE_LIB_TOOLSET "_v120"
#elif defined(_MSC_VER)
// vc14:
#	define DXLE_LIB_TOOLSET "_v140"
#elif
//other
#	define DXLE_LIB_TOOLSET ""
#endif

#ifdef _MSC_VER
#	ifdef _WIN64
#		define DXLE_PLATFORM_TARGET "_x64"
#	else
#		define DXLE_PLATFORM_TARGET "_x86"
#	endif
#else
#	define DXLE_PLATFORM_TARGET ""
#endif

#if defined(_DEBUG) || !defined(NDEBUG)
#	define DXLE_CONFIGURATION "_d"
#else
#	define DXLE_CONFIGURATION "_r"
#endif
#ifdef _MSC_VER
#	ifdef UNICODE
#		define DXLE_CHARACTERSET "_Unicode"
#	else
#		define DXLE_CHARACTERSET "_MultiByte"
#	endif
#else
#	define DXLE_CHAR_CODE ""
#endif

#pragma comment(lib,"DXLIBEX" DXLE_LIB_TOOLSET DXLE_CHARACTERSET DXLE_PLATFORM_TARGET DXLE_CONFIGURATION ".lib")

#endif
