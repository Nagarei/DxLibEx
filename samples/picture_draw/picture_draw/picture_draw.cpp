#include <dxlibex/DxLibEx.h>

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	ChangeWindowMode(true);
	if (DxLib_Init() == -1) {//DXライブラリ初期化処理
		return -1;//エラーが起きたら直ちに終了
	}
	SetGraphMode(1000, 667, 32);
	SetDrawScreen(DX_SCREEN_BACK);
	// BMP画像のメモリへの読みこみ
	//添付の絵素材は https://www.pakutaso.com/20150142021post-5083.html より
	auto GHandle = dxle::Graph2D::LoadGraph("bsHIRO92_tukinomieruoka.jpg");//Texture2D型													 
	GHandle.DrawGraph(0, 0, false);// 画面左上に描画します(『DrawGraph』を使用)
	const auto font_h = CreateFontToHandle(nullptr, 12, 2, DX_FONTTYPE_ANTIALIASING);//フォントハンドルの作成
	DrawStringToHandle(20, 20, GHandle.GetGraphFilePath().c_str(), GetColor(250, 250, 0), font_h);//画像のフルパスを表示
	ScreenFlip();//表画面と裏画面を入れ替え

	WaitKey();//キーの入力待ち(『WaitKey』を使用)
	DxLib_End();// ＤＸライブラリ使用の終了処理
	return 0;
}
