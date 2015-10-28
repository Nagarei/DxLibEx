#ifndef DX_LIB_EX_GRAPH_2D_H_201510241561
#define DX_LIB_EX_GRAPH_2D_H_201510241561

#include "DxLib.h"
#include <type_traits>
#include <utility>
#include <memory>
#include <string>
#include "DxLibEx_Helper.h"
#include "DxLibEx_Defines.h"

//----------2Dグラフィック----------//

namespace DxLibEx
{
	//!DxLibExの公開クラスが全て入ったnamespaceです
	namespace DxLibEx_Classes {}
	//! 2Dグラフィック
	namespace Graph2D
	{
	
		//! class Texture2Dのオーバーヘッドが少ないVer
		//! ただしコピーができない
		class Texture2D_Unique : public impl::Unique_Handle_Bace<Texture2D_Unique>
		{
		public:
			//!ハンドルを削除する
			inline void Delete(bool LogOutFlag = false) { DeleteGraph(GetHandle(), LogOutFlag); }
			//!このオブジェクトを無効化し、今まで使っていたハンドルを返す
			//!自動削除も無効化される
			inline int release() { return ReleaseRun(); }
	
			//生成用static関数
	
			// グラフィックハンドル作成関係関数
	
			//! 指定サイズのグラフィックハンドルを作成する
			static inline Texture2D_Unique MakeGraph(int SizeX, int SizeY, bool NotUse3DFlag = false)DXLIBEX_NOEXCEPT { return DxLib::MakeGraph(SizeX, SizeY, NotUse3DFlag); }
			//! SetDrawScreen で描画対象にできるグラフィックハンドルを作成する
			static inline Texture2D_Unique MakeScreen(int SizeX, int SizeY, bool UseAlphaChannel = false)DXLIBEX_NOEXCEPT { return DxLib::MakeScreen(SizeX, SizeY, UseAlphaChannel); }
			//! 指定のグラフィックハンドルの指定部分だけを抜き出して新たなグラフィックハンドルを作成する
			static inline Texture2D_Unique DerivationGraph(int SrcX, int SrcY, int Width, int Height, int SrcGraphHandle)DXLIBEX_NOEXCEPT { return DxLib::DerivationGraph(SrcX, SrcY, Width, Height, SrcGraphHandle); }
	
			// 画像からグラフィックハンドルを作成する関数
	
			//! 画像ファイルからグラフィックハンドルを作成する
			static inline Texture2D_Unique LoadBmpToGraph(const TCHAR *FileName, int TextureFlag, int ReverseFlag, int SurfaceMode = DX_MOVIESURFACE_NORMAL)DXLIBEX_NOEXCEPT { return DxLib::LoadBmpToGraph(FileName, TextureFlag, ReverseFlag, SurfaceMode); }
			//! 画像ファイルからグラフィックハンドルを作成する
			static inline Texture2D_Unique LoadGraph(const TCHAR *FileName, bool NotUse3DFlag = false)DXLIBEX_NOEXCEPT { return DxLib::LoadGraph(FileName, NotUse3DFlag); }
			//! 画像ファイルを反転したものでグラフィックハンドルを作成する
			static inline Texture2D_Unique LoadReverseGraph(const TCHAR *FileName, bool NotUse3DFlag = false)DXLIBEX_NOEXCEPT { return DxLib::LoadReverseGraph(FileName, NotUse3DFlag); }
			//! 画像ファイルからブレンド用グラフィックハンドルを作成する
			static inline Texture2D_Unique LoadBlendGraph(const TCHAR *FileName)DXLIBEX_NOEXCEPT { return DxLib::LoadBlendGraph(FileName); }
	
			//! メモリ上の画像イメージからグラフィックハンドルを作成する
			static inline Texture2D_Unique CreateGraphFromMem(const void *RGBFileImage, int RGBFileImageSize, const void *AlphaFileImage = nullptr, int AlphaFileImageSize = 0, bool TextureFlag = true, bool ReverseFlag = false)DXLIBEX_NOEXCEPT { return DxLib::CreateGraphFromMem(RGBFileImage, RGBFileImageSize, AlphaFileImage, AlphaFileImageSize, TextureFlag, ReverseFlag); }
			//! メモリ上の画像イメージから既存のグラフィックハンドルにデータを転送する
			static inline int ReCreateGraphFromMem(const void *RGBFileImage, int RGBFileImageSize, Texture2D_Unique& GrHandle, const void *AlphaFileImage = nullptr, int AlphaFileImageSize = 0, bool TextureFlag = true, bool ReverseFlag = false)DXLIBEX_NOEXCEPT { return DxLib::ReCreateGraphFromMem(RGBFileImage, RGBFileImageSize, GrHandle.GetHandle(), AlphaFileImage, AlphaFileImageSize, TextureFlag, ReverseFlag); }
	
			//! 基本イメージデータからサイズを割り出し、それに合ったグラフィックハンドルを作成する
			static inline Texture2D_Unique CreateDXGraph(const BASEIMAGE *RgbBaseImage, const BASEIMAGE *AlphaBaseImage, bool TextureFlag)DXLIBEX_NOEXCEPT { return DxLib::CreateDXGraph(RgbBaseImage, AlphaBaseImage, TextureFlag); }
			//! 基本イメージデータからグラフィックハンドルを作成する
			static inline Texture2D_Unique CreateGraphFromGraphImage(const BASEIMAGE *RgbBaseImage, bool TextureFlag = true, bool ReverseFlag = false)DXLIBEX_NOEXCEPT { return DxLib::CreateGraphFromGraphImage(RgbBaseImage, TextureFlag, ReverseFlag); }
			//! 基本イメージデータからグラフィックハンドルを作成する
			static inline Texture2D_Unique CreateGraphFromGraphImage(const BASEIMAGE *RgbBaseImage, const BASEIMAGE *AlphaBaseImage, bool TextureFlag = true, bool ReverseFlag = false)DXLIBEX_NOEXCEPT { return DxLib::CreateGraphFromGraphImage(RgbBaseImage, AlphaBaseImage, TextureFlag, ReverseFlag); }
			//! 基本イメージデータから既存のグラフィックハンドルにデータを転送する
			static inline int ReCreateGraphFromGraphImage(const BASEIMAGE *RgbBaseImage, Texture2D_Unique& GrHandle, bool TextureFlag = true, bool ReverseFlag = false)DXLIBEX_NOEXCEPT { return DxLib::ReCreateGraphFromGraphImage(RgbBaseImage, GrHandle.GetHandle(), TextureFlag, ReverseFlag); }
			//! 基本イメージデータから既存のグラフィックハンドルにデータを転送する
			static inline int ReCreateGraphFromGraphImage(const BASEIMAGE *RgbBaseImage, const BASEIMAGE *AlphaBaseImage, Texture2D_Unique& GrHandle, bool TextureFlag = true, bool ReverseFlag = false)DXLIBEX_NOEXCEPT { return DxLib::ReCreateGraphFromGraphImage(RgbBaseImage, AlphaBaseImage, GrHandle.GetHandle(), TextureFlag, ReverseFlag); }
	
			//! メモリ上のビットマップイメージからグラフィックハンドルを作成する
			static inline Texture2D_Unique CreateGraph(int Width, int Height, int Pitch, const void *RGBImage, const void *AlphaImage = nullptr, const Texture2D_Unique& GrHandle = Texture2D_Unique())DXLIBEX_NOEXCEPT { return DxLib::CreateGraph(Width, Height, Pitch, RGBImage, AlphaImage, GrHandle.GetHandle()); }
			//! メモリ上のビットマップイメージからグラフィックハンドルを再作成する
			static inline int ReCreateGraph(int Width, int Height, int Pitch, const void *RGBImage, Texture2D_Unique& GrHandle, const void *AlphaImage = nullptr)DXLIBEX_NOEXCEPT { return DxLib::ReCreateGraph(Width, Height, Pitch, RGBImage, GrHandle.GetHandle(), AlphaImage); }
	#ifndef DX_NON_SOFTIMAGE
			//extern	int			CreateBlendGraphFromSoftImage(int SIHandle);																													// ソフトウエアで扱うイメージからブレンド用画像グラフィックハンドルを作成する( -1:エラー  -1以外:ブレンド用グラフィックハンドル )
			//extern	int			CreateGraphFromSoftImage(int SIHandle);																													// ソフトウエアで扱うイメージからグラフィックハンドルを作成する( -1:エラー  -1以外:グラフィックハンドル )
			//extern	int			CreateGraphFromRectSoftImage(int SIHandle, int x, int y, int SizeX, int SizeY);																				// ソフトウエアで扱うイメージの指定の領域を使ってグラフィックハンドルを作成する( -1:エラー  -1以外:グラフィックハンドル )
			//extern	int			ReCreateGraphFromSoftImage(int SIHandle, int GrHandle);																										// ソフトウエアで扱うイメージから既存のグラフィックハンドルに画像データを転送する
			//extern	int			ReCreateGraphFromRectSoftImage(int SIHandle, int x, int y, int SizeX, int SizeY, int GrHandle);																	// ソフトウエアで扱うイメージから既存のグラフィックハンドルに画像データを転送する
			//extern	int			CreateDivGraphFromSoftImage(int SIHandle, int AllNum, int XNum, int YNum, int SizeX, int SizeY, int *HandleBuf);												// ソフトウエアで扱うイメージから分割グラフィックハンドルを作成する
	#endif // DX_NON_SOFTIMAGE
			//! 基本イメージデータからグラフィックハンドルを作成する
			static inline Texture2D_Unique CreateGraphFromBaseImage(const BASEIMAGE *BaseImage)DXLIBEX_NOEXCEPT { return DxLib::CreateGraphFromBaseImage(BaseImage); }
			//! 基本イメージデータの指定の領域を使ってグラフィックハンドルを作成する
			static inline Texture2D_Unique CreateGraphFromRectBaseImage(const BASEIMAGE *BaseImage, int x, int y, int SizeX, int SizeY)DXLIBEX_NOEXCEPT { return DxLib::CreateGraphFromRectBaseImage(BaseImage, x, y, SizeX, SizeY); }
			//! 基本イメージデータから既存のグラフィックハンドルに画像データを転送する
			static inline int ReCreateGraphFromBaseImage(const BASEIMAGE *BaseImage, Texture2D_Unique& GrHandle)DXLIBEX_NOEXCEPT { return DxLib::ReCreateGraphFromBaseImage(BaseImage, GrHandle.GetHandle()); }
			//! 基本イメージデータの指定の領域を使って既存のグラフィックハンドルに画像データを転送する
			static inline int ReCreateGraphFromRectBaseImage(const BASEIMAGE *BaseImage, int x, int y, int SizeX, int SizeY, Texture2D_Unique& GrHandle)DXLIBEX_NOEXCEPT { return DxLib::ReCreateGraphFromRectBaseImage(BaseImage, x, y, SizeX, SizeY, GrHandle.GetHandle()); }
			//! 画像ファイルからグラフィックハンドルへ画像データを転送する
			static inline int ReloadGraph(const TCHAR *FileName, Texture2D_Unique& GrHandle, bool ReverseFlag = false)DXLIBEX_NOEXCEPT { return DxLib::ReloadGraph(FileName, GrHandle.GetHandle(), ReverseFlag); }
			//! ReloadGraph の画像反転処理追加版
			static inline int ReloadReverseGraph(const TCHAR *FileName, Texture2D_Unique& GrHandle)DXLIBEX_NOEXCEPT { return DxLib::ReloadReverseGraph(FileName, GrHandle.GetHandle()); }
	
	
			//メンバ関数
	
			//! 指定のグラフィックハンドルのＡＲＧＢ８イメージを取得する( 現在動画ファイルをグラフィックハンドルで読み込んだ場合のみ使用可能 )
			inline const unsigned int* GetFullColorImage()const DXLIBEX_NOEXCEPT { return DxLib::GetFullColorImage(GetHandle()); }
			//! グラフィックメモリ領域のロック
			inline int GraphLock(int *PitchBuf, void **DataPointBuf, COLORDATA **ColorDataPP = nullptr, bool WriteOnly = false)DXLIBEX_NOEXCEPT { return DxLib::GraphLock(GetHandle(), PitchBuf, DataPointBuf, ColorDataPP, WriteOnly); }
			//! グラフィックメモリ領域のロック解除
			inline int GraphUnLock()DXLIBEX_NOEXCEPT { return DxLib::GraphUnLock(GetHandle()); }
			//! グラフィックハンドル専用のＺバッファを持つかどうかを設定する( GrHandle:対象となるグラフィックハンドル( 描画対象として使用可能なグラフィックハンドルのみ有効 )  UseFlag:専用のＺバッファを持つかどうか( TRUE:持つ( デフォルト )  FALSE:持たない )  BitDepth:ビット深度( 16 or 24 or 32 ) )
			inline int SetUseGraphZBuffer(int UseFlag, int BitDepth = -1)DXLIBEX_NOEXCEPT { return DxLib::SetUseGraphZBuffer(GetHandle(), UseFlag, BitDepth); }
			//! グラフィックハンドルのＺバッファの状態を別のグラフィックハンドルのＺバッファにコピーする( DestGrHandle も SrcGrHandle もＺバッファを持っている描画対象にできるグラフィックハンドルで、サイズが同じであり、且つマルチサンプリング( アンチエイリアス )設定が無いことが条件 )
			inline int CopyGraphZBufferImage(Texture2D_Unique& ToGrHandle)const DXLIBEX_NOEXCEPT { return DxLib::CopyGraphZBufferImage(GetHandle(), ToGrHandle.GetHandle()); }
			//! グラフィックスデバイスのデバイスロスト発生時に指定のグラフィックハンドルを削除するかどうかを設定する( TRUE:デバイスロスト時に削除する  FALSE:デバイスロストが発生しても削除しない )
			inline int SetDeviceLostDeleteGraphFlag(bool DeleteFlag)DXLIBEX_NOEXCEPT { return DxLib::SetDeviceLostDeleteGraphFlag(GetHandle(), DeleteFlag); }
			//! グラフィックハンドルが持つ画像のサイズを得る
			inline int GetGraphSize(int *SizeXBuf, int *SizeYBuf)const DXLIBEX_NOEXCEPT { return DxLib::GetGraphSize(GetHandle(), SizeXBuf, SizeYBuf); }
			//! グラフィックハンドルが持つ画像のサイズを得る
			//!@return .first:SizeX .second:SizeY
			inline std::pair<int, int> GetGraphSize()const DXLIBEX_NOEXCEPT { int x, y; GetGraphSize(&x, &y); return std::make_pair(x, y); }
			//! グラフィックハンドルが持つ一つ目のテクスチャのサイズを得る
			inline int GetGraphTextureSize(int *SizeXBuf, int *SizeYBuf)const DXLIBEX_NOEXCEPT { return DxLib::GetGraphTextureSize(GetHandle(), SizeXBuf, SizeYBuf); }
			//! グラフィックハンドルが持つ一つ目のテクスチャのサイズを得る
			//!@return .first:SizeX .second:SizeY
			inline std::pair<int, int> GetGraphTextureSize()const DXLIBEX_NOEXCEPT { int x, y; GetGraphTextureSize(&x, &y); return std::make_pair(x, y); }
			//! グラフィックハンドルが持つテクスチャのミップマップレベル数を取得する
			inline int GetGraphMipmapCount()const DXLIBEX_NOEXCEPT { return DxLib::GetGraphMipmapCount(GetHandle()); }
			//! グラフィックハンドルが画像ファイルから読み込まれていた場合、その画像のファイルパスを取得する
			inline int GetGraphFilePath(TCHAR FilePathBuffer[])const DXLIBEX_NOEXCEPT { return DxLib::GetGraphFilePath(GetHandle(), FilePathBuffer); }
			//! グラフィックハンドルが画像ファイルから読み込まれていた場合、その画像のファイルパスを取得する
			inline std::basic_string<TCHAR> GetGraphFilePath()const DXLIBEX_NOEXCEPT { TCHAR FilePathBuffer[1024]; if (GetGraphFilePath(FilePathBuffer) == -1) { return std::basic_string<TCHAR>(); } return FilePathBuffer; }
	
			//! 画像の等倍描画
			inline int DrawGraph(int x, int y, bool TransFlag)const DXLIBEX_NOEXCEPT { return DxLib::DrawGraph(x, y, GetHandle(), TransFlag); }
			//! 画像の拡大描画
			inline int DrawExtendGraph(int x1, int y1, int x2, int y2, bool TransFlag)const DXLIBEX_NOEXCEPT { return DxLib::DrawExtendGraph(x1, y1, x2, y2, GetHandle(), TransFlag); }
			//! 画像の回転描画
			inline int DrawRotaGraph(int x, int y, double ExRate, double Angle, bool TransFlag, bool TurnFlag = false)const DXLIBEX_NOEXCEPT { return DxLib::DrawRotaGraph(x, y, ExRate, Angle, GetHandle(), TransFlag, TurnFlag); }
			//! 画像の回転描画２( 回転中心指定付き )
			inline int DrawRotaGraph2(int x, int y, int cx, int cy, double ExtRate, double Angle, bool TransFlag, bool TurnFlag = false)const DXLIBEX_NOEXCEPT { return DxLib::DrawRotaGraph2(x, y, cx, cy, ExtRate, Angle, GetHandle(), TransFlag, TurnFlag); }
			//! 画像の回転描画３( 回転中心指定付き＋縦横拡大率別指定版 )
			inline int DrawRotaGraph3(int x, int y, int cx, int cy, double ExtRateX, double ExtRateY, double Angle, bool TransFlag, bool TurnFlag = FALSE)const DXLIBEX_NOEXCEPT { return DxLib::DrawRotaGraph3(x, y, cx, cy, ExtRateX, ExtRateY, Angle, GetHandle(), TransFlag, TurnFlag); }
			//! 画像の自由変形描画
			inline int DrawModiGraph(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4, bool TransFlag)const DXLIBEX_NOEXCEPT { return DxLib::DrawModiGraph(x1, y1, x2, y2, x3, y3, x4, y4, GetHandle(), TransFlag); }
			//! 画像の左右反転描画
			inline int DrawTurnGraph(int x, int y, bool TransFlag)const DXLIBEX_NOEXCEPT { return DxLib::DrawTurnGraph(x, y, GetHandle(), TransFlag); }
			//! 画像の拡大左右反転描画
			inline int DrawExtendTurnGraph(int x1, int y1, int x2, int y2, bool TransFlag)const DXLIBEX_NOEXCEPT { return DxLib::DrawExtendGraph(x2, y1, x1, y2, GetHandle(), TransFlag); }
	
			//! 画像の描画( 座標指定が float 版 )
			inline int DrawGraphF(float xf, float yf, bool TransFlag)const DXLIBEX_NOEXCEPT { return DxLib::DrawGraphF(xf, yf, GetHandle(), TransFlag); }
			//! 画像の拡大描画( 座標指定が float 版 )
			inline int DrawExtendGraphF(float x1f, float y1f, float x2f, float y2, bool TransFlag)const DXLIBEX_NOEXCEPT { return DxLib::DrawExtendGraphF(x1f, y1f, x2f, y2, GetHandle(), TransFlag); }
			//! 画像の回転描画( 座標指定が float 版 )
			inline int DrawRotaGraphF(float xf, float yf, double ExRate, double Angle, bool TransFlag, bool TurnFlag = FALSE)const DXLIBEX_NOEXCEPT { return DxLib::DrawRotaGraphF(xf, yf, ExRate, Angle, GetHandle(), TransFlag, TurnFlag); }
			//! 画像の回転描画２( 回転中心指定付き )( 座標指定が float 版 )
			inline int DrawRotaGraph2F(float xf, float yf, float cxf, float cyf, double ExtRate, double Angle, bool TransFlag, bool TurnFlag = FALSE)const DXLIBEX_NOEXCEPT { return DxLib::DrawRotaGraph2F(xf, yf, cxf, cyf, ExtRate, Angle, GetHandle(), TransFlag, TurnFlag); }
			//! 画像の回転描画３( 回転中心指定付き＋縦横拡大率別指定版 )( 座標指定が float 版 )
			inline int DrawRotaGraph3F(float xf, float yf, float cxf, float cyf, double ExtRateX, double ExtRateY, double Angle, bool TransFlag, bool TurnFlag = FALSE)const DXLIBEX_NOEXCEPT { return DxLib::DrawRotaGraph3F(xf, yf, cxf, cyf, ExtRateX, ExtRateY, Angle, GetHandle(), TransFlag, TurnFlag); }
			//! 画像の自由変形描画( 座標指定が float 版 )
			inline int DrawModiGraphF(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4, bool TransFlag)const DXLIBEX_NOEXCEPT { return DxLib::DrawModiGraphF(x1, y1, x2, y2, x3, y3, x4, y4, GetHandle(), TransFlag); }
			//! 画像の左右反転描画( 座標指定が float 版 )
			inline int DrawTurnGraphF(float xf, float yf, bool TransFlag)const DXLIBEX_NOEXCEPT { return DxLib::DrawTurnGraphF(xf, yf, GetHandle(), TransFlag); }
			//! 画像の拡大左右反転描画( 座標指定が float 版 )
			inline int DrawExtendTurnGraphF(float x1f, float y1f, float x2f, float y2f, bool TransFlag)const DXLIBEX_NOEXCEPT { return DxLib::DrawExtendGraphF(x2f, y1f, x1f, y2f, GetHandle(), TransFlag); }
	
		private:
			typedef Unique_Handle_Bace<Texture2D_Unique> Parent_T;
		public:
			Texture2D_Unique() : Unique_Handle_Bace() {}
			//!所有権の譲渡
			Texture2D_Unique(Texture2D_Unique&& other) : Unique_Handle_Bace(std::move(other)) {}
			//! 所有権の譲渡
			Texture2D_Unique& operator=(Texture2D_Unique&& other) { Parent_T::operator=(std::move(other)); return *this; }
		private:
			Texture2D_Unique(int param_handle) : Unique_Handle_Bace(param_handle) {}//間違えて他の種類のハンドルを持たないようにprivateにしておく
		};
	
		//! 画像ハンドルクラス
		//! 誰も参照しなくなったらハンドルを自動削除する
		class Texture2D
		{
		public:
			//!サウンドハンドルを削除する
			inline void Delete(bool LogOutFlag = false) { p_handle->Delete(LogOutFlag); }
			//!このオブジェクトを無効化する
			//!最後の一個だった場合ハンドルは自動的に削除される
			inline void release_this() { p_handle.reset(); }
			//!このオブジェクトおよび同じ派生元のオブジェクトを無効化し、今まで使っていたハンドルを返す
			//!自動削除も無効化される
			inline int release_all() { return p_handle->release(); }
	
	
			//生成用static関数
	
			// グラフィックハンドル作成関係関数
	
			//! 指定サイズのグラフィックハンドルを作成する
			static inline Texture2D MakeGraph(int SizeX, int SizeY, bool NotUse3DFlag = false)DXLIBEX_NOEXCEPT { return Texture2D_Unique::MakeGraph(SizeX, SizeY, NotUse3DFlag); }
			//! SetDrawScreen で描画対象にできるグラフィックハンドルを作成する
			static inline Texture2D MakeScreen(int SizeX, int SizeY, bool UseAlphaChannel = false)DXLIBEX_NOEXCEPT { return Texture2D_Unique::MakeScreen(SizeX, SizeY, UseAlphaChannel); }
			//! 指定のグラフィックハンドルの指定部分だけを抜き出して新たなグラフィックハンドルを作成する
			static inline Texture2D DerivationGraph(int SrcX, int SrcY, int Width, int Height, int SrcGraphHandle)DXLIBEX_NOEXCEPT { return Texture2D_Unique::DerivationGraph(SrcX, SrcY, Width, Height, SrcGraphHandle); }
	
			// 画像からグラフィックハンドルを作成する関数
	
			//! 画像ファイルからグラフィックハンドルを作成する
			static inline Texture2D LoadBmpToGraph(const TCHAR *FileName, int TextureFlag, int ReverseFlag, int SurfaceMode = DX_MOVIESURFACE_NORMAL)DXLIBEX_NOEXCEPT { return Texture2D_Unique::LoadBmpToGraph(FileName, TextureFlag, ReverseFlag, SurfaceMode); }
			//! 画像ファイルからグラフィックハンドルを作成する
			static inline Texture2D LoadGraph(const TCHAR *FileName, bool NotUse3DFlag = false)DXLIBEX_NOEXCEPT { return Texture2D_Unique::LoadGraph(FileName, NotUse3DFlag); }
			//! 画像ファイルを反転したものでグラフィックハンドルを作成する
			static inline Texture2D LoadReverseGraph(const TCHAR *FileName, bool NotUse3DFlag = false)DXLIBEX_NOEXCEPT { return Texture2D_Unique::LoadReverseGraph(FileName, NotUse3DFlag); }
			//! 画像ファイルからブレンド用グラフィックハンドルを作成する
			static inline Texture2D LoadBlendGraph(const TCHAR *FileName)DXLIBEX_NOEXCEPT { return Texture2D_Unique::LoadBlendGraph(FileName); }
	
			//! メモリ上の画像イメージからグラフィックハンドルを作成する
			static inline Texture2D CreateGraphFromMem(const void *RGBFileImage, int RGBFileImageSize, const void *AlphaFileImage = nullptr, int AlphaFileImageSize = 0, bool TextureFlag = true, bool ReverseFlag = false)DXLIBEX_NOEXCEPT { return Texture2D_Unique::CreateGraphFromMem(RGBFileImage, RGBFileImageSize, AlphaFileImage, AlphaFileImageSize, TextureFlag, ReverseFlag); }
			//! メモリ上の画像イメージから既存のグラフィックハンドルにデータを転送する
			static inline int ReCreateGraphFromMem(const void *RGBFileImage, int RGBFileImageSize, Texture2D_Unique& GrHandle, const void *AlphaFileImage = nullptr, int AlphaFileImageSize = 0, bool TextureFlag = true, bool ReverseFlag = false)DXLIBEX_NOEXCEPT { return Texture2D_Unique::ReCreateGraphFromMem(RGBFileImage, RGBFileImageSize, GrHandle, AlphaFileImage, AlphaFileImageSize, TextureFlag, ReverseFlag); }
	
			//! 基本イメージデータからサイズを割り出し、それに合ったグラフィックハンドルを作成する
			static inline Texture2D CreateDXGraph(const BASEIMAGE *RgbBaseImage, const BASEIMAGE *AlphaBaseImage, bool TextureFlag)DXLIBEX_NOEXCEPT { return Texture2D_Unique::CreateDXGraph(RgbBaseImage, AlphaBaseImage, TextureFlag); }
			//! 基本イメージデータからグラフィックハンドルを作成する
			static inline Texture2D CreateGraphFromGraphImage(const BASEIMAGE *RgbBaseImage, bool TextureFlag = true, bool ReverseFlag = false)DXLIBEX_NOEXCEPT { return Texture2D_Unique::CreateGraphFromGraphImage(RgbBaseImage, TextureFlag, ReverseFlag); }
			//! 基本イメージデータからグラフィックハンドルを作成する
			static inline Texture2D CreateGraphFromGraphImage(const BASEIMAGE *RgbBaseImage, const BASEIMAGE *AlphaBaseImage, bool TextureFlag = true, bool ReverseFlag = false)DXLIBEX_NOEXCEPT { return Texture2D_Unique::CreateGraphFromGraphImage(RgbBaseImage, AlphaBaseImage, TextureFlag, ReverseFlag); }
			//! 基本イメージデータから既存のグラフィックハンドルにデータを転送する
			static inline int ReCreateGraphFromGraphImage(const BASEIMAGE *RgbBaseImage, Texture2D_Unique& GrHandle, bool TextureFlag = true, bool ReverseFlag = false)DXLIBEX_NOEXCEPT { return Texture2D_Unique::ReCreateGraphFromGraphImage(RgbBaseImage, GrHandle, TextureFlag, ReverseFlag); }
			//! 基本イメージデータから既存のグラフィックハンドルにデータを転送する
			static inline int ReCreateGraphFromGraphImage(const BASEIMAGE *RgbBaseImage, const BASEIMAGE *AlphaBaseImage, Texture2D_Unique& GrHandle, bool TextureFlag = true, bool ReverseFlag = false)DXLIBEX_NOEXCEPT { return Texture2D_Unique::ReCreateGraphFromGraphImage(RgbBaseImage, AlphaBaseImage, GrHandle, TextureFlag, ReverseFlag); }
	
			//! メモリ上のビットマップイメージからグラフィックハンドルを作成する
			static inline Texture2D CreateGraph(int Width, int Height, int Pitch, const void *RGBImage, const void *AlphaImage = nullptr, const Texture2D_Unique& GrHandle = Texture2D_Unique())DXLIBEX_NOEXCEPT { return Texture2D_Unique::CreateGraph(Width, Height, Pitch, RGBImage, AlphaImage, GrHandle); }
			//! メモリ上のビットマップイメージからグラフィックハンドルを再作成する
			static inline int ReCreateGraph(int Width, int Height, int Pitch, const void *RGBImage, Texture2D_Unique& GrHandle, const void *AlphaImage = nullptr)DXLIBEX_NOEXCEPT { return Texture2D_Unique::ReCreateGraph(Width, Height, Pitch, RGBImage, GrHandle, AlphaImage); }
	#ifndef DX_NON_SOFTIMAGE
			//extern	int			CreateBlendGraphFromSoftImage(int SIHandle);																													// ソフトウエアで扱うイメージからブレンド用画像グラフィックハンドルを作成する( -1:エラー  -1以外:ブレンド用グラフィックハンドル )
			//extern	int			CreateGraphFromSoftImage(int SIHandle);																													// ソフトウエアで扱うイメージからグラフィックハンドルを作成する( -1:エラー  -1以外:グラフィックハンドル )
			//extern	int			CreateGraphFromRectSoftImage(int SIHandle, int x, int y, int SizeX, int SizeY);																				// ソフトウエアで扱うイメージの指定の領域を使ってグラフィックハンドルを作成する( -1:エラー  -1以外:グラフィックハンドル )
			//extern	int			ReCreateGraphFromSoftImage(int SIHandle, int GrHandle);																										// ソフトウエアで扱うイメージから既存のグラフィックハンドルに画像データを転送する
			//extern	int			ReCreateGraphFromRectSoftImage(int SIHandle, int x, int y, int SizeX, int SizeY, int GrHandle);																	// ソフトウエアで扱うイメージから既存のグラフィックハンドルに画像データを転送する
			//extern	int			CreateDivGraphFromSoftImage(int SIHandle, int AllNum, int XNum, int YNum, int SizeX, int SizeY, int *HandleBuf);												// ソフトウエアで扱うイメージから分割グラフィックハンドルを作成する
	#endif // DX_NON_SOFTIMAGE
			//! 基本イメージデータからグラフィックハンドルを作成する
			static inline Texture2D CreateGraphFromBaseImage(const BASEIMAGE *BaseImage)DXLIBEX_NOEXCEPT { return Texture2D_Unique::CreateGraphFromBaseImage(BaseImage); }
			//! 基本イメージデータの指定の領域を使ってグラフィックハンドルを作成する
			static inline Texture2D CreateGraphFromRectBaseImage(const BASEIMAGE *BaseImage, int x, int y, int SizeX, int SizeY)DXLIBEX_NOEXCEPT { return Texture2D_Unique::CreateGraphFromRectBaseImage(BaseImage, x, y, SizeX, SizeY); }
			//! 基本イメージデータから既存のグラフィックハンドルに画像データを転送する
			static inline int ReCreateGraphFromBaseImage(const BASEIMAGE *BaseImage, Texture2D_Unique& GrHandle)DXLIBEX_NOEXCEPT { return Texture2D_Unique::ReCreateGraphFromBaseImage(BaseImage, GrHandle); }
			//! 基本イメージデータの指定の領域を使って既存のグラフィックハンドルに画像データを転送する
			static inline int ReCreateGraphFromRectBaseImage(const BASEIMAGE *BaseImage, int x, int y, int SizeX, int SizeY, Texture2D_Unique& GrHandle)DXLIBEX_NOEXCEPT { return Texture2D_Unique::ReCreateGraphFromRectBaseImage(BaseImage, x, y, SizeX, SizeY, GrHandle); }
			//! 画像ファイルからグラフィックハンドルへ画像データを転送する
			static inline int ReloadGraph(const TCHAR *FileName, Texture2D_Unique& GrHandle, bool ReverseFlag = false)DXLIBEX_NOEXCEPT { return Texture2D_Unique::ReloadGraph(FileName, GrHandle, ReverseFlag); }
			//! ReloadGraph の画像反転処理追加版
			static inline int ReloadReverseGraph(const TCHAR *FileName, Texture2D_Unique& GrHandle)DXLIBEX_NOEXCEPT { return Texture2D_Unique::ReloadReverseGraph(FileName, GrHandle); }
	
	
			//メンバ関数
	
			//! 指定のグラフィックハンドルのＡＲＧＢ８イメージを取得する( 現在動画ファイルをグラフィックハンドルで読み込んだ場合のみ使用可能 )
			inline const unsigned int* GetFullColorImage()const DXLIBEX_NOEXCEPT { return p_handle->GetFullColorImage(); }
			//! グラフィックメモリ領域のロック
			inline int GraphLock(int *PitchBuf, void **DataPointBuf, COLORDATA **ColorDataPP = nullptr, bool WriteOnly = false)DXLIBEX_NOEXCEPT { return p_handle->GraphLock(PitchBuf, DataPointBuf, ColorDataPP, WriteOnly); }
			//! グラフィックメモリ領域のロック解除
			inline int GraphUnLock()DXLIBEX_NOEXCEPT { return p_handle->GraphUnLock(); }
			//! グラフィックハンドル専用のＺバッファを持つかどうかを設定する( GrHandle:対象となるグラフィックハンドル( 描画対象として使用可能なグラフィックハンドルのみ有効 )  UseFlag:専用のＺバッファを持つかどうか( TRUE:持つ( デフォルト )  FALSE:持たない )  BitDepth:ビット深度( 16 or 24 or 32 ) )
			inline int SetUseGraphZBuffer(int UseFlag, int BitDepth = -1)DXLIBEX_NOEXCEPT { return p_handle->SetUseGraphZBuffer(UseFlag, BitDepth); }
			//! グラフィックハンドルのＺバッファの状態を別のグラフィックハンドルのＺバッファにコピーする( DestGrHandle も SrcGrHandle もＺバッファを持っている描画対象にできるグラフィックハンドルで、サイズが同じであり、且つマルチサンプリング( アンチエイリアス )設定が無いことが条件 )
			inline int CopyGraphZBufferImage(Texture2D_Unique& ToGrHandle)const DXLIBEX_NOEXCEPT { return p_handle->CopyGraphZBufferImage(ToGrHandle); }
			//! グラフィックスデバイスのデバイスロスト発生時に指定のグラフィックハンドルを削除するかどうかを設定する( TRUE:デバイスロスト時に削除する  FALSE:デバイスロストが発生しても削除しない )
			inline int SetDeviceLostDeleteGraphFlag(bool DeleteFlag)DXLIBEX_NOEXCEPT { return p_handle->SetDeviceLostDeleteGraphFlag(DeleteFlag); }
			//! グラフィックハンドルが持つ画像のサイズを得る
			inline int GetGraphSize(int *SizeXBuf, int *SizeYBuf)const DXLIBEX_NOEXCEPT { return p_handle->GetGraphSize(SizeXBuf, SizeYBuf); }
			//! グラフィックハンドルが持つ画像のサイズを得る
			//!@return .first:SizeX .second:SizeY
			inline std::pair<int, int> GetGraphSize()const DXLIBEX_NOEXCEPT { int x, y; GetGraphSize(&x, &y); return std::make_pair(x, y); }
			//! グラフィックハンドルが持つ一つ目のテクスチャのサイズを得る
			inline int GetGraphTextureSize(int *SizeXBuf, int *SizeYBuf)const DXLIBEX_NOEXCEPT { return p_handle->GetGraphTextureSize(SizeXBuf, SizeYBuf); }
			//! グラフィックハンドルが持つ一つ目のテクスチャのサイズを得る
			//!@return .first:SizeX .second:SizeY
			inline std::pair<int, int> GetGraphTextureSize()const DXLIBEX_NOEXCEPT { int x, y; GetGraphTextureSize(&x, &y); return std::make_pair(x, y); }
			//! グラフィックハンドルが持つテクスチャのミップマップレベル数を取得する
			inline int GetGraphMipmapCount()const DXLIBEX_NOEXCEPT { return p_handle->GetGraphMipmapCount(); }
			//! グラフィックハンドルが画像ファイルから読み込まれていた場合、その画像のファイルパスを取得する
			inline int GetGraphFilePath(TCHAR FilePathBuffer[])const DXLIBEX_NOEXCEPT { return p_handle->GetGraphFilePath(FilePathBuffer); }
			//! グラフィックハンドルが画像ファイルから読み込まれていた場合、その画像のファイルパスを取得する
			inline std::basic_string<TCHAR> GetGraphFilePath()const DXLIBEX_NOEXCEPT { TCHAR FilePathBuffer[1024]; if (GetGraphFilePath(FilePathBuffer) == -1) { return std::basic_string<TCHAR>(); } return FilePathBuffer; }
	
			//! 画像の等倍描画
			inline int DrawGraph(int x, int y, bool TransFlag)const DXLIBEX_NOEXCEPT { return p_handle->DrawGraph(x, y, TransFlag); }
			//! 画像の拡大描画
			inline int DrawExtendGraph(int x1, int y1, int x2, int y2, bool TransFlag)const DXLIBEX_NOEXCEPT { return p_handle->DrawExtendGraph(x1, y1, x2, y2, TransFlag); }
			//! 画像の回転描画
			inline int DrawRotaGraph(int x, int y, double ExRate, double Angle, bool TransFlag, bool TurnFlag = false)const DXLIBEX_NOEXCEPT { return p_handle->DrawRotaGraph(x, y, ExRate, Angle, TransFlag, TurnFlag); }
			//! 画像の回転描画２( 回転中心指定付き )
			inline int DrawRotaGraph2(int x, int y, int cx, int cy, double ExtRate, double Angle, bool TransFlag, bool TurnFlag = false)const DXLIBEX_NOEXCEPT { return p_handle->DrawRotaGraph2(x, y, cx, cy, ExtRate, Angle, TransFlag, TurnFlag); }
			//! 画像の回転描画３( 回転中心指定付き＋縦横拡大率別指定版 )
			inline int DrawRotaGraph3(int x, int y, int cx, int cy, double ExtRateX, double ExtRateY, double Angle, bool TransFlag, bool TurnFlag = FALSE)const DXLIBEX_NOEXCEPT { return p_handle->DrawRotaGraph3(x, y, cx, cy, ExtRateX, ExtRateY, Angle, TransFlag, TurnFlag); }
			//! 画像の自由変形描画
			inline int DrawModiGraph(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4, bool TransFlag)const DXLIBEX_NOEXCEPT { return p_handle->DrawModiGraph(x1, y1, x2, y2, x3, y3, x4, y4, TransFlag); }
			//! 画像の左右反転描画
			inline int DrawTurnGraph(int x, int y, bool TransFlag)const DXLIBEX_NOEXCEPT { return p_handle->DrawTurnGraph(x, y, TransFlag); }
			//! 画像の拡大左右反転描画
			inline int DrawExtendTurnGraph(int x1, int y1, int x2, int y2, bool TransFlag)const DXLIBEX_NOEXCEPT { return p_handle->DrawExtendGraph(x2, y1, x1, y2, TransFlag); }
	
			//! 画像の描画( 座標指定が float 版 )
			inline int DrawGraphF(float xf, float yf, bool TransFlag)const DXLIBEX_NOEXCEPT { return p_handle->DrawGraphF(xf, yf, TransFlag); }
			//! 画像の拡大描画( 座標指定が float 版 )
			inline int DrawExtendGraphF(float x1f, float y1f, float x2f, float y2, bool TransFlag)const DXLIBEX_NOEXCEPT { return p_handle->DrawExtendGraphF(x1f, y1f, x2f, y2, TransFlag); }
			//! 画像の回転描画( 座標指定が float 版 )
			inline int DrawRotaGraphF(float xf, float yf, double ExRate, double Angle, bool TransFlag, bool TurnFlag = FALSE)const DXLIBEX_NOEXCEPT { return p_handle->DrawRotaGraphF(xf, yf, ExRate, Angle, TransFlag, TurnFlag); }
			//! 画像の回転描画２( 回転中心指定付き )( 座標指定が float 版 )
			inline int DrawRotaGraph2F(float xf, float yf, float cxf, float cyf, double ExtRate, double Angle, bool TransFlag, bool TurnFlag = FALSE)const DXLIBEX_NOEXCEPT { return p_handle->DrawRotaGraph2F(xf, yf, cxf, cyf, ExtRate, Angle, TransFlag, TurnFlag); }
			//! 画像の回転描画３( 回転中心指定付き＋縦横拡大率別指定版 )( 座標指定が float 版 )
			inline int DrawRotaGraph3F(float xf, float yf, float cxf, float cyf, double ExtRateX, double ExtRateY, double Angle, bool TransFlag, bool TurnFlag = FALSE)const DXLIBEX_NOEXCEPT { return p_handle->DrawRotaGraph3F(xf, yf, cxf, cyf, ExtRateX, ExtRateY, Angle, TransFlag, TurnFlag); }
			//! 画像の自由変形描画( 座標指定が float 版 )
			inline int DrawModiGraphF(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4, bool TransFlag)const DXLIBEX_NOEXCEPT { return p_handle->DrawModiGraphF(x1, y1, x2, y2, x3, y3, x4, y4, TransFlag); }
			//! 画像の左右反転描画( 座標指定が float 版 )
			inline int DrawTurnGraphF(float xf, float yf, bool TransFlag)const DXLIBEX_NOEXCEPT { return p_handle->DrawTurnGraphF(xf, yf, TransFlag); }
			//! 画像の拡大左右反転描画( 座標指定が float 版 )
			inline int DrawExtendTurnGraphF(float x1f, float y1f, float x2f, float y2f, bool TransFlag)const DXLIBEX_NOEXCEPT { return p_handle->DrawExtendGraphF(x2f, y1f, x1f, y2f, TransFlag); }
	
			//コンストラクタ
	
			inline Texture2D(Texture2D_Unique&& handle) : p_handle(std::make_shared<Texture2D_Unique>(std::move(handle))) {}
			Texture2D() {}
		private:
			std::shared_ptr<Texture2D_Unique> p_handle;
		};
	
		//生成用関数
	
		// グラフィックハンドル作成関係関数
	
		//! 指定サイズのグラフィックハンドルを作成する
		inline Texture2D_Unique MakeGraph(int SizeX, int SizeY, bool NotUse3DFlag = false)DXLIBEX_NOEXCEPT { return Texture2D_Unique::MakeGraph(SizeX, SizeY, NotUse3DFlag); }
		//! SetDrawScreen で描画対象にできるグラフィックハンドルを作成する
		inline Texture2D_Unique MakeScreen(int SizeX, int SizeY, bool UseAlphaChannel = false)DXLIBEX_NOEXCEPT { return Texture2D_Unique::MakeScreen(SizeX, SizeY, UseAlphaChannel); }
		//! 指定のグラフィックハンドルの指定部分だけを抜き出して新たなグラフィックハンドルを作成する
		inline Texture2D_Unique DerivationGraph(int SrcX, int SrcY, int Width, int Height, int SrcGraphHandle)DXLIBEX_NOEXCEPT { return Texture2D_Unique::DerivationGraph(SrcX, SrcY, Width, Height, SrcGraphHandle); }
	
		// 画像からグラフィックハンドルを作成する関数
	
		//! 画像ファイルからグラフィックハンドルを作成する
		inline Texture2D_Unique LoadBmpToGraph(const TCHAR *FileName, int TextureFlag, int ReverseFlag, int SurfaceMode = DX_MOVIESURFACE_NORMAL)DXLIBEX_NOEXCEPT { return Texture2D_Unique::LoadBmpToGraph(FileName, TextureFlag, ReverseFlag, SurfaceMode); }
		//! 画像ファイルからグラフィックハンドルを作成する
		inline Texture2D_Unique LoadGraph(const TCHAR *FileName, bool NotUse3DFlag = false)DXLIBEX_NOEXCEPT { return Texture2D_Unique::LoadGraph(FileName, NotUse3DFlag); }
		//! 画像ファイルを反転したものでグラフィックハンドルを作成する
		inline Texture2D_Unique LoadReverseGraph(const TCHAR *FileName, bool NotUse3DFlag = false)DXLIBEX_NOEXCEPT { return Texture2D_Unique::LoadReverseGraph(FileName, NotUse3DFlag); }
		//! 画像ファイルからブレンド用グラフィックハンドルを作成する
		inline Texture2D_Unique LoadBlendGraph(const TCHAR *FileName)DXLIBEX_NOEXCEPT { return Texture2D_Unique::LoadBlendGraph(FileName); }
	
		//! メモリ上の画像イメージからグラフィックハンドルを作成する
		inline Texture2D_Unique CreateGraphFromMem(const void *RGBFileImage, int RGBFileImageSize, const void *AlphaFileImage = nullptr, int AlphaFileImageSize = 0, bool TextureFlag = true, bool ReverseFlag = false)DXLIBEX_NOEXCEPT { return Texture2D_Unique::CreateGraphFromMem(RGBFileImage, RGBFileImageSize, AlphaFileImage, AlphaFileImageSize, TextureFlag, ReverseFlag); }
		//! メモリ上の画像イメージから既存のグラフィックハンドルにデータを転送する
		inline int ReCreateGraphFromMem(const void *RGBFileImage, int RGBFileImageSize, Texture2D_Unique& GrHandle, const void *AlphaFileImage = nullptr, int AlphaFileImageSize = 0, bool TextureFlag = true, bool ReverseFlag = false)DXLIBEX_NOEXCEPT { return Texture2D_Unique::ReCreateGraphFromMem(RGBFileImage, RGBFileImageSize, GrHandle, AlphaFileImage, AlphaFileImageSize, TextureFlag, ReverseFlag); }
	
		//! 基本イメージデータからサイズを割り出し、それに合ったグラフィックハンドルを作成する
		inline Texture2D_Unique CreateDXGraph(const BASEIMAGE *RgbBaseImage, const BASEIMAGE *AlphaBaseImage, bool TextureFlag)DXLIBEX_NOEXCEPT { return Texture2D_Unique::CreateDXGraph(RgbBaseImage, AlphaBaseImage, TextureFlag); }
		//! 基本イメージデータからグラフィックハンドルを作成する
		inline Texture2D_Unique CreateGraphFromGraphImage(const BASEIMAGE *RgbBaseImage, bool TextureFlag = true, bool ReverseFlag = false)DXLIBEX_NOEXCEPT { return Texture2D_Unique::CreateGraphFromGraphImage(RgbBaseImage, TextureFlag, ReverseFlag); }
		//! 基本イメージデータからグラフィックハンドルを作成する
		inline Texture2D_Unique CreateGraphFromGraphImage(const BASEIMAGE *RgbBaseImage, const BASEIMAGE *AlphaBaseImage, bool TextureFlag = true, bool ReverseFlag = false)DXLIBEX_NOEXCEPT { return Texture2D_Unique::CreateGraphFromGraphImage(RgbBaseImage, AlphaBaseImage, TextureFlag, ReverseFlag); }
		//! 基本イメージデータから既存のグラフィックハンドルにデータを転送する
		inline int ReCreateGraphFromGraphImage(const BASEIMAGE *RgbBaseImage, Texture2D_Unique& GrHandle, bool TextureFlag = true, bool ReverseFlag = false)DXLIBEX_NOEXCEPT { return Texture2D_Unique::ReCreateGraphFromGraphImage(RgbBaseImage, GrHandle, TextureFlag, ReverseFlag); }
		//! 基本イメージデータから既存のグラフィックハンドルにデータを転送する
		inline int ReCreateGraphFromGraphImage(const BASEIMAGE *RgbBaseImage, const BASEIMAGE *AlphaBaseImage, Texture2D_Unique& GrHandle, bool TextureFlag = true, bool ReverseFlag = false)DXLIBEX_NOEXCEPT { return Texture2D_Unique::ReCreateGraphFromGraphImage(RgbBaseImage, AlphaBaseImage, GrHandle, TextureFlag, ReverseFlag); }
	
		//! メモリ上のビットマップイメージからグラフィックハンドルを作成する
		inline Texture2D_Unique CreateGraph(int Width, int Height, int Pitch, const void *RGBImage, const void *AlphaImage = nullptr, const Texture2D_Unique& GrHandle = Texture2D_Unique())DXLIBEX_NOEXCEPT { return Texture2D_Unique::CreateGraph(Width, Height, Pitch, RGBImage, AlphaImage, GrHandle); }
		//! メモリ上のビットマップイメージからグラフィックハンドルを再作成する
		inline int ReCreateGraph(int Width, int Height, int Pitch, const void *RGBImage, Texture2D_Unique& GrHandle, const void *AlphaImage = nullptr)DXLIBEX_NOEXCEPT { return Texture2D_Unique::ReCreateGraph(Width, Height, Pitch, RGBImage, GrHandle, AlphaImage); }
	#ifndef DX_NON_SOFTIMAGE
	#endif // DX_NON_SOFTIMAGE
		//! 基本イメージデータからグラフィックハンドルを作成する
		inline Texture2D_Unique CreateGraphFromBaseImage(const BASEIMAGE *BaseImage)DXLIBEX_NOEXCEPT { return Texture2D_Unique::CreateGraphFromBaseImage(BaseImage); }
		//! 基本イメージデータの指定の領域を使ってグラフィックハンドルを作成する
		inline Texture2D_Unique CreateGraphFromRectBaseImage(const BASEIMAGE *BaseImage, int x, int y, int SizeX, int SizeY)DXLIBEX_NOEXCEPT { return Texture2D_Unique::CreateGraphFromRectBaseImage(BaseImage, x, y, SizeX, SizeY); }
		//! 基本イメージデータから既存のグラフィックハンドルに画像データを転送する
		inline int ReCreateGraphFromBaseImage(const BASEIMAGE *BaseImage, Texture2D_Unique& GrHandle)DXLIBEX_NOEXCEPT { return Texture2D_Unique::ReCreateGraphFromBaseImage(BaseImage, GrHandle); }
		//! 基本イメージデータの指定の領域を使って既存のグラフィックハンドルに画像データを転送する
		inline int ReCreateGraphFromRectBaseImage(const BASEIMAGE *BaseImage, int x, int y, int SizeX, int SizeY, Texture2D_Unique& GrHandle)DXLIBEX_NOEXCEPT { return Texture2D_Unique::ReCreateGraphFromRectBaseImage(BaseImage, x, y, SizeX, SizeY, GrHandle); }
		//! 画像ファイルからグラフィックハンドルへ画像データを転送する
		inline int ReloadGraph(const TCHAR *FileName, Texture2D_Unique& GrHandle, bool ReverseFlag = false)DXLIBEX_NOEXCEPT { return Texture2D_Unique::ReloadGraph(FileName, GrHandle, ReverseFlag); }
		//! ReloadGraph の画像反転処理追加版
		inline int ReloadReverseGraph(const TCHAR *FileName, Texture2D_Unique& GrHandle)DXLIBEX_NOEXCEPT { return Texture2D_Unique::ReloadReverseGraph(FileName, GrHandle); }
	
	
		//使用用関数
	
		//! 指定のグラフィックハンドルのＡＲＧＢ８イメージを取得する( 現在動画ファイルをグラフィックハンドルで読み込んだ場合のみ使用可能 )
		inline const unsigned int* GetFullColorImage(const Texture2D_Unique& GrHandle)DXLIBEX_NOEXCEPT { return GrHandle.GetFullColorImage(); }
		//! グラフィックメモリ領域のロック
		inline int GraphLock(Texture2D_Unique& GrHandle, int *PitchBuf, void **DataPointBuf, COLORDATA **ColorDataPP = nullptr, bool WriteOnly = false)DXLIBEX_NOEXCEPT { return GrHandle.GraphLock(PitchBuf, DataPointBuf, ColorDataPP, WriteOnly); }
		//! グラフィックメモリ領域のロック解除
		inline int GraphUnLock(Texture2D_Unique& GrHandle)DXLIBEX_NOEXCEPT { return GrHandle.GraphUnLock(); }
		//! グラフィックハンドル専用のＺバッファを持つかどうかを設定する( GrHandle:対象となるグラフィックハンドル( 描画対象として使用可能なグラフィックハンドルのみ有効 )  UseFlag:専用のＺバッファを持つかどうか( TRUE:持つ( デフォルト )  FALSE:持たない )  BitDepth:ビット深度( 16 or 24 or 32 ) )
		inline int SetUseGraphZBuffer(Texture2D_Unique& GrHandle, int UseFlag, int BitDepth = -1)DXLIBEX_NOEXCEPT { return GrHandle.SetUseGraphZBuffer(UseFlag, BitDepth); }
		//! グラフィックハンドルのＺバッファの状態を別のグラフィックハンドルのＺバッファにコピーする( DestGrHandle も SrcGrHandle もＺバッファを持っている描画対象にできるグラフィックハンドルで、サイズが同じであり、且つマルチサンプリング( アンチエイリアス )設定が無いことが条件 )
		inline int CopyGraphZBufferImage(const Texture2D_Unique& GrHandle, Texture2D_Unique& ToGrHandle)DXLIBEX_NOEXCEPT { return GrHandle.CopyGraphZBufferImage(ToGrHandle); }
		//! グラフィックスデバイスのデバイスロスト発生時に指定のグラフィックハンドルを削除するかどうかを設定する( TRUE:デバイスロスト時に削除する  FALSE:デバイスロストが発生しても削除しない )
		inline int SetDeviceLostDeleteGraphFlag(Texture2D_Unique& GrHandle, bool DeleteFlag)DXLIBEX_NOEXCEPT { return GrHandle.SetDeviceLostDeleteGraphFlag(DeleteFlag); }
		//! グラフィックハンドルが持つ画像のサイズを得る
		inline int GetGraphSize(const Texture2D_Unique& GrHandle, int *SizeXBuf, int *SizeYBuf)DXLIBEX_NOEXCEPT { return GrHandle.GetGraphSize(SizeXBuf, SizeYBuf); }
		//! グラフィックハンドルが持つ画像のサイズを得る
		//!@return .first:SizeX .second:SizeY
		inline std::pair<int, int> GetGraphSize(const Texture2D_Unique& GrHandle) DXLIBEX_NOEXCEPT { return GrHandle.GetGraphSize(); }
		//! グラフィックハンドルが持つ一つ目のテクスチャのサイズを得る
		inline int GetGraphTextureSize(const Texture2D_Unique& GrHandle, int *SizeXBuf, int *SizeYBuf)DXLIBEX_NOEXCEPT { return GrHandle.GetGraphTextureSize(SizeXBuf, SizeYBuf); }
		//! グラフィックハンドルが持つ一つ目のテクスチャのサイズを得る
		//!@return .first:SizeX .second:SizeY
		inline std::pair<int, int> GetGraphTextureSize(const Texture2D_Unique& GrHandle) DXLIBEX_NOEXCEPT { return GrHandle.GetGraphTextureSize(); }
		//! グラフィックハンドルが持つテクスチャのミップマップレベル数を取得する
		inline int GetGraphMipmapCount(const Texture2D_Unique& GrHandle) DXLIBEX_NOEXCEPT { return GrHandle.GetGraphMipmapCount(); }
		//! グラフィックハンドルが画像ファイルから読み込まれていた場合、その画像のファイルパスを取得する
		inline int GetGraphFilePath(const Texture2D_Unique& GrHandle, TCHAR FilePathBuffer[])DXLIBEX_NOEXCEPT { return GrHandle.GetGraphFilePath(FilePathBuffer); }
		//! グラフィックハンドルが画像ファイルから読み込まれていた場合、その画像のファイルパスを取得する
		inline std::basic_string<TCHAR> GetGraphFilePath(const Texture2D_Unique& GrHandle) DXLIBEX_NOEXCEPT { return GrHandle.GetGraphFilePath(); }
	
		//! 画像の等倍描画
		inline int DrawGraph(int x, int y, const Texture2D_Unique& GrHandle, bool TransFlag)DXLIBEX_NOEXCEPT { return GrHandle.DrawGraph(x, y, TransFlag); }
		//! 画像の拡大描画
		inline int DrawExtendGraph(int x1, int y1, int x2, int y2, const Texture2D_Unique& GrHandle, bool TransFlag)DXLIBEX_NOEXCEPT { return GrHandle.DrawExtendGraph(x1, y1, x2, y2, TransFlag); }
		//! 画像の回転描画
		inline int DrawRotaGraph(int x, int y, double ExRate, double Angle, const Texture2D_Unique& GrHandle, bool TransFlag, bool TurnFlag = false)DXLIBEX_NOEXCEPT { return GrHandle.DrawRotaGraph(x, y, ExRate, Angle, TransFlag, TurnFlag); }
		//! 画像の回転描画２( 回転中心指定付き )
		inline int DrawRotaGraph2(int x, int y, int cx, int cy, double ExtRate, double Angle, const Texture2D_Unique& GrHandle, bool TransFlag, bool TurnFlag = false)DXLIBEX_NOEXCEPT { return GrHandle.DrawRotaGraph2(x, y, cx, cy, ExtRate, Angle, TransFlag, TurnFlag); }
		//! 画像の回転描画３( 回転中心指定付き＋縦横拡大率別指定版 )
		inline int DrawRotaGraph3(int x, int y, int cx, int cy, double ExtRateX, double ExtRateY, double Angle, const Texture2D_Unique& GrHandle, bool TransFlag, bool TurnFlag = FALSE)DXLIBEX_NOEXCEPT { return GrHandle.DrawRotaGraph3(x, y, cx, cy, ExtRateX, ExtRateY, Angle, TransFlag, TurnFlag); }
		//! 画像の自由変形描画
		inline int DrawModiGraph(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4, const Texture2D_Unique& GrHandle, bool TransFlag)DXLIBEX_NOEXCEPT { return GrHandle.DrawModiGraph(x1, y1, x2, y2, x3, y3, x4, y4, TransFlag); }
		//! 画像の左右反転描画
		inline int DrawTurnGraph(int x, int y, const Texture2D_Unique& GrHandle, bool TransFlag)DXLIBEX_NOEXCEPT { return GrHandle.DrawTurnGraph(x, y, TransFlag); }
		//! 画像の拡大左右反転描画
		inline int DrawExtendTurnGraph(int x1, int y1, int x2, int y2, const Texture2D_Unique& GrHandle, bool TransFlag)DXLIBEX_NOEXCEPT { return GrHandle.DrawExtendGraph(x2, y1, x1, y2, TransFlag); }
	
		//! 画像の描画( 座標指定が float 版 )
		inline int DrawGraphF(float xf, float yf, const Texture2D_Unique& GrHandle, bool TransFlag)DXLIBEX_NOEXCEPT { return GrHandle.DrawGraphF(xf, yf, TransFlag); }
		//! 画像の拡大描画( 座標指定が float 版 )
		inline int DrawExtendGraphF(float x1f, float y1f, float x2f, float y2, const Texture2D_Unique& GrHandle, bool TransFlag)DXLIBEX_NOEXCEPT { return GrHandle.DrawExtendGraphF(x1f, y1f, x2f, y2, TransFlag); }
		//! 画像の回転描画( 座標指定が float 版 )
		inline int DrawRotaGraphF(float xf, float yf, double ExRate, double Angle, const Texture2D_Unique& GrHandle, bool TransFlag, bool TurnFlag = FALSE)DXLIBEX_NOEXCEPT { return GrHandle.DrawRotaGraphF(xf, yf, ExRate, Angle, TransFlag, TurnFlag); }
		//! 画像の回転描画２( 回転中心指定付き )( 座標指定が float 版 )
		inline int DrawRotaGraph2F(float xf, float yf, float cxf, float cyf, double ExtRate, double Angle, const Texture2D_Unique& GrHandle, bool TransFlag, bool TurnFlag = FALSE)DXLIBEX_NOEXCEPT { return GrHandle.DrawRotaGraph2F(xf, yf, cxf, cyf, ExtRate, Angle, TransFlag, TurnFlag); }
		//! 画像の回転描画３( 回転中心指定付き＋縦横拡大率別指定版 )( 座標指定が float 版 )
		inline int DrawRotaGraph3F(float xf, float yf, float cxf, float cyf, double ExtRateX, double ExtRateY, double Angle, const Texture2D_Unique& GrHandle, bool TransFlag, bool TurnFlag = FALSE)DXLIBEX_NOEXCEPT { return GrHandle.DrawRotaGraph3F(xf, yf, cxf, cyf, ExtRateX, ExtRateY, Angle, TransFlag, TurnFlag); }
		//! 画像の自由変形描画( 座標指定が float 版 )
		inline int DrawModiGraphF(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4, const Texture2D_Unique& GrHandle, bool TransFlag)DXLIBEX_NOEXCEPT { return GrHandle.DrawModiGraphF(x1, y1, x2, y2, x3, y3, x4, y4, TransFlag); }
		//! 画像の左右反転描画( 座標指定が float 版 )
		inline int DrawTurnGraphF(float xf, float yf, const Texture2D_Unique& GrHandle, bool TransFlag)DXLIBEX_NOEXCEPT { return GrHandle.DrawTurnGraphF(xf, yf, TransFlag); }
		//! 画像の拡大左右反転描画( 座標指定が float 版 )
		inline int DrawExtendTurnGraphF(float x1f, float y1f, float x2f, float y2f, const Texture2D_Unique& GrHandle, bool TransFlag)DXLIBEX_NOEXCEPT { return GrHandle.DrawExtendGraphF(x2f, y1f, x1f, y2f, TransFlag); }
	
	}
	namespace DxLibEx_Classes {
		using Graph2D::Texture2D_Unique;
		using Graph2D::Texture2D;
	}
}

#endif
