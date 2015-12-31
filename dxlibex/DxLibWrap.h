/*=============================================================================
  Copyright (C) 2015-2016 DxLibEx project
https://github.com/Nagarei/DxLibEx/

Distributed under the Boost Software License, Version 1.0.
(See http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
/**
* \~english  this is thread-safe wrapper of DxLib's functions
* \~japanese DxLibの関数のスレッドセーフなラッパー
* \~
*
* <a href="http://homepage2.nifty.com/natupaji/DxLib/dxfunc.html">ＤＸライブラリ　関数リファレンスページ</a>
*/

#ifndef DXLE_INC_DXLIBWRAP_H_
#define DXLE_INC_DXLIBWRAP_H_
#include "dxlibex/config/no_min_max.h"
#include "Graph2D.h"
#include "dxlibex/thread.h"
namespace dxle {
//置換用正規表現（Visual Studio用）
//^(?<indent>\s*)extern\s+(?<ReturnType>\w+)\s+(?<FunctionName>\w+)\((?<Arguments>[^)]*)\)\s*;\s*//(?<Comment>.+)$
//${indent}//!\~japanese${Comment}\n${indent}inline ${ReturnType} ${FunctionName}(dxle_mutex, ${Arguments}){ return DxLib::${FunctionName}(${Arguments}); }
#define DX_THREAD_SAFE
static_assert(false, "実装用define");
#ifdef DX_THREAD_SAFE

	enum class dxle_mutex { dxle_mutex };

	//使用必須関数

	int DxLib_Init(dxle_mutex) { return DxLib::DxLib_Init(); }
	int DxLib_End(dxle_mutex) { return DxLib::DxLib_End(); }
	int ProcessMessage(dxle_mutex) { return DxLib::ProcessMessage(); }

	//３Ｄ関係関数

	//図形描画関数

	//!\~japanese 線を描画する
	inline int DrawLine(dxle_mutex, int x1, int y1, int x2, int y2, unsigned int Color, int Thickness = 1){ DXLE_GET_LOCK(dxle::Graph2D::screen_mutex_c::mtx);  return DxLib::DrawLine(x1, y1, x2, y2, Color, Thickness); }
	//!\~japanese 四角形の描画する
	inline int DrawBox(dxle_mutex, int x1, int y1, int x2, int y2, unsigned int Color, int FillFlag){ DXLE_GET_LOCK(dxle::Graph2D::screen_mutex_c::mtx);  return DxLib::DrawBox(x1, y1, x2, y2, Color, FillFlag); }
	//!\~japanese 中身を塗りつぶす四角形を描画する
	inline int DrawFillBox(dxle_mutex, int x1, int y1, int x2, int y2, unsigned int Color){ DXLE_GET_LOCK(dxle::Graph2D::screen_mutex_c::mtx);  return DxLib::DrawFillBox(x1, y1, x2, y2, Color); }
	//!\~japanese 枠だけの四角形の描画 する
	inline int DrawLineBox(dxle_mutex, int x1, int y1, int x2, int y2, unsigned int Color){ DXLE_GET_LOCK(dxle::Graph2D::screen_mutex_c::mtx); return DxLib::DrawLineBox(x1, y1, x2, y2, Color); }
	//!\~japanese 円を描画する
	inline int DrawCircle(dxle_mutex, int x, int y, int r, unsigned int Color, int FillFlag = TRUE, int LineThickness = 1){ DXLE_GET_LOCK(dxle::Graph2D::screen_mutex_c::mtx); return DxLib::DrawCircle(x, y, r, Color, FillFlag, LineThickness); }
	//!\~japanese 楕円を描画する
	inline int DrawOval(dxle_mutex, int x, int y, int rx, int ry, unsigned int Color, int FillFlag, int LineThickness = 1){ DXLE_GET_LOCK(dxle::Graph2D::screen_mutex_c::mtx);  return DxLib::DrawOval(x, y, rx, ry, Color, FillFlag, LineThickness); }
	//!\~japanese 三角形を描画する
	inline int DrawTriangle(dxle_mutex, int x1, int y1, int x2, int y2, int x3, int y3, unsigned int Color, int FillFlag){ DXLE_GET_LOCK(dxle::Graph2D::screen_mutex_c::mtx);  return DxLib::DrawTriangle(x1, y1, x2, y2, x3, y3, Color, FillFlag); }
	//!\~japanese 四角形を描画する
	inline int DrawQuadrangle(dxle_mutex, int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4, unsigned int Color, int FillFlag){ DXLE_GET_LOCK(dxle::Graph2D::screen_mutex_c::mtx);  return DxLib::DrawQuadrangle(x1, y1, x2, y2, x3, y3, x4, y4, Color, FillFlag); }
	//!\~japanese 角の丸い四角形を描画する
	inline int DrawRoundRect(dxle_mutex, int x1, int y1, int x2, int y2, int rx, int ry, unsigned int Color, int FillFlag){ DXLE_GET_LOCK(dxle::Graph2D::screen_mutex_c::mtx);  return DxLib::DrawRoundRect(x1, y1, x2, y2, rx, ry, Color, FillFlag); }
	//!\~japanese 点を描画する
	inline int DrawPixel(dxle_mutex, int x, int y, unsigned int Color){ DXLE_GET_LOCK(dxle::Graph2D::screen_mutex_c::mtx);  return DxLib::DrawPixel(x, y, Color); }


	//グラフィックデータ制御関数

	//!\~japanese 画像ファイルを読みこんで画面に描画する
	inline int LoadGraphScreen(dxle_mutex, int x, int y, const TCHAR *GraphName, int TransFlag){ DXLE_GET_LOCK(dxle::Graph2D::screen_mutex_c::mtx); return DxLib::LoadGraphScreen(x, y, GraphName, TransFlag); }
	//!\~japanese 画像ファイルからグラフィックハンドルを作成する
	inline int LoadGraph(dxle_mutex, const TCHAR *FileName, int NotUse3DFlag = FALSE){ return DxLib::LoadGraph(FileName, NotUse3DFlag); }
	//!\~japanese 画像ファイルを分割してグラフィックハンドルを作成する
	inline int LoadDivGraph(dxle_mutex, const TCHAR *FileName, int AllNum, int XNum, int YNum, int XSize, int YSize, int *HandleBuf, int NotUse3DFlag = FALSE){ return DxLib::LoadDivGraph(FileName, AllNum, XNum, YNum, XSize, YSize, HandleBuf, NotUse3DFlag); }
	//!\~japanese 指定サイズのグラフィックハンドルを作成する
	inline int MakeGraph(dxle_mutex, int SizeX, int SizeY, int NotUse3DFlag = FALSE){ return DxLib::MakeGraph(SizeX, SizeY, NotUse3DFlag); }
	//!\~japanese SetDrawScreen で描画対象にできるグラフィックハンドルを作成する
	inline int MakeScreen(dxle_mutex, int SizeX, int SizeY, int UseAlphaChannel = FALSE){ return DxLib::MakeScreen(SizeX, SizeY, UseAlphaChannel); }
	//!\~japanese SetDrawScreen の引数として渡せる( 描画対象として使用できる )グラフィックハンドルに適用するマルチサンプリング( アンチエイリアシング )設定を行う( Samples:マルチサンプル処理に使用するドット数( 多いほど重くなります )  Quality:マルチサンプル処理の品質 )
	inline int SetCreateDrawValidGraphMultiSample(dxle_mutex, int Samples, int Quality){ return DxLib::SetCreateDrawValidGraphMultiSample(Samples, Quality); }
	//!\~japanese 作成するグラフィックハンドルの色深度を設定する
	inline int SetCreateGraphColorBitDepth(dxle_mutex, int BitDepth){ return DxLib::SetCreateGraphColorBitDepth(BitDepth); }
	//!\~japanese SetDrawScreen の引数として渡せる( 描画対象として使用できる )ピクセルフォーマットが浮動小数点型のグラフィックハンドルを作成するかどうかを設定する( SetDrawValidGraphCreateFlag 関数で描画対象として使用できるグラフィックハンドルを作成するように設定されていないと効果ありません )、グラフィックスデバイスが浮動小数点型のピクセルフォーマットに対応していない場合はグラフィックハンドルの作成に失敗する( TRUE:浮動小数点型　　FALSE:整数型( デフォルト ) )
	inline int SetDrawValidFloatTypeGraphCreateFlag(dxle_mutex, int Flag){ return DxLib::SetDrawValidFloatTypeGraphCreateFlag(Flag); }
	//!\~japanese SetDrawScreen の引数として渡せる( 描画対象として使用できる )グラフィックハンドルに適用する色のチャンネル数を設定する( ChannelNum:チャンネル数( 指定可能な値は 1, 2, 4 の何れか( SetDrawValidGraphCreateFlag 関数で描画対象として使用できるグラフィックハンドルを作成するように設定されていないと効果ありません )
	inline int SetCreateDrawValidGraphChannelNum(dxle_mutex, int ChannelNum){ return DxLib::SetCreateDrawValidGraphChannelNum(ChannelNum); }
	//!\~japanese 画像ファイル読み込み時に乗算済みアルファ画像に変換するかどうかを設定する( TRUE:変換処理を行う  FALSE:変換処理を行わない( デフォルト ) )
	inline int SetUsePremulAlphaConvertLoad(dxle_mutex, int UseFlag){ return DxLib::SetUsePremulAlphaConvertLoad(UseFlag); }

	//!\~japanese 画像の等倍描画
	inline int DrawGraph(dxle_mutex, int x, int y, int GrHandle, int TransFlag){ DXLE_GET_LOCK(dxle::Graph2D::screen_mutex_c::mtx); return DxLib::DrawGraph(x, y, GrHandle, TransFlag); }
	//!\~japanese 画像の拡大描画
	inline int DrawExtendGraph(dxle_mutex, int x1, int y1, int x2, int y2, int GrHandle, int TransFlag){ DXLE_GET_LOCK(dxle::Graph2D::screen_mutex_c::mtx); return DxLib::DrawExtendGraph(x1, y1, x2, y2, GrHandle, TransFlag); }
	//!\~japanese 画像の回転描画
	inline int DrawRotaGraph(dxle_mutex, int x, int y, double ExRate, double Angle, int GrHandle, int TransFlag, int TurnFlag = FALSE){ DXLE_GET_LOCK(dxle::Graph2D::screen_mutex_c::mtx); return DxLib::DrawRotaGraph(x, y, ExRate, Angle, GrHandle, TransFlag, TurnFlag); }
	//!\~japanese 画像の回転描画２( 回転中心指定付き )
	inline int DrawRotaGraph2(dxle_mutex, int x, int y, int cx, int cy, double ExtRate, double Angle, int GrHandle, int TransFlag, int TurnFlag = FALSE){ DXLE_GET_LOCK(dxle::Graph2D::screen_mutex_c::mtx); return DxLib::DrawRotaGraph2(x, y, cx, cy, ExtRate, Angle, GrHandle, TransFlag, TurnFlag); }
	//!\~japanese 画像の回転描画３( 回転中心指定付き＋縦横拡大率別指定版 )
	inline int DrawRotaGraph3(dxle_mutex, int x, int y, int cx, int cy, double ExtRateX, double ExtRateY, double Angle, int GrHandle, int TransFlag, int TurnFlag = FALSE){ DXLE_GET_LOCK(dxle::Graph2D::screen_mutex_c::mtx); return DxLib::DrawRotaGraph3(x, y, cx, cy, ExtRateX, ExtRateY, Angle, GrHandle, TransFlag, TurnFlag); }
	//!\~japanese 画像の自由変形描画
	inline int DrawModiGraph(dxle_mutex, int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4, int GrHandle, int TransFlag){ DXLE_GET_LOCK(dxle::Graph2D::screen_mutex_c::mtx); return DxLib::DrawModiGraph(x1, y1, x2, y2, x3, y3, x4, y4, GrHandle, TransFlag); }
	//!\~japanese 画像の指定矩形部分のみを等倍描画
	inline int DrawRectGraph(dxle_mutex, int DestX, int DestY, int SrcX, int SrcY, int    Width, int    Height, int GraphHandle, int TransFlag, int TurnFlag){ DXLE_GET_LOCK(dxle::Graph2D::screen_mutex_c::mtx); return DxLib::DrawRectGraph(DestX, DestY, SrcX, SrcY, Width, Height, GraphHandle, TransFlag, TurnFlag); }

	//!\~japanese 指定のグラフィックハンドルの指定部分だけを抜き出して新たなグラフィックハンドルを作成する
	inline int DerivationGraph(dxle_mutex, int SrcX, int SrcY, int Width, int Height, int SrcGraphHandle){ return DxLib::DerivationGraph(SrcX, SrcY, Width, Height, SrcGraphHandle); }
	//!\~japanese 描画先の画面から指定領域の画像情報をグラフィックハンドルに転送する
	inline int GetDrawScreenGraph(dxle_mutex, int x1, int y1, int x2, int y2, int GrHandle, int UseClientFlag = TRUE){ DXLE_GET_LOCK(dxle::Graph2D::screen_mutex_c::mtx); return DxLib::GetDrawScreenGraph(x1, y1, x2, y2, GrHandle, UseClientFlag); }
	//!\~japanese グラフィックハンドルが持つ画像のサイズを得る
	inline int GetGraphSize(dxle_mutex, int GrHandle, int *SizeXBuf, int *SizeYBuf){ return DxLib::GetGraphSize(GrHandle, SizeXBuf, SizeYBuf); }
	//!\~japanese すべてのグラフィックハンドルを削除する
	inline int InitGraph(dxle_mutex, int LogOutFlag = FALSE){ return DxLib::InitGraph(LogOutFlag); }
	//!\~japanese グラフィックハンドルを削除する
	inline int DeleteGraph(dxle_mutex, int GrHandle, int LogOutFlag = FALSE){ return DxLib::DeleteGraph(GrHandle, LogOutFlag); }
	//!\~japanese 描画モードを設定する
	inline int SetDrawMode(dxle_mutex, int DrawMode){ return DxLib::SetDrawMode(DrawMode); }
	//!\~japanese 描画ブレンドモードを設定する
	inline int SetDrawBlendMode(dxle_mutex, int BlendMode, int BlendParam){ return DxLib::SetDrawBlendMode(BlendMode, BlendParam); }
	//!\~japanese 描画輝度を設定する
	inline int SetDrawBright(dxle_mutex, int RedBright, int GreenBright, int BlueBright){ return DxLib::SetDrawBright(RedBright, GreenBright, BlueBright); }
	//!\~japanese 作成するグラフィックハンドルに適用する透過色を設定する( Red,Green,Blue:透過色を光の３原色で表したもの( 各色０～２５５ ) )
	inline int SetTransColor(dxle_mutex, int Red, int Green, int Blue){ return DxLib::SetTransColor(Red, Green, Blue); }
	//!\~japanese 画像ファイルからブレンド用グラフィックハンドルを作成する
	inline int LoadBlendGraph(dxle_mutex, const TCHAR *FileName){ return DxLib::LoadBlendGraph(FileName); }

#endif
}

#endif
