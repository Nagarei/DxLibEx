#ifndef DX_LIB_EX_GRAPH_2D_H_201510241561
#define DX_LIB_EX_GRAPH_2D_H_201510241561

#include "DxLib.h"
#include <type_traits>
#include <utility>
#include <memory>
#include <string>
#include "DxLibEx_Helper.h"
#include "DxLibEx_Defines.h"
#include "DxLibEx_basic_types.hpp"

//----------2Dグラフィック----------//

namespace dxle
{
	//!\~japanese DxLibExの公開クラスが全て入ったnamespaceです
	namespace DxLibEx_Classes {}
	//! 2Dグラフィック
	namespace Graph2D
	{

		//!\~japanese 画像クラス(画像ハンドルクラスではない)
		//!\~english  A image class (NOT a image handle class)
		class Texture2D : public impl::Unique_HandledObject_Bace<Texture2D>
		{
		public:
			//!\~japanese 画像を削除する
			//!\~english  Delete this image
			inline void Delete(bool LogOutFlag = false) { DeleteGraph(GetHandle(), LogOutFlag); }

			//生成用static関数
	
			// グラフィック作成関係関数
	
			//!\~japanese 指定サイズのグラフィックを作成する
			//!\~english  Create image with sizes
			static inline Texture2D MakeGraph(int SizeX, int SizeY, bool NotUse3DFlag = false)DXLIBEX_NOEXCEPT{ return Texture2D(DxLib::MakeGraph(SizeX, SizeY, NotUse3DFlag), NotUse3DFlag); }
			//! 指定のグラフィックの指定部分だけを抜き出して新たなグラフィックを作成する
			static inline Texture2D DerivationGraph(int SrcX, int SrcY, int Width, int Height, int SrcGraphHandle)DXLIBEX_NOEXCEPT{ return Texture2D(DxLib::DerivationGraph(SrcX, SrcY, Width, Height, SrcGraphHandle), false); }
	
			// 画像からグラフィックを作成する関数
	
			//! 画像ファイルからグラフィックを作成する
			static inline Texture2D LoadBmpToGraph(const TCHAR *FileName, int TextureFlag, int ReverseFlag, int SurfaceMode = DX_MOVIESURFACE_NORMAL)DXLIBEX_NOEXCEPT{ return Texture2D(DxLib::LoadBmpToGraph(FileName, TextureFlag, ReverseFlag, SurfaceMode), false); }
			//! 画像ファイルからグラフィックを作成する
			static inline Texture2D LoadGraph(const TCHAR *FileName, bool NotUse3DFlag = false)DXLIBEX_NOEXCEPT{ return Texture2D(DxLib::LoadGraph(FileName, NotUse3DFlag), NotUse3DFlag); }
			//! 画像ファイルを反転したものでグラフィックを作成する
			static inline Texture2D LoadReverseGraph(const TCHAR *FileName, bool NotUse3DFlag = false)DXLIBEX_NOEXCEPT{ return Texture2D(DxLib::LoadReverseGraph(FileName, NotUse3DFlag), NotUse3DFlag); }
			//! 画像ファイルからブレンド用グラフィックを作成する
			static inline Texture2D LoadBlendGraph(const TCHAR *FileName)DXLIBEX_NOEXCEPT{ return Texture2D(DxLib::LoadBlendGraph(FileName), false); }
	
			//! メモリ上の画像イメージからグラフィックを作成する
			static inline Texture2D CreateGraphFromMem(const void *RGBFileImage, int RGBFileImageSize, const void *AlphaFileImage = nullptr, int AlphaFileImageSize = 0, bool TextureFlag = true, bool ReverseFlag = false)DXLIBEX_NOEXCEPT{ return Texture2D(DxLib::CreateGraphFromMem(RGBFileImage, RGBFileImageSize, AlphaFileImage, AlphaFileImageSize, TextureFlag, ReverseFlag), false); }
			//! メモリ上の画像イメージから既存のグラフィックにデータを転送する
			static inline int ReCreateGraphFromMem(const void *RGBFileImage, int RGBFileImageSize, Texture2D& GrHandle, const void *AlphaFileImage = nullptr, int AlphaFileImageSize = 0, bool TextureFlag = true, bool ReverseFlag = false)DXLIBEX_NOEXCEPT { return DxLib::ReCreateGraphFromMem(RGBFileImage, RGBFileImageSize, GrHandle.GetHandle(), AlphaFileImage, AlphaFileImageSize, TextureFlag, ReverseFlag); }
	
			//! 基本イメージデータからサイズを割り出し、それに合ったグラフィックを作成する
			static inline Texture2D CreateDXGraph(const BASEIMAGE *RgbBaseImage, const BASEIMAGE *AlphaBaseImage, bool TextureFlag)DXLIBEX_NOEXCEPT{ return Texture2D(DxLib::CreateDXGraph(RgbBaseImage, AlphaBaseImage, TextureFlag), false); }
			//! 基本イメージデータからグラフィックを作成する
			static inline Texture2D CreateGraphFromGraphImage(const BASEIMAGE *RgbBaseImage, bool TextureFlag = true, bool ReverseFlag = false)DXLIBEX_NOEXCEPT{ return Texture2D(DxLib::CreateGraphFromGraphImage(RgbBaseImage, TextureFlag, ReverseFlag), false); }
			//! 基本イメージデータからグラフィックを作成する
			static inline Texture2D CreateGraphFromGraphImage(const BASEIMAGE *RgbBaseImage, const BASEIMAGE *AlphaBaseImage, bool TextureFlag = true, bool ReverseFlag = false)DXLIBEX_NOEXCEPT{ return Texture2D(DxLib::CreateGraphFromGraphImage(RgbBaseImage, AlphaBaseImage, TextureFlag, ReverseFlag), false); }
			//! 基本イメージデータから既存のグラフィックにデータを転送する
			static inline int ReCreateGraphFromGraphImage(const BASEIMAGE *RgbBaseImage, Texture2D& GrHandle, bool TextureFlag = true, bool ReverseFlag = false)DXLIBEX_NOEXCEPT { return DxLib::ReCreateGraphFromGraphImage(RgbBaseImage, GrHandle.GetHandle(), TextureFlag, ReverseFlag); }
			//! 基本イメージデータから既存のグラフィックにデータを転送する
			static inline int ReCreateGraphFromGraphImage(const BASEIMAGE *RgbBaseImage, const BASEIMAGE *AlphaBaseImage, Texture2D& GrHandle, bool TextureFlag = true, bool ReverseFlag = false)DXLIBEX_NOEXCEPT { return DxLib::ReCreateGraphFromGraphImage(RgbBaseImage, AlphaBaseImage, GrHandle.GetHandle(), TextureFlag, ReverseFlag); }
	
			//! メモリ上のビットマップイメージからグラフィックを作成する
			static inline Texture2D CreateGraph(int Width, int Height, int Pitch, const void *RGBImage, const void *AlphaImage = nullptr, const Texture2D& GrHandle = Texture2D())DXLIBEX_NOEXCEPT { return Texture2D(DxLib::CreateGraph(Width, Height, Pitch, RGBImage, AlphaImage, GrHandle.GetHandle()), false); }
			//! メモリ上のビットマップイメージからグラフィックを再作成する
			static inline int ReCreateGraph(int Width, int Height, int Pitch, const void *RGBImage, Texture2D& GrHandle, const void *AlphaImage = nullptr)DXLIBEX_NOEXCEPT { return DxLib::ReCreateGraph(Width, Height, Pitch, RGBImage, GrHandle.GetHandle(), AlphaImage); }
	#ifndef DX_NON_SOFTIMAGE
			//extern	int			CreateBlendGraphFromSoftImage(int SIHandle);																													// ソフトウエアで扱うイメージからブレンド用画像グラフィックハンドルを作成する( -1:エラー  -1以外:ブレンド用グラフィックハンドル )
			//extern	int			CreateGraphFromSoftImage(int SIHandle);																													// ソフトウエアで扱うイメージからグラフィックハンドルを作成する( -1:エラー  -1以外:グラフィックハンドル )
			//extern	int			CreateGraphFromRectSoftImage(int SIHandle, int x, int y, int SizeX, int SizeY);																				// ソフトウエアで扱うイメージの指定の領域を使ってグラフィックハンドルを作成する( -1:エラー  -1以外:グラフィックハンドル )
			//extern	int			ReCreateGraphFromSoftImage(int SIHandle, int GrHandle);																										// ソフトウエアで扱うイメージから既存のグラフィックハンドルに画像データを転送する
			//extern	int			ReCreateGraphFromRectSoftImage(int SIHandle, int x, int y, int SizeX, int SizeY, int GrHandle);																	// ソフトウエアで扱うイメージから既存のグラフィックハンドルに画像データを転送する
			//extern	int			CreateDivGraphFromSoftImage(int SIHandle, int AllNum, int XNum, int YNum, int SizeX, int SizeY, int *HandleBuf);												// ソフトウエアで扱うイメージから分割グラフィックハンドルを作成する
	#endif // DX_NON_SOFTIMAGE
			//! 基本イメージデータからグラフィックを作成する
			static inline Texture2D CreateGraphFromBaseImage(const BASEIMAGE *BaseImage)DXLIBEX_NOEXCEPT{ return Texture2D(DxLib::CreateGraphFromBaseImage(BaseImage), false); }
			//! 基本イメージデータの指定の領域を使ってグラフィックを作成する
			static inline Texture2D CreateGraphFromRectBaseImage(const BASEIMAGE *BaseImage, int x, int y, int SizeX, int SizeY)DXLIBEX_NOEXCEPT{ return Texture2D(DxLib::CreateGraphFromRectBaseImage(BaseImage, x, y, SizeX, SizeY), false); }
			//! 基本イメージデータから既存のグラフィックに画像データを転送する
			static inline int ReCreateGraphFromBaseImage(const BASEIMAGE *BaseImage, Texture2D& GrHandle)DXLIBEX_NOEXCEPT { return DxLib::ReCreateGraphFromBaseImage(BaseImage, GrHandle.GetHandle()); }
			//! 基本イメージデータの指定の領域を使って既存のグラフィックに画像データを転送する
			static inline int ReCreateGraphFromRectBaseImage(const BASEIMAGE *BaseImage, int x, int y, int SizeX, int SizeY, Texture2D& GrHandle)DXLIBEX_NOEXCEPT { return DxLib::ReCreateGraphFromRectBaseImage(BaseImage, x, y, SizeX, SizeY, GrHandle.GetHandle()); }
			//! 画像ファイルからグラフィックへ画像データを転送する
			static inline int ReloadGraph(const TCHAR *FileName, Texture2D& GrHandle, bool ReverseFlag = false)DXLIBEX_NOEXCEPT { return DxLib::ReloadGraph(FileName, GrHandle.GetHandle(), ReverseFlag); }
			//! ReloadGraph の画像反転処理追加版
			static inline int ReloadReverseGraph(const TCHAR *FileName, Texture2D& GrHandle)DXLIBEX_NOEXCEPT { return DxLib::ReloadReverseGraph(FileName, GrHandle.GetHandle()); }
	
	
			//メンバ関数
	
			////! 指定のグラフィックのＡＲＧＢ８イメージを取得する( 現在動画ファイルをグラフィックハンドルで読み込んだ場合のみ使用可能 )
			//inline const unsigned int* GetFullColorImage()const DXLIBEX_NOEXCEPT { return DxLib::GetFullColorImage(GetHandle()); }

			//! グラフィックメモリ領域のロック
			inline int GraphLock(int *PitchBuf, void **DataPointBuf, COLORDATA **ColorDataPP = nullptr, bool WriteOnly = false)DXLIBEX_NOEXCEPT { return DxLib::GraphLock(GetHandle(), PitchBuf, DataPointBuf, ColorDataPP, WriteOnly); }
			//! グラフィックメモリ領域のロック解除
			inline int GraphUnLock()DXLIBEX_NOEXCEPT { return DxLib::GraphUnLock(GetHandle()); }
			//! グラフィックスデバイスのデバイスロスト発生時に指定のグラフィックを削除するかどうかを設定する( TRUE:デバイスロスト時に削除する  FALSE:デバイスロストが発生しても削除しない )
			inline int SetDeviceLostDeleteGraphFlag(bool DeleteFlag)DXLIBEX_NOEXCEPT { return DxLib::SetDeviceLostDeleteGraphFlag(GetHandle(), DeleteFlag); }
			//! グラフィックのサイズを得る
			inline int GetGraphSize(int *SizeXBuf, int *SizeYBuf)const DXLIBEX_NOEXCEPT { return DxLib::GetGraphSize(GetHandle(), SizeXBuf, SizeYBuf); }
			//! グラフィックのサイズを得る
			//!@return .first:SizeX .second:SizeY
			inline pointi GetGraphSize()const DXLIBEX_NOEXCEPT { int x, y; GetGraphSize(&x, &y); return std::make_pair(x, y); }
			//! グラフィックが持つ一つ目のテクスチャのサイズを得る
			inline int GetGraphTextureSize(int *SizeXBuf, int *SizeYBuf)const DXLIBEX_NOEXCEPT { return DxLib::GetGraphTextureSize(GetHandle(), SizeXBuf, SizeYBuf); }
			//! グラフィックが持つ一つ目のテクスチャのサイズを得る
			//!@return .first:SizeX .second:SizeY
			inline pointi GetGraphTextureSize()const DXLIBEX_NOEXCEPT { int x, y; GetGraphTextureSize(&x, &y); return std::make_pair(x, y); }
			//! グラフィックが持つテクスチャのミップマップレベル数を取得する
			inline int GetGraphMipmapCount()const DXLIBEX_NOEXCEPT { return DxLib::GetGraphMipmapCount(GetHandle()); }
			//! グラフィックが画像ファイルから読み込まれていた場合、その画像のファイルパスを取得する
			inline int GetGraphFilePath(TCHAR FilePathBuffer[])const DXLIBEX_NOEXCEPT { return DxLib::GetGraphFilePath(GetHandle(), FilePathBuffer); }
			//! グラフィックが画像ファイルから読み込まれていた場合、その画像のファイルパスを取得する
			inline std::basic_string<TCHAR> GetGraphFilePath()const DXLIBEX_NOEXCEPT { TCHAR FilePathBuffer[2048]; if (GetGraphFilePath(FilePathBuffer) == -1) { return std::basic_string<TCHAR>(); } return FilePathBuffer; }
	
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
			typedef Unique_HandledObject_Bace<Texture2D> Parent_T;
		public:
			Texture2D() : Unique_HandledObject_Bace() {}
			//! コピー禁止
			Texture2D(const Texture2D& other) = delete;
			//!所有権の譲渡
			Texture2D(Texture2D&& other) : Unique_HandledObject_Bace(std::move(other)), NotUse3DFlag(std::move(other.NotUse3DFlag)) {}
			//! コピー禁止
			Texture2D& operator=(const Texture2D& other) = delete;
			//! 所有権の譲渡
			Texture2D& operator=(Texture2D&& other) { Parent_T::operator=(std::move(other)); NotUse3DFlag = (std::move(other.NotUse3DFlag)); return *this; }

			virtual ~Texture2D()DXLIBEX_NOEXCEPT {}
		protected:
			Texture2D(int param_handle, bool param_NotUse3DFlag)DXLIBEX_NOEXCEPT : Unique_HandledObject_Bace(param_handle), NotUse3DFlag(param_NotUse3DFlag){}
			bool NotUse3DFlag;

			//Screen等の子クラスからGetHandleにアクセスできるようにする為
			static int GetTexture2DHandle(const Texture2D& texture2d_obj) { return texture2d_obj.GetHandle(); }
		};
		//! 描画可能画像クラス
		class Screen : public Texture2D
		{
		public:
			//!画像を複製する
			Screen clone()const;
			//!画像を複製する
			Texture2D cloneGr()const;

			//生成用static関数

			//! SetDrawScreen で描画対象にできるグラフィックを作成する
			static inline Screen MakeScreen(int SizeX, int SizeY, bool UseAlphaChannel = false)DXLIBEX_NOEXCEPT { return Screen(DxLib::MakeScreen(SizeX, SizeY, UseAlphaChannel), UseAlphaChannel); }

			//メンバ関数

			template<typename Func_T>
			void DrawnOn(Func_T&& draw_func) {
				auto old_draw_screen = DxLib::GetDrawScreen();
				this->SetDrawScreen();
				struct Finary_ { ~Finary_() {
					DxLib::SetDrawScreen(old_draw_screen);
				} }finally_;

				draw_func();
			}

			//! グラフィック専用のＺバッファを持つかどうかを設定する
			//!@param UseFlag 専用のＺバッファを持つかどうか( true:持つ( デフォルト )  false:持たない )
			//!@param BitDepth ビット深度( 16 or 24 or 32 ) )
			inline int SetUseGraphZBuffer(bool UseFlag, int BitDepth = -1)DXLIBEX_NOEXCEPT { return DxLib::SetUseGraphZBuffer(GetHandle(), UseFlag, BitDepth); }
			//! グラフィックのＺバッファの状態を別のグラフィックのＺバッファにコピーする
			//! *thisはＺバッファを持っている描画対象にできるグラフィック
			//!@param ToGraph *thisとサイズが同じであり、且つマルチサンプリング( アンチエイリアス )設定が無い、Ｚバッファを持っている描画対象にできるグラフィック
			inline int CopyGraphZBufferImage(Screen& ToGraph)const DXLIBEX_NOEXCEPT { return DxLib::CopyGraphZBufferImage(GetHandle(), ToGraph.GetHandle()); }
			//! SetDrawScreen で描画対象にできるグラフィックハンドルから指定領域の画像情報を別のグラフィックハンドルに転送する
			//!@param x1 転送元の矩形の左座標
			//!@param y1 転送元の矩形の上座標
			//!@param x2 転送元の矩形の右座標
			//!@param y2 転送元の矩形の下座標
			//!@param DestX, DestY 転送先の矩形の左上座標
			//!@param DestGrHandle 転送先の画像
			inline int BltDrawValidGraph(int x1, int y1, int x2, int y2, int DestX, int DestY, Texture2D& DestGrHandle)const DXLIBEX_NOEXCEPT{ return DxLib::BltDrawValidGraph(GetHandle(), x1, y1, x2, y2, DestX, DestY, Texture2D::GetTexture2DHandle(DestGrHandle)); }

			//! 描画先画面を設定する
			inline int SetDrawScreen()const DXLIBEX_NOEXCEPT { return DxLib::SetDrawScreen(GetHandle()); }

		public:
			Screen() : Texture2D() {}
			//! コピー禁止
			Screen(const Screen& other) = delete;
			//!所有権の譲渡
			Screen(Screen&& other) : Texture2D(std::move(other)), UseAlphaChannel(std::move(other.UseAlphaChannel)) {}
			//! コピー禁止
			Screen& operator=(const Screen& other) = delete;
			//! 所有権の譲渡
			Screen& operator=(Screen&& other) { Texture2D::operator=(std::move(other)); UseAlphaChannel = (std::move(other.UseAlphaChannel)); return *this; }

			virtual ~Screen()DXLIBEX_NOEXCEPT {}
		protected:
			Screen(int param_handle, bool param_UseAlphaChannel)DXLIBEX_NOEXCEPT : Texture2D(param_handle, false), UseAlphaChannel(param_UseAlphaChannel) {}
			bool UseAlphaChannel;
		};
		//! 複製
		inline Screen Screen::clone()const
		{
			int graph_size_x, graph_size_y;
			GetGraphSize(&graph_size_x, &graph_size_y);

			Screen new_screen = Screen::MakeScreen(graph_size_x, graph_size_y, UseAlphaChannel);
			this->BltDrawValidGraph(0, 0, graph_size_x, graph_size_y, 0, 0, new_screen);
			return std::move(new_screen);
		}
		inline Texture2D Screen::cloneGr()const
		{
			int graph_size_x, graph_size_y;
			GetGraphSize(&graph_size_x, &graph_size_y);

			Texture2D new_graph = Texture2D::MakeGraph(graph_size_x, graph_size_y, NotUse3DFlag);
			this->BltDrawValidGraph(0, 0, graph_size_x, graph_size_y, 0, 0, new_graph);
			return std::move(new_graph);
		}
	
		//生成用関数
	
		// グラフィック作成関係関数
	
		//! 指定サイズのグラフィックを作成する
		inline Texture2D MakeGraph(int SizeX, int SizeY, bool NotUse3DFlag = false)DXLIBEX_NOEXCEPT { return Texture2D::MakeGraph(SizeX, SizeY, NotUse3DFlag); }
		//! SetDrawScreen で描画対象にできるグラフィックを作成する
		inline Screen MakeScreen(int SizeX, int SizeY, bool UseAlphaChannel = false)DXLIBEX_NOEXCEPT { return Screen::MakeScreen(SizeX, SizeY, UseAlphaChannel); }
		//! 指定のグラフィックの指定部分だけを抜き出して新たなグラフィックを作成する
		inline Texture2D DerivationGraph(int SrcX, int SrcY, int Width, int Height, int SrcGraphHandle)DXLIBEX_NOEXCEPT { return Texture2D::DerivationGraph(SrcX, SrcY, Width, Height, SrcGraphHandle); }
	
		// 画像からグラフィックを作成する関数
	
		//! 画像ファイルからグラフィックを作成する
		inline Texture2D LoadBmpToGraph(const TCHAR *FileName, int TextureFlag, int ReverseFlag, int SurfaceMode = DX_MOVIESURFACE_NORMAL)DXLIBEX_NOEXCEPT { return Texture2D::LoadBmpToGraph(FileName, TextureFlag, ReverseFlag, SurfaceMode); }
		//! 画像ファイルからグラフィックを作成する
		inline Texture2D LoadGraph(const TCHAR *FileName, bool NotUse3DFlag = false)DXLIBEX_NOEXCEPT { return Texture2D::LoadGraph(FileName, NotUse3DFlag); }
		//! 画像ファイルを反転したものでグラフィックを作成する
		inline Texture2D LoadReverseGraph(const TCHAR *FileName, bool NotUse3DFlag = false)DXLIBEX_NOEXCEPT { return Texture2D::LoadReverseGraph(FileName, NotUse3DFlag); }
		//! 画像ファイルからブレンド用グラフィックを作成する
		inline Texture2D LoadBlendGraph(const TCHAR *FileName)DXLIBEX_NOEXCEPT { return Texture2D::LoadBlendGraph(FileName); }
	
		//! メモリ上の画像イメージからグラフィックを作成する
		inline Texture2D CreateGraphFromMem(const void *RGBFileImage, int RGBFileImageSize, const void *AlphaFileImage = nullptr, int AlphaFileImageSize = 0, bool TextureFlag = true, bool ReverseFlag = false)DXLIBEX_NOEXCEPT { return Texture2D::CreateGraphFromMem(RGBFileImage, RGBFileImageSize, AlphaFileImage, AlphaFileImageSize, TextureFlag, ReverseFlag); }
		//! メモリ上の画像イメージから既存のグラフィックにデータを転送する
		inline int ReCreateGraphFromMem(const void *RGBFileImage, int RGBFileImageSize, Texture2D& GrHandle, const void *AlphaFileImage = nullptr, int AlphaFileImageSize = 0, bool TextureFlag = true, bool ReverseFlag = false)DXLIBEX_NOEXCEPT { return Texture2D::ReCreateGraphFromMem(RGBFileImage, RGBFileImageSize, GrHandle, AlphaFileImage, AlphaFileImageSize, TextureFlag, ReverseFlag); }
	
		//! 基本イメージデータからサイズを割り出し、それに合ったグラフィックを作成する
		inline Texture2D CreateDXGraph(const BASEIMAGE *RgbBaseImage, const BASEIMAGE *AlphaBaseImage, bool TextureFlag)DXLIBEX_NOEXCEPT { return Texture2D::CreateDXGraph(RgbBaseImage, AlphaBaseImage, TextureFlag); }
		//! 基本イメージデータからグラフィックを作成する
		inline Texture2D CreateGraphFromGraphImage(const BASEIMAGE *RgbBaseImage, bool TextureFlag = true, bool ReverseFlag = false)DXLIBEX_NOEXCEPT { return Texture2D::CreateGraphFromGraphImage(RgbBaseImage, TextureFlag, ReverseFlag); }
		//! 基本イメージデータからグラフィックを作成する
		inline Texture2D CreateGraphFromGraphImage(const BASEIMAGE *RgbBaseImage, const BASEIMAGE *AlphaBaseImage, bool TextureFlag = true, bool ReverseFlag = false)DXLIBEX_NOEXCEPT { return Texture2D::CreateGraphFromGraphImage(RgbBaseImage, AlphaBaseImage, TextureFlag, ReverseFlag); }
		//! 基本イメージデータから既存のグラフィックにデータを転送する
		inline int ReCreateGraphFromGraphImage(const BASEIMAGE *RgbBaseImage, Texture2D& GrHandle, bool TextureFlag = true, bool ReverseFlag = false)DXLIBEX_NOEXCEPT { return Texture2D::ReCreateGraphFromGraphImage(RgbBaseImage, GrHandle, TextureFlag, ReverseFlag); }
		//! 基本イメージデータから既存のグラフィックにデータを転送する
		inline int ReCreateGraphFromGraphImage(const BASEIMAGE *RgbBaseImage, const BASEIMAGE *AlphaBaseImage, Texture2D& GrHandle, bool TextureFlag = true, bool ReverseFlag = false)DXLIBEX_NOEXCEPT { return Texture2D::ReCreateGraphFromGraphImage(RgbBaseImage, AlphaBaseImage, GrHandle, TextureFlag, ReverseFlag); }
	
		//! メモリ上のビットマップイメージからグラフィックを作成する
		inline Texture2D CreateGraph(int Width, int Height, int Pitch, const void *RGBImage, const void *AlphaImage = nullptr, const Texture2D& GrHandle = Texture2D())DXLIBEX_NOEXCEPT { return Texture2D::CreateGraph(Width, Height, Pitch, RGBImage, AlphaImage, GrHandle); }
		//! メモリ上のビットマップイメージからグラフィックを再作成する
		inline int ReCreateGraph(int Width, int Height, int Pitch, const void *RGBImage, Texture2D& GrHandle, const void *AlphaImage = nullptr)DXLIBEX_NOEXCEPT { return Texture2D::ReCreateGraph(Width, Height, Pitch, RGBImage, GrHandle, AlphaImage); }
	#ifndef DX_NON_SOFTIMAGE
	#endif // DX_NON_SOFTIMAGE
		//! 基本イメージデータからグラフィックを作成する
		inline Texture2D CreateGraphFromBaseImage(const BASEIMAGE *BaseImage)DXLIBEX_NOEXCEPT { return Texture2D::CreateGraphFromBaseImage(BaseImage); }
		//! 基本イメージデータの指定の領域を使ってグラフィックを作成する
		inline Texture2D CreateGraphFromRectBaseImage(const BASEIMAGE *BaseImage, int x, int y, int SizeX, int SizeY)DXLIBEX_NOEXCEPT { return Texture2D::CreateGraphFromRectBaseImage(BaseImage, x, y, SizeX, SizeY); }
		//! 基本イメージデータから既存のグラフィックに画像データを転送する
		inline int ReCreateGraphFromBaseImage(const BASEIMAGE *BaseImage, Texture2D& GrHandle)DXLIBEX_NOEXCEPT { return Texture2D::ReCreateGraphFromBaseImage(BaseImage, GrHandle); }
		//! 基本イメージデータの指定の領域を使って既存のグラフィックに画像データを転送する
		inline int ReCreateGraphFromRectBaseImage(const BASEIMAGE *BaseImage, int x, int y, int SizeX, int SizeY, Texture2D& GrHandle)DXLIBEX_NOEXCEPT { return Texture2D::ReCreateGraphFromRectBaseImage(BaseImage, x, y, SizeX, SizeY, GrHandle); }
		//! 画像ファイルからグラフィックへ画像データを転送する
		inline int ReloadGraph(const TCHAR *FileName, Texture2D& GrHandle, bool ReverseFlag = false)DXLIBEX_NOEXCEPT { return Texture2D::ReloadGraph(FileName, GrHandle, ReverseFlag); }
		//! ReloadGraph の画像反転処理追加版
		inline int ReloadReverseGraph(const TCHAR *FileName, Texture2D& GrHandle)DXLIBEX_NOEXCEPT { return Texture2D::ReloadReverseGraph(FileName, GrHandle); }
	
	}
	namespace DxLibEx_Classes {
		using Graph2D::Texture2D;
	}
}

#endif
