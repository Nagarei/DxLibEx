/*=============================================================================
  Copyright (C) 2015-2016 DxLibEx project
  https://github.com/Nagarei/DxLibEx/

  Distributed under the Boost Software License, Version 1.0.
  (See http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#include "dxlibex\Color.hpp"
#if 0
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
#if !defined(_MSC_VER) || (defined(_MSC_VER) && _MSC_VER >= 1900)

constexpr dxle::color::rgb gr2{ 0,0,222 };
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
#endif
#endif
#include "dxlibex/DxLibEx.h"
namespace dxle {
	int DrawBox(const dxle::pointi& pos1, const dxle::pointi& pos2, dxle::dx_color_param Color, bool FillFlag)
	{
		return DxLib::DrawBox(pos1.x, pos1.y, pos2.x, pos2.y, Color.get(), FillFlag);
	}
	typedef dxle::pointi vec2di;
}


int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	using dxle::color::color_tag;

	ChangeWindowMode(TRUE);
	if (DxLib_Init() == -1)        // ＤＸライブラリ初期化処理
	{
		return -1;        // エラーが起きたら直ちに終了
	}
	// 描画先画面を裏画面にする
	SetDrawScreen(DX_SCREEN_BACK);

	// プレイヤーの初期位置をセット
	dxle::pointi player{ 320,240 };
	const dxle::sizei player_size{ 40,40 };

	// ループ
	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
	{
		// 画面を初期化
		ClearDrawScreen();

		// キー入力を得る
		int Key = GetJoypadInputState(DX_INPUT_KEY_PAD1);

		// キー入力に応じてプレイヤーの座標を移動
		const int MOVE_SPEED = 5;
		dxle::vec2di move_vec{ 0,0 };
		if (Key & PAD_INPUT_LEFT ){ move_vec.x -= 1; }
		if (Key & PAD_INPUT_RIGHT){ move_vec.x += 1; }
		if (Key & PAD_INPUT_UP   ){ move_vec.y -= 1; }
		if (Key & PAD_INPUT_DOWN ){ move_vec.y += 1; }
		move_vec *= MOVE_SPEED;
		player += move_vec;

		// プレイヤーの描画
		dxle::DrawBox(player, player + player_size, color_tag::red, TRUE);

		// 裏画面の内容を表画面に映す
		ScreenFlip();
	}

	DxLib_End();				// ＤＸライブラリ使用の終了処理

	return 0;            // ソフトの終了
}