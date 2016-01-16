/*=============================================================================
  Copyright (C) 2015-2016 DxLibEx project
  https://github.com/Nagarei/DxLibEx/

  Distributed under the Boost Software License, Version 1.0.
  (See http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#ifndef DXLE_INC_COLOR_HPP_
#define DXLE_INC_COLOR_HPP_

#include "dxlibex/config/no_min_max.h"
#include "DxLib.h"
#include "config/defines.h"
#include <cstdint>
#include <cassert>

namespace dxle {
namespace color{

	enum class color_tag {
		black    ,//!< 黒
		blue     ,//!< 青
		cyan     ,//!< シアン 
		darkgray ,//!< ダークグレー(X11)
		gray     ,//!< グレー
		green    ,//!< 緑(X11)
		lightgray,//!< ライトグレー
		magenta  ,//!< マゼンタ
		orange   ,//!< オレンジ
		pink     ,//!< ピンク
		red      ,//!< 赤
		white    ,//!< 白
		yellow   ,//!< 黄色
	};

	class rgb;
	class dx_color;

	class rgb final
	{
	private:
		struct rgb_value_t {
			DXLE_CONSTEXPR rgb_value_t(int& v) :value(v) {}
			DXLE_CONSTEXPR operator int()const DXLE_NOEXCEPT_OR_NOTHROW { return value; }
			DXLE_CXX14_CONSTEXPR rgb_value_t& operator=(int v)DXLE_NOEXCEPT_OR_NOTHROW { assert(0 <= v && v <= 255); value = v; return *this; }
			DXLE_CXX14_CONSTEXPR rgb_value_t& operator=(const rgb_value_t& v)DXLE_NOEXCEPT_OR_NOTHROW { return (*this = static_cast<int>(v)); }
		private:
			int& value;
		};
		int m_red, m_green, m_blue;
	public:
		rgb_value_t red = m_red;
		rgb_value_t green = m_green;
		rgb_value_t blue = m_blue;

		DXLE_CONSTEXPR rgb()DXLE_NOEXCEPT_OR_NOTHROW
			: m_red(), m_green(), m_blue()
		{}
		DXLE_CONSTEXPR rgb(int red_, int green_, int blue_)DXLE_NOEXCEPT_OR_NOTHROW
			: m_red(red_), m_green(green_), m_blue(blue_)
		{}
		rgb(dx_color)DXLE_NOEXCEPT_OR_NOTHROW;
	};

	class dx_color final
	{
	public:
		typedef decltype(DxLib::GetColor(0, 0, 0)) value_type;

		DXLE_CONSTEXPR dx_color()DXLE_NOEXCEPT
			: value(0)
		{}
		dx_color(int Red, int Green, int Blue)DXLE_NOEXCEPT
			: value(DxLib::GetColor(Red, Green, Blue))
		{}
		explicit dx_color(rgb rgb_color)DXLE_NOEXCEPT
			: value(DxLib::GetColor(rgb_color.red, rgb_color.green, rgb_color.blue))
		{}

		//! DrawPixel 等の描画関数で使用するカラー値を取得する
		static dx_color MakeDxColor(int Red, int Green, int Blue)DXLE_NOEXCEPT { return DxLib::GetColor(Red, Green, Blue); }
		//! DrawPixel 等の描画関数で使用するカラー値を取得する
		static dx_color GetColor(int Red, int Green, int Blue)DXLE_NOEXCEPT { return MakeDxColor(Red, Green, Blue); }
		//! 指定のピクセルフォーマットに対応したカラー値を得る
		static dx_color GetColor3(const COLORDATA *ColorData, int Red, int Green, int Blue, int Alpha = 255)DXLE_NOEXCEPT { return DxLib::GetColor3(ColorData, Red, Green, Blue, Alpha); }
		//! 指定のカラーフォーマットのカラー値を別のカラーフォーマットのカラー値に変換する
		static dx_color GetColor4(const COLORDATA *DestColorData, const COLORDATA* SrcColorData, const dx_color& SrcColor)DXLE_NOEXCEPT { return DxLib::GetColor4(DestColorData, SrcColorData, SrcColor.value); }

		//メンバ関数

		//! カラー値から赤、緑、青の値を取得する
		int GetColor2(int *Red, int *Green, int *Blue)const DXLE_NOEXCEPT { return GetRGB(Red, Green, Blue); }
		//! カラー値から赤、緑、青の値を取得する
		int GetRGB(int *Red, int *Green, int *Blue)const DXLE_NOEXCEPT { return DxLib::GetColor2(value, Red, Green, Blue); }
		//! カラー値から赤、緑、青の値を取得する
		rgb GetRGB()const DXLE_NOEXCEPT;
		//! 指定のカラーフォーマットのカラー値を赤、緑、青、アルファの値を取得する
		int GetColor5(const COLORDATA *ColorData, int *Red, int *Green, int *Blue, int *Alpha = NULL)const DXLE_NOEXCEPT { return DxLib::GetColor5(ColorData, value, Red, Green, Blue, Alpha); }

		value_type get()const DXLE_NOEXCEPT { return value; }

		/*
		extern	COLOR_F			GetColorF(float Red, float Green, float Blue, float Alpha);												// 浮動小数点型のカラー値を作成する
		extern	COLOR_U8		GetColorU8(int Red, int Green, int Blue, int Alpha);														// 符号なし整数８ビットのカラー値を作成する
		extern	int				CreatePaletteColorData(COLORDATA *ColorDataBuf);																		// パレットカラーのカラーフォーマットを構築する
		extern	int				CreateARGBF32ColorData(COLORDATA *ColorDataBuf);																		// ＡＲＧＢ各チャンネル 32bit 浮動小数点型カラーのカラーフォーマットを構築する
		extern	int				CreateARGBF16ColorData(COLORDATA *ColorDataBuf);																		// ＡＲＧＢ各チャンネル 16bit 浮動小数点型カラーのカラーフォーマットを構築する
		extern	int				CreateXRGB8ColorData(COLORDATA *ColorDataBuf);																		// ＸＲＧＢ８カラーのカラーフォーマットを構築する
		extern	int				CreateARGB8ColorData(COLORDATA *ColorDataBuf);																		// ＡＲＧＢ８カラーのカラーフォーマットを構築する
		extern	int				CreateARGB4ColorData(COLORDATA *ColorDataBuf);																		// ＡＲＧＢ４カラーのカラーフォーマットを構築する
		extern	int				CreateFullColorData(COLORDATA *ColorDataBuf);																		// ２４ビットカラーのカラーフォーマットを構築する
		extern	int				CreateGrayColorData(COLORDATA *ColorDataBuf);																		// グレースケールのカラーフォーマットを構築する
		extern	int				CreatePal8ColorData(COLORDATA *ColorDataBuf);																		// パレット２５６色のカラーフォーマットを構築する
		extern	int				CreateColorData(COLORDATA *ColorDataBuf, int ColorBitDepth,
		DWORD RedMask, DWORD GreenMask, DWORD BlueMask, DWORD AlphaMask,
		int ChannelNum = 0, int ChannelBitDepth = 0, int FloatTypeFlag = FALSE);						// カラーフォーマットを作成する
		extern	void			SetColorDataNoneMask(COLORDATA *ColorData);																		// NoneMask 以外の要素を埋めた COLORDATA 構造体の情報を元に NoneMask をセットする
		extern	int				CmpColorData(const COLORDATA *ColorData1, const COLORDATA *ColorData2);									// 二つのカラーフォーマットが等しいかどうか調べる( 戻り値　TRUE:等しい  FALSE:等しくない )
		*/

	private:
		value_type value;
		dx_color(value_type param)DXLE_NOEXCEPT
			: value(param)
		{}
	};

	template<typename to, typename from>
	to color_cast(from&& bace)
	{
		return static_cast<to>(std::forward<from>(bace));
	}


	//定義


	inline rgb::rgb(dx_color dx_color_) DXLE_NOEXCEPT_OR_NOTHROW
		: rgb(dx_color_.GetRGB())
	{}
	//! カラー値から赤、緑、青の値を取得する
	inline rgb dx_color::GetRGB()const DXLE_NOEXCEPT
	{
		int r, g, b;
		dx_color::GetColor2(&r, &g, &b);
		return rgb{ r, g, b };
	}


}}//namespace


#if 0
#include "dxlibex/config/no_min_max.h"
#include "DxLib.h"
#include "config/defines.h"

namespace dxle
{
	//! utility for Color
	//! based on X11
	struct Color
	{
		typedef decltype(DxLib::GetColor(0, 0, 0)) value_type;

		struct { inline operator Color()const DXLE_NOEXCEPT { return DxLib::GetColor(  0,   0,   0); } } static const black    ;//!< 黒
		struct { inline operator Color()const DXLE_NOEXCEPT { return DxLib::GetColor(  0,   0, 255); } } static const blue     ;//!< 青
		struct { inline operator Color()const DXLE_NOEXCEPT { return DxLib::GetColor(  0, 255, 255); } } static const cyan     ;//!< シアン 
		struct { inline operator Color()const DXLE_NOEXCEPT { return DxLib::GetColor(169, 169, 169); } } static const darkgray ;//!< ダークグレー(X11)
		struct { inline operator Color()const DXLE_NOEXCEPT { return DxLib::GetColor(190, 190, 190); } } static const gray     ;//!< グレー
		struct { inline operator Color()const DXLE_NOEXCEPT { return DxLib::GetColor(  0, 255,   0); } } static const green    ;//!< 緑(X11)
		struct { inline operator Color()const DXLE_NOEXCEPT { return DxLib::GetColor(211, 211, 211); } } static const lightgray;//!< ライトグレー
		struct { inline operator Color()const DXLE_NOEXCEPT { return DxLib::GetColor(255,   0, 255); } } static const magenta  ;//!< マゼンタ
		struct { inline operator Color()const DXLE_NOEXCEPT { return DxLib::GetColor(255, 165,   0); } } static const orange   ;//!< オレンジ
		struct { inline operator Color()const DXLE_NOEXCEPT { return DxLib::GetColor(255, 192, 203); } } static const pink     ;//!< ピンク
		struct { inline operator Color()const DXLE_NOEXCEPT { return DxLib::GetColor(255,   0,   0); } } static const red      ;//!< 赤
		struct { inline operator Color()const DXLE_NOEXCEPT { return DxLib::GetColor(255, 255, 255); } } static const white    ;//!< 白
		struct { inline operator Color()const DXLE_NOEXCEPT { return DxLib::GetColor(255, 255,   0); } } static const yellow   ;//!< 黄色

		//! DrawPixel 等の描画関数で使用するカラー値を取得する
		static Color GetColor(int Red, int Green, int Blue)DXLE_NOEXCEPT { return DxLib::GetColor(Red, Green, Blue); }
		//! 指定のピクセルフォーマットに対応したカラー値を得る
		static Color GetColor3(const COLORDATA *ColorData, int Red, int Green, int Blue, int Alpha = 255)DXLE_NOEXCEPT { return DxLib::GetColor3(ColorData, Red, Green, Blue, Alpha); }
		//! 指定のカラーフォーマットのカラー値を別のカラーフォーマットのカラー値に変換する
		static Color GetColor4(const COLORDATA *DestColorData, const COLORDATA* SrcColorData, const Color& SrcColor)DXLE_NOEXCEPT { return DxLib::GetColor4(DestColorData, SrcColorData, SrcColor.value); }

		//メンバ関数

		//! カラー値から赤、緑、青の値を取得する
		int GetColor2(int *Red, int *Green, int *Blue)const DXLE_NOEXCEPT { return DxLib::GetColor2(value, Red, Green, Blue); }
		//! 指定のカラーフォーマットのカラー値を赤、緑、青、アルファの値を取得する
		int GetColor5(const COLORDATA *ColorData, int *Red, int *Green, int *Blue, int *Alpha = NULL)const DXLE_NOEXCEPT { return DxLib::GetColor5(ColorData, value, Red, Green, Blue, Alpha); }

		value_type get()const DXLE_NOEXCEPT { return value; }

		/*
		extern	COLOR_F			GetColorF(float Red, float Green, float Blue, float Alpha);												// 浮動小数点型のカラー値を作成する
		extern	COLOR_U8		GetColorU8(int Red, int Green, int Blue, int Alpha);														// 符号なし整数８ビットのカラー値を作成する
		extern	int				CreatePaletteColorData(COLORDATA *ColorDataBuf);																		// パレットカラーのカラーフォーマットを構築する
		extern	int				CreateARGBF32ColorData(COLORDATA *ColorDataBuf);																		// ＡＲＧＢ各チャンネル 32bit 浮動小数点型カラーのカラーフォーマットを構築する
		extern	int				CreateARGBF16ColorData(COLORDATA *ColorDataBuf);																		// ＡＲＧＢ各チャンネル 16bit 浮動小数点型カラーのカラーフォーマットを構築する
		extern	int				CreateXRGB8ColorData(COLORDATA *ColorDataBuf);																		// ＸＲＧＢ８カラーのカラーフォーマットを構築する
		extern	int				CreateARGB8ColorData(COLORDATA *ColorDataBuf);																		// ＡＲＧＢ８カラーのカラーフォーマットを構築する
		extern	int				CreateARGB4ColorData(COLORDATA *ColorDataBuf);																		// ＡＲＧＢ４カラーのカラーフォーマットを構築する
		extern	int				CreateFullColorData(COLORDATA *ColorDataBuf);																		// ２４ビットカラーのカラーフォーマットを構築する
		extern	int				CreateGrayColorData(COLORDATA *ColorDataBuf);																		// グレースケールのカラーフォーマットを構築する
		extern	int				CreatePal8ColorData(COLORDATA *ColorDataBuf);																		// パレット２５６色のカラーフォーマットを構築する
		extern	int				CreateColorData(COLORDATA *ColorDataBuf, int ColorBitDepth,
			DWORD RedMask, DWORD GreenMask, DWORD BlueMask, DWORD AlphaMask,
			int ChannelNum = 0, int ChannelBitDepth = 0, int FloatTypeFlag = FALSE);						// カラーフォーマットを作成する
		extern	void			SetColorDataNoneMask(COLORDATA *ColorData);																		// NoneMask 以外の要素を埋めた COLORDATA 構造体の情報を元に NoneMask をセットする
		extern	int				CmpColorData(const COLORDATA *ColorData1, const COLORDATA *ColorData2);									// 二つのカラーフォーマットが等しいかどうか調べる( 戻り値　TRUE:等しい  FALSE:等しくない )
		*/

	private:
		value_type value;
		Color(value_type param)DXLE_NOEXCEPT
			: value(param)
		{}
	};

}//namespace dxle
#endif//#if 0 old version


#endif
