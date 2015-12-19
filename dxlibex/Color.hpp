/*=============================================================================
  Copyright (C) 2015 DxLibEx project
  https://github.com/Nagarei/DxLibEx/

  Distributed under the Boost Software License, Version 1.0.
  (See http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#ifndef DXLE_INC_COLOR_HPP_
#define DXLE_INC_COLOR_HPP_

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

		struct { inline operator Color()const DXLE_NOEXCEPT { return DxLib::GetColor(0, 0, 0); } } static const black;//!< 黒
		struct { inline operator Color()const DXLE_NOEXCEPT { return DxLib::GetColor(0, 0, 255); } } static const blue;//!< 青
		struct { inline operator Color()const DXLE_NOEXCEPT { return DxLib::GetColor(0, 255, 255); } } static const cyan;//!< シアン 
		struct { inline operator Color()const DXLE_NOEXCEPT { return DxLib::GetColor(169, 169, 169); } } static const darkgray;//!< ダークグレー(X11)
		struct { inline operator Color()const DXLE_NOEXCEPT { return DxLib::GetColor(190, 190, 190); } } static const gray;//!< グレー
		struct { inline operator Color()const DXLE_NOEXCEPT { return DxLib::GetColor(0, 255, 0); } } static const green;//!< 緑(X11)
		struct { inline operator Color()const DXLE_NOEXCEPT { return DxLib::GetColor(211, 211, 211); } } static const lightgray;//!< ライトグレー
		struct { inline operator Color()const DXLE_NOEXCEPT { return DxLib::GetColor(255, 0, 255); } } static const magenta;//!< マゼンタ
		struct { inline operator Color()const DXLE_NOEXCEPT { return DxLib::GetColor(255, 165, 0); } } static const orange;//!< オレンジ
		struct { inline operator Color()const DXLE_NOEXCEPT { return DxLib::GetColor(255, 192, 203); } } static const pink;//!< ピンク
		struct { inline operator Color()const DXLE_NOEXCEPT { return DxLib::GetColor(255, 0, 0); } } static const red;//!< 赤
		struct { inline operator Color()const DXLE_NOEXCEPT { return DxLib::GetColor(255, 255, 255); } } static const white;//!< 白
		struct { inline operator Color()const DXLE_NOEXCEPT { return DxLib::GetColor(255, 255, 0); } } static const yellow;//!< 黄色

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
#endif
