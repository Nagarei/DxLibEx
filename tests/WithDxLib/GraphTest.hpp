//現状テストというよりサンプル
//後で書き直す
#define DX_NON_USING_NAMESPACE_DXLIB
#include "dxlibex\Graph2D.h"
#include "dxlibex\basic_types.hpp"

int DrawGraph(const dxle::pointi& point, const dxle::texture2d& Graph, bool TransFlag)
{
	return Graph.DrawGraph(point.x, point.y, TransFlag);
}

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	using namespace dxle::graph2d;
	using dxle::sizei;
	DxLib::ChangeWindowMode(TRUE);
	if (DxLib::DxLib_Init() == -1)        // ＤＸライブラリ初期化処理
	{
		return -1;        // エラーが起きたら直ちに終了
	}

	{
		// test1.bmpの読み込み
		auto GHandle = LoadGraph("test1.bmp");

		// 読みこんだグラフィックを画面左上に描画
		GHandle.DrawGraph({ 0, 0 }, true);

		DxLib::WaitKey();
		DxLib::ClearDrawScreen();

		// 読みこんだグラフィックを画面中央に描画
		DrawGraph(dxle::pointi(sizei{640, 480} - GHandle.size()) / 2, GHandle, true);
		GHandle.DrawGraph({0.1,0.1}, true);

		DxLib::WaitKey();
		DxLib::ClearDrawScreen();
	}



	DxLib::DxLib_End();            // ＤＸライブラリ使用の終了処理

	return 0;            // ソフトの終了
}