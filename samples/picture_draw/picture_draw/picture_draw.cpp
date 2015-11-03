#include <dxlibex/DxLibEx.h>

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	ChangeWindowMode(true);
	if (DxLib_Init() == -1) {// ＤＸライブラリ初期化処理
		return -1;        // エラーが起きたら直ちに終了
	}

	// ＢＭＰ画像のメモリへの読みこみ
	auto GHandle = dxle::Graph2D::LoadGraph("test1.bmp");//Texture2D_Unique型

														 // 画面左上に描画します(『DrawGraph』を使用)
	GHandle.DrawGraph(0, 0, false);//これでも良い

	WaitKey();//キーの入力待ち(『WaitKey』を使用)
	DxLib_End();// ＤＸライブラリ使用の終了処理
	return 0;
}
