/*=============================================================================
  Copyright (C) 2015-2016 DxLibEx project
  https://github.com/Nagarei/DxLibEx/

  Distributed under the Boost Software License, Version 1.0.
  (See http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#include "dxlibex\Color.hpp"

int DrawLine(int x1, int y1, int x2, int y2, dxle::dx_color_param Color, int Thickness = 1)
{
	return DxLib::DrawLine(x1, y1, x2, y2, Color.get(), Thickness);
}
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	using namespace dxle::color;
	ChangeWindowMode(TRUE);
	if (DxLib_Init() == -1)        // ＤＸライブラリ初期化処理
	{
		return -1;        // エラーが起きたら直ちに終了
	}

	auto Cr = color_tag::green;    // 白色の値を取得

	DrawLine(0, 0, 640, 480, Cr);    // 線を描画
	DrawLine(0, 480, 640, 0, rgb{ 255, 165,   0 });    // オレンジの線を描画

	WaitKey();            // キーの入力待ち(『WaitKey』を使用)

	DxLib_End();            // ＤＸライブラリ使用の終了処理

	return 0;            // ソフトの終了
}

//for test

constexpr	dxle::color::rgb gr2{ 0,0,222 };
static_assert(gr2.red == 0, "");
static_assert(gr2.green == 0, "");
static_assert(gr2.blue == 222, "");
DXLE_CXX14_CONSTEXPR void func()
{
	using namespace dxle::color;
	constexpr rgb r(0, 0, 222);
	constexpr rgb r2(color_tag::white);
	rgb r3(color_tag::red);
	r3.red = r3.green = r3.blue;
}
