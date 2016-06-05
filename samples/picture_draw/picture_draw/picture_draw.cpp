/*=============================================================================
  Copyright (C) 2015-2016 DxLibEx project
  https://github.com/Nagarei/DxLibEx/

  Distributed under the Boost Software License, Version 1.0.
  (See http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#include "dxlibex/dxlibex.hpp"

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	ChangeWindowMode(true);
	if (DxLib_Init() == -1) {//DXライブラリ初期化処理
		return -1;//エラーが起きたら直ちに終了
	}
	SetGraphMode(1000, 667, 32);
	SetDrawScreen(DX_SCREEN_BACK);
	// BMP画像のメモリへの読みこみ
	//添付の絵素材は https://www.pakutaso.com/20150142021post-5083.html より
	auto GHandle = dxle::graph2d::LoadGraph(_T("bsHIRO92_tukinomieruoka.jpg"));//Texture2D型
	GHandle.DrawGraph(0, 0, false);// 画面左上に描画します(『DrawGraph』を使用)
	const auto font_h = CreateFontToHandle(nullptr, 12, 2, DX_FONTTYPE_ANTIALIASING);//フォントハンドルの作成
	DrawStringToHandle(20, 20, GHandle.GetGraphFilePath().c_str(), GetColor(250, 250, 0), font_h);//画像のフルパスを表示
	ScreenFlip();//表画面と裏画面を入れ替え
	WaitKey();//キーの入力待ち(『WaitKey』を使用)
	auto sc = dxle::MakeScreen(1000, 667);
	sc.draw_on_this([&GHandle, font_h]() {
		GHandle.DrawGraph(0, 0, false);// 画面左上に描画します(『DrawGraph』を使用)
		DrawStringToHandle(20, 20, (GHandle.GetGraphFilePath() + _T(" on screen.")).c_str(), GetColor(250, 250, 0), font_h);//画像のフルパスを表示
	})
	.DrawGraph({}, false);
	ScreenFlip();//表画面と裏画面を入れ替え
	WaitKey();//キーの入力待ち(『WaitKey』を使用)

	{
		// 一定時間待つ(『WaitTimer』使用)
		DxLib::WaitTimer(100);
		//DxLibのLoadDivGraphのsampleより

		SetDrawScreen(DX_SCREEN_BACK);
		// ＢＭＰ画像のメモリへの分割読み込み
		dxle::derivative_texture2d div_graph(_T("test2.bmp"), 10, { 4, 3 }, { 48, 56 });
		int i = 0;

		// ロードしたグラフィックのアニメーション
		// キーが押されるまでループ(キー判定には『CheckHitKeyAll』を使用)
		while (DxLib::CheckHitKeyAll() == 0)
		{
			ScreenFlip();
			ClearDrawScreen();

			// グラフィックの描画(『DrawGraph』使用)
			//dxle::DrawGraph({ 0, 0 }, div_graph[i], false);
			div_graph[i].DrawGraph({ 0, 0 }, false);

			// アニメーションパターンナンバーを変更
			i++;
			if (i == 10) i = 0;

			// 一定時間待つ(『WaitTimer』使用)
			DxLib::WaitTimer(100);

			// メッセージ処理
			if (DxLib::ProcessMessage() == -1)
			{
				break;        // エラーが起きたらループから抜ける
			}
		}
	}

	DxLib_End();// ＤＸライブラリ使用の終了処理
	return 0;
}
