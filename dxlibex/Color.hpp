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
#include "dxlibex/type_traits/enable_if.hpp"
#include <cstdint>
#include <cassert>
#include <type_traits>

namespace dxle {
namespace color{

	enum class color_tag : unsigned {
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

		//\~japanese color_tagの種類の数	
		numof_variation
	};

	class color_base {};
	template<typename T>
	struct is_colortype : std::is_base_of<color_base, T>{};

	class rgb;
	class dx_color;

	class rgb final : private color_base
	{
	private:
		//like C#'s property
		struct rgb_value_t {
			DXLE_CONSTEXPR rgb_value_t()DXLE_NOEXCEPT_OR_NOTHROW :value() {}
			DXLE_CONSTEXPR rgb_value_t(int v) : value(v)
			{
#ifndef DXLE_NO_CXX14_CONSTEXPR
				assert(0 <= v && v <= 255);
#endif
			}

			DXLE_CONSTEXPR operator int()const DXLE_NOEXCEPT_OR_NOTHROW { return value; }
			DXLE_CXX14_CONSTEXPR rgb_value_t& operator=(int v){ assert(0 <= v && v <= 255); value = v; return *this; }
			DXLE_CXX14_CONSTEXPR rgb_value_t& operator=(const rgb_value_t& v)DXLE_NOEXCEPT_OR_NOTHROW { return (*this = static_cast<int>(v)); }
		private:
			int value;
		};
	public:
		rgb_value_t red;
		rgb_value_t green;
		rgb_value_t blue;

		DXLE_CONSTEXPR rgb()DXLE_NOEXCEPT_OR_NOTHROW
			: red(), green(), blue()
		{}
		DXLE_CONSTEXPR rgb(int red_, int green_, int blue_)
			: red(red_), green(green_), blue(blue_)
		{}
		DXLE_CONSTEXPR rgb(color_tag);
		rgb(dx_color)DXLE_NOEXCEPT_OR_NOTHROW;

		DXLE_CXX14_CONSTEXPR rgb& reverse()DXLE_NOEXCEPT_OR_NOTHROW {
			red = 255 - red; green = 255 - green; blue = 255 - blue;//理論上nothrow
			return *this;
		}
	};

	class dx_color final : private color_base
	{
	public:
		typedef decltype(DxLib::GetColor(0, 0, 0)) value_type;

		DXLE_CONSTEXPR dx_color()DXLE_NOEXCEPT_OR_NOTHROW
			: value(0)
		{}
		dx_color(const dx_color&) = default;
		DXLE_CONSTEXPR dx_color(dx_color&& other)DXLE_NOEXCEPT_OR_NOTHROW
			: value(static_cast<value_type&&>(other.value))
		{}
		dx_color(int Red, int Green, int Blue)
			: value(DxLib::GetColor(Red, Green, Blue))
		{
#ifndef DXLE_NO_CXX14_CONSTEXPR
			assert(0 <= Red   && Red <= 255);
			assert(0 <= Green && Green <= 255);
			assert(0 <= Blue  && Blue <= 255);
#endif
		}
		dx_color(color_tag);

		explicit dx_color(const rgb& rgb_color)DXLE_NOEXCEPT_OR_NOTHROW
			: value(DxLib::GetColor(rgb_color.red, rgb_color.green, rgb_color.blue))
		{}

		//! DrawPixel 等の描画関数で使用するカラー値を取得する
		static dx_color MakeDxColor(int Red, int Green, int Blue) { return dx_color{ Red, Green, Blue }; }
		//! DrawPixel 等の描画関数で使用するカラー値を取得する
		static dx_color GetColor(int Red, int Green, int Blue) { return MakeDxColor(Red, Green, Blue); }
		//! 指定のピクセルフォーマットに対応したカラー値を得る
		static dx_color GetColor3(const DxLib::COLORDATA *ColorData, int Red, int Green, int Blue, int Alpha = 255) { return DxLib::GetColor3(ColorData, Red, Green, Blue, Alpha); }
		//! 指定のカラーフォーマットのカラー値を別のカラーフォーマットのカラー値に変換する
		static dx_color GetColor4(const DxLib::COLORDATA *DestColorData, const DxLib::COLORDATA* SrcColorData, const dx_color& SrcColor) { return DxLib::GetColor4(DestColorData, SrcColorData, SrcColor.value); }

		//メンバ関数

		//! カラー値から赤、緑、青の値を取得する
		int GetColor2(int *Red, int *Green, int *Blue)const DXLE_NOEXCEPT_OR_NOTHROW { return GetRGB(Red, Green, Blue); }
		//! カラー値から赤、緑、青の値を取得する
		int GetRGB(int *Red, int *Green, int *Blue)const DXLE_NOEXCEPT_OR_NOTHROW { return DxLib::GetColor2(value, Red, Green, Blue); }
		//! カラー値から赤、緑、青の値を取得する
		rgb GetRGB()const DXLE_NOEXCEPT_OR_NOTHROW;
		//! 指定のカラーフォーマットのカラー値を赤、緑、青、アルファの値を取得する
		int GetColor5(const DxLib::COLORDATA *ColorData, int *Red, int *Green, int *Blue, int *Alpha = NULL)const DXLE_NOEXCEPT_OR_NOTHROW { return DxLib::GetColor5(ColorData, value, Red, Green, Blue, Alpha); }

		value_type get()const DXLE_NOEXCEPT_OR_NOTHROW { return value; }

		/*
		extern	COLOR_F			GetColorF(float Red, float Green, float Blue, float Alpha);												// 浮動小数点型のカラー値を作成する
		extern	COLOR_U8		GetColorU8(int Red, int Green, int Blue, int Alpha);														// 符号なし整数８ビットのカラー値を作成する
		extern	int				CreatePaletteColorData(DxLib::COLORDATA *ColorDataBuf);																		// パレットカラーのカラーフォーマットを構築する
		extern	int				CreateARGBF32ColorData(DxLib::COLORDATA *ColorDataBuf);																		// ＡＲＧＢ各チャンネル 32bit 浮動小数点型カラーのカラーフォーマットを構築する
		extern	int				CreateARGBF16ColorData(DxLib::COLORDATA *ColorDataBuf);																		// ＡＲＧＢ各チャンネル 16bit 浮動小数点型カラーのカラーフォーマットを構築する
		extern	int				CreateXRGB8ColorData(DxLib::COLORDATA *ColorDataBuf);																		// ＸＲＧＢ８カラーのカラーフォーマットを構築する
		extern	int				CreateARGB8ColorData(DxLib::COLORDATA *ColorDataBuf);																		// ＡＲＧＢ８カラーのカラーフォーマットを構築する
		extern	int				CreateARGB4ColorData(DxLib::COLORDATA *ColorDataBuf);																		// ＡＲＧＢ４カラーのカラーフォーマットを構築する
		extern	int				CreateFullColorData(DxLib::COLORDATA *ColorDataBuf);																		// ２４ビットカラーのカラーフォーマットを構築する
		extern	int				CreateGrayColorData(DxLib::COLORDATA *ColorDataBuf);																		// グレースケールのカラーフォーマットを構築する
		extern	int				CreatePal8ColorData(DxLib::COLORDATA *ColorDataBuf);																		// パレット２５６色のカラーフォーマットを構築する
		extern	int				CreateColorData(DxLib::COLORDATA *ColorDataBuf, int ColorBitDepth,
		DWORD RedMask, DWORD GreenMask, DWORD BlueMask, DWORD AlphaMask,
		int ChannelNum = 0, int ChannelBitDepth = 0, int FloatTypeFlag = FALSE);						// カラーフォーマットを作成する
		extern	void			SetColorDataNoneMask(DxLib::COLORDATA *ColorData);																		// NoneMask 以外の要素を埋めた DxLib::COLORDATA 構造体の情報を元に NoneMask をセットする
		extern	int				CmpColorData(const DxLib::COLORDATA *ColorData1, const DxLib::COLORDATA *ColorData2);									// 二つのカラーフォーマットが等しいかどうか調べる( 戻り値　TRUE:等しい  FALSE:等しくない )
		*/

	private:
		value_type value;
		dx_color(value_type param)DXLE_NOEXCEPT_OR_NOTHROW
			: value(param)
		{}
	};

	/**
	\~ Example:
	@code
	int MyDrawLine(int x1, int y1, int x2, int y2, dxle::dx_color_param Color, int Thickness = 1)
	{
	return DxLib::DrawLine(x1, y1, x2, y2, Color.get(), Thickness);
	}
	@endcode
	*/
	class dx_color_param final
	{
	public:
		dx_color_param(const dx_color_param& other)DXLE_NOEXCEPT_OR_NOTHROW
			: value(other.value)
		{}
		template<typename color_t, typename std::enable_if<std::is_constructible<dx_color, color_t&&>::value, std::nullptr_t>::type = nullptr>
		dx_color_param(color_t&& color) DXLE_NOEXCEPT_IF((std::is_nothrow_constructible<dx_color, color_t&&>::value))
			: value(std::forward<color_t>(color))
		{}
		dx_color::value_type get()const DXLE_NOEXCEPT_OR_NOTHROW { return value.get(); }
	private:
		dx_color value;
	};

	template<typename to, typename from, enable_if_t<is_colortype<from>::value && is_colortype<to>::value, nullptr_t> = nullptr>
	to color_cast(from&& bace)
	{
		return static_cast<to>(std::forward<from>(bace));
	}


	//定義



	//rgb

#if defined(DXLE_NO_CXX14_CONSTEXPR) && !defined(DXLE_NO_CXX11_CONSTEXPR)
	inline DXLE_CONSTEXPR rgb::rgb(color_tag param)
		: red(
			(param == color_tag::black    ) ? (  0) :
			(param == color_tag::blue     ) ? (  0) :
			(param == color_tag::cyan     ) ? (  0) :
			(param == color_tag::darkgray ) ? (169) :
			(param == color_tag::gray     ) ? (190) :
			(param == color_tag::green    ) ? (  0) :
			(param == color_tag::lightgray) ? (211) :
			(param == color_tag::magenta  ) ? (255) :
			(param == color_tag::orange   ) ? (255) :
			(param == color_tag::pink     ) ? (255) :
			(param == color_tag::red      ) ? (255) :
			(param == color_tag::white    ) ? (255) :
			(param == color_tag::yellow   ) ? (255) :
			-1//error
			)
		, green(
			(param == color_tag::black    ) ? (  0) :
			(param == color_tag::blue     ) ? (  0) :
			(param == color_tag::cyan     ) ? (255) :
			(param == color_tag::darkgray ) ? (169) :
			(param == color_tag::gray     ) ? (190) :
			(param == color_tag::green    ) ? (255) :
			(param == color_tag::lightgray) ? (211) :
			(param == color_tag::magenta  ) ? (  0) :
			(param == color_tag::orange   ) ? (165) :
			(param == color_tag::pink     ) ? (192) :
			(param == color_tag::red      ) ? (  0) :
			(param == color_tag::white    ) ? (255) :
			(param == color_tag::yellow   ) ? (255) :
			-1//error
			)
		, blue(
			(param == color_tag::black    ) ? (  0) :
			(param == color_tag::blue     ) ? (255) :
			(param == color_tag::cyan     ) ? (255) :
			(param == color_tag::darkgray ) ? (169) :
			(param == color_tag::gray     ) ? (190) :
			(param == color_tag::green    ) ? (  0) :
			(param == color_tag::lightgray) ? (211) :
			(param == color_tag::magenta  ) ? (255) :
			(param == color_tag::orange   ) ? (  0) :
			(param == color_tag::pink     ) ? (203) :
			(param == color_tag::red      ) ? (  0) :
			(param == color_tag::white    ) ? (255) :
			(param == color_tag::yellow   ) ? (  0) :
			-1//error
			)
	{
		static_assert((unsigned)color_tag::numof_variation == 13u, "color variation is not match. Please debug DxLibEx.");
	}
#else
	inline DXLE_CXX14_CONSTEXPR rgb::rgb(color_tag color_tag_)
	{
		DXLE_CONSTEXPR_OR_CONST int rgb_meta[][3] = {
			{   0,   0,   0 }, //黒
			{   0,   0, 255 }, //青
			{   0, 255, 255 }, //シアン 
			{ 169, 169, 169 }, //ダークグレー(X11)
			{ 190, 190, 190 }, //グレー
			{   0, 255,   0 }, //緑(X11)
			{ 211, 211, 211 }, //ライトグレー
			{ 255,   0, 255 }, //マゼンタ
			{ 255, 165,   0 }, //オレンジ
			{ 255, 192, 203 }, //ピンク
			{ 255,   0,   0 }, //赤
			{ 255, 255, 255 }, //白
			{ 255, 255,   0 }, //黄色
		};
		static_assert((unsigned)color_tag::numof_variation == (sizeof(rgb_meta) / sizeof(rgb_meta[0])), "color variation is not match. Please debug DxLibEx.");
		assert(0 <= (unsigned)color_tag_ && (unsigned)color_tag_ < (unsigned)color_tag::numof_variation);

		red   = rgb_meta[(unsigned)color_tag_][0];
		green = rgb_meta[(unsigned)color_tag_][1];
		blue  = rgb_meta[(unsigned)color_tag_][2];
	}
#endif
	inline rgb::rgb(dx_color dx_color_) DXLE_NOEXCEPT_OR_NOTHROW
		: rgb(dx_color_.GetRGB())
	{}

	//dx_color

	inline dx_color::dx_color(color_tag color_tag_)
		: value(static_cast<dx_color>(rgb{ color_tag_ }).get())
	{}
	//! カラー値から赤、緑、青の値を取得する
	inline rgb dx_color::GetRGB()const DXLE_NOEXCEPT_OR_NOTHROW
	{
		int r, g, b;
		dx_color::GetColor2(&r, &g, &b);
		return rgb{ r, g, b };
	}


}//namespace	
using namespace color;
}//namespace


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
