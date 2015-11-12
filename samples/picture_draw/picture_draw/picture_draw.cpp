#include <dxlibex/DxLibEx.h>

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	ChangeWindowMode(true);
	if (DxLib_Init() == -1) {// ＤＸライブラリ初期化処理
		return -1;        // エラーが起きたら直ちに終了
	}
	SetGraphMode(1000, 667, 32);
	SetDrawScreen(DX_SCREEN_BACK);
	// ＢＭＰ画像のメモリへの読みこみ
	//添付の絵素材は https://www.pakutaso.com/20150142021post-5083.html より
	auto GHandle = dxle::Graph2D::LoadGraph("bsHIRO92_tukinomieruoka.jpg");//Texture2D_Unique型

														 // 画面左上に描画します(『DrawGraph』を使用)
	GHandle.DrawGraph(0, 0, false);//これでも良い
	ScreenFlip();

	WaitKey();//キーの入力待ち(『WaitKey』を使用)
	DxLib_End();// ＤＸライブラリ使用の終了処理
	return 0;
}
