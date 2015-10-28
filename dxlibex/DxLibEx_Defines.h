#ifndef DX_LIB_EX_DEFINES_H_2015_10_11_1714_185961646566518
#define DX_LIB_EX_DEFINES_H_2015_10_11_1714_185961646566518

//実装用ファイルです
//開発者以外がここの機能を使うのはお勧めできません

//ここのdefineは完全ではありません

#ifdef _MSC_VER
#if (_MSC_VER >= 1900)
	//VC2015
#	define DXLIBEX_NOEXCEPT noexcept
#	define DXLIBEX_CONSTEXPR constexpr
#	define DXLIBEX_SFINAE template<typename = void>
#
#elif (_MSC_VER >= 1700)
	//VC2013,VC2012
#	define DXLIBEX_NOEXCEPT throw()
#	define DXLIBEX_CONSTEXPR
#	define DXLIBEX_SFINAE
#
#endif
//#ifdef _MSC_VER
#else
#	define DXLIBEX_NOEXCEPT noexcept
#	define DXLIBEX_CONSTEXPR constexpr
#	define DXLIBEX_SFINAE template<typename = void>
#endif

//インクルードガード
#endif
