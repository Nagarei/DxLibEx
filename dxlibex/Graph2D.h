/*=============================================================================
  Copyright (C) 2015-2016 DxLibEx project
  https://github.com/Nagarei/DxLibEx/

  Distributed under the Boost Software License, Version 1.0.
  (See http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#ifndef DXLE_INC_GRAPH_2D_H_
#define DXLE_INC_GRAPH_2D_H_

#include "dxlibex/config/no_min_max.h"
#include "DxLib.h"
#include <type_traits>
#include <utility>
#include <memory>
#include <string>
#include <array>
#include "dxlibex/Helper.h"
#include "dxlibex/config/defines.h"
#include "dxlibex/basic_types.hpp"
#include "dxlibex/thread.h"

//----------2Dグラフィック----------//

namespace dxle
{
	//! 2Dグラフィック
	namespace graph2d
	{
		//!\~japanese DxLibの描画先スクリーン同期のためのmutex
		//!\~english  mutex for DxLib's draw screen
		class screen_mutex_c final
		{
		public:
#ifdef DX_THREAD_SAFE
			static std::mutex mtx;
#endif
		};

		//!\~japanese 画像クラス(画像ハンドルクラスではない)
		//!\~english  A image class (NOT an image handle class)
		class texture_2d /*final*/ : private impl::Unique_HandledObject_Bace<texture_2d>
		{
		public:
			texture_2d() : Unique_HandledObject_Bace() {}
			//! コピー禁止
			texture_2d(const texture_2d& other) = delete;
			//!所有権の譲渡
			texture_2d(texture_2d&& other) DXLE_NOEXCEPT_OR_NOTHROW : Unique_HandledObject_Bace(std::move(other)), NotUse3DFlag(std::move(other.NotUse3DFlag)) {}
			//! コピー禁止
			texture_2d& operator=(const texture_2d& other) = delete;
			//! 所有権の譲渡
			texture_2d& operator=(texture_2d&& other) DXLE_NOEXCEPT_OR_NOTHROW{ Unique_HandledObject_Bace<texture_2d>::operator=(std::move(other)); NotUse3DFlag = (std::move(other.NotUse3DFlag)); return *this; }

			//!画像を複製する
			virtual texture_2d cloneGr()const;

			//!\~japanese 画像を削除する
			//!\~english  Delete this image
			inline void delete_this(bool LogOutFlag = false) { DeleteGraph(GetHandle(), LogOutFlag); }

			//! グラフィックのサイズを得る
			inline sizei size()const DXLE_NOEXCEPT_OR_NOTHROW{ return GetGraphSize(); }


			virtual ~texture_2d()DXLE_NOEXCEPT_OR_NOTHROW {}

		public:
			//生成用static関数

			// グラフィック作成関係関数

			//!\~japanese 指定サイズのグラフィックを作成する
			//!\~english  Create an image with sizes
			static inline texture_2d MakeGraph(int SizeX, int SizeY, bool NotUse3DFlag = false)DXLE_NOEXCEPT_OR_NOTHROW { return texture_2d(DxLib::MakeGraph(SizeX, SizeY, NotUse3DFlag), NotUse3DFlag); }
			//!\~japanese 指定サイズのグラフィックを作成する
			//!\~english  Create an image with sizes
			static inline texture_2d MakeGraph(const sizei& size, bool NotUse3DFlag = false)DXLE_NOEXCEPT_OR_NOTHROW { return texture_2d(DxLib::MakeGraph(size.width, size.height, NotUse3DFlag), NotUse3DFlag); }
			//! 指定のグラフィックの指定部分だけを抜き出して新たなグラフィックを作成する
			static inline texture_2d DerivationGraph(int SrcX, int SrcY, int Width, int Height, int SrcGraphHandle)DXLE_NOEXCEPT_OR_NOTHROW { return texture_2d(DxLib::DerivationGraph(SrcX, SrcY, Width, Height, SrcGraphHandle), false); }
			//! 指定のグラフィックの指定部分だけを抜き出して新たなグラフィックを作成する
			static inline texture_2d DerivationGraph(const sizei& src, const sizei& size, int SrcGraphHandle)DXLE_NOEXCEPT_OR_NOTHROW { return texture_2d(DxLib::DerivationGraph(src.width, src.height, size.width, size.height, SrcGraphHandle), false); }

			// 画像からグラフィックを作成する関数

			//!\~japanese 画像ファイルからグラフィックを作成する
			//!\~english  Create an image form an image file
			static inline texture_2d LoadBmpToGraph(const TCHAR *FileName, int TextureFlag, int ReverseFlag, int SurfaceMode = DX_MOVIESURFACE_NORMAL)DXLE_NOEXCEPT_OR_NOTHROW { return texture_2d(DxLib::LoadBmpToGraph(FileName, TextureFlag, ReverseFlag, SurfaceMode), false); }
			//!\~japanese 画像ファイルからグラフィックを作成する
			//!\~english  Create an image form an image file
			static inline texture_2d LoadBmpToGraph(const dxle::tstring& FileName, int TextureFlag, int ReverseFlag, int SurfaceMode = DX_MOVIESURFACE_NORMAL)DXLE_NOEXCEPT_OR_NOTHROW { return texture_2d(DxLib::LoadBmpToGraph(FileName.c_str(), TextureFlag, ReverseFlag, SurfaceMode), false); }
			//!\~japanese 画像ファイルからグラフィックを作成する
			//!\~english  Create an image form an image file
			static inline texture_2d LoadGraph(const TCHAR *FileName, bool NotUse3DFlag = false)DXLE_NOEXCEPT_OR_NOTHROW { return texture_2d(DxLib::LoadGraph(FileName, NotUse3DFlag), NotUse3DFlag); }
			//!\~japanese 画像ファイルからグラフィックを作成する
			//!\~english  Create an image form an image file
			static inline texture_2d LoadGraph(const dxle::tstring& FileName, bool NotUse3DFlag = false)DXLE_NOEXCEPT_OR_NOTHROW { return texture_2d(DxLib::LoadGraph(FileName.c_str(), NotUse3DFlag), NotUse3DFlag); }
			//!\~japanese 画像ファイルからグラフィックを作成する
			//!\~english  Create reverse image form an image file
			static inline texture_2d LoadReverseGraph(const TCHAR *FileName, bool NotUse3DFlag = false)DXLE_NOEXCEPT_OR_NOTHROW { return texture_2d(DxLib::LoadReverseGraph(FileName, NotUse3DFlag), NotUse3DFlag); }
			//!\~japanese 画像ファイルからグラフィックを作成する
			//!\~english  Create reverse image form an image file
			static inline texture_2d LoadReverseGraph(const dxle::tstring& FileName, bool NotUse3DFlag = false)DXLE_NOEXCEPT_OR_NOTHROW { return texture_2d(DxLib::LoadReverseGraph(FileName.c_str(), NotUse3DFlag), NotUse3DFlag); }

			//!\~japanese メモリ上の画像イメージからグラフィックを作成する
			//!\~english  Create an image form an image in the memory
			static inline texture_2d CreateGraphFromMem(const void *RGBFileImage, int RGBFileImageSize, const void *AlphaFileImage = nullptr, int AlphaFileImageSize = 0, bool TextureFlag = true, bool ReverseFlag = false)DXLE_NOEXCEPT_OR_NOTHROW { return texture_2d(DxLib::CreateGraphFromMem(RGBFileImage, RGBFileImageSize, AlphaFileImage, AlphaFileImageSize, TextureFlag, ReverseFlag), false); }
			//!\~japanese メモリ上の画像イメージから既存のグラフィックにデータを転送する
			//!\~english  Forward one image in the memory to the other image
			static inline int ReCreateGraphFromMem(const void *RGBFileImage, int RGBFileImageSize, texture_2d& GrHandle, const void *AlphaFileImage = nullptr, int AlphaFileImageSize = 0, bool TextureFlag = true, bool ReverseFlag = false)DXLE_NOEXCEPT_OR_NOTHROW { return DxLib::ReCreateGraphFromMem(RGBFileImage, RGBFileImageSize, GrHandle.GetHandle(), AlphaFileImage, AlphaFileImageSize, TextureFlag, ReverseFlag); }

			//!\~japanese 基本イメージデータからサイズを割り出し、それに合ったグラフィックを作成する
			//!\~english  Create an image what is fit others BASEIMAGE
			static inline texture_2d CreateDXGraph(const BASEIMAGE *RgbBaseImage, const BASEIMAGE *AlphaBaseImage, bool TextureFlag)DXLE_NOEXCEPT_OR_NOTHROW { return texture_2d(DxLib::CreateDXGraph(RgbBaseImage, AlphaBaseImage, TextureFlag), false); }
			//!\~japanese 基本イメージデータからグラフィックを作成する
			//!\~english  Create an image from a BASEIMAGE
			static inline texture_2d CreateGraphFromGraphImage(const BASEIMAGE *RgbBaseImage, bool TextureFlag = true, bool ReverseFlag = false)DXLE_NOEXCEPT_OR_NOTHROW { return texture_2d(DxLib::CreateGraphFromGraphImage(RgbBaseImage, TextureFlag, ReverseFlag), false); }
			//!\~japanese 基本イメージデータからグラフィックを作成する
			//!\~english  Create an image from others BASEIMAGE
			static inline texture_2d CreateGraphFromGraphImage(const BASEIMAGE *RgbBaseImage, const BASEIMAGE *AlphaBaseImage, bool TextureFlag = true, bool ReverseFlag = false)DXLE_NOEXCEPT_OR_NOTHROW { return texture_2d(DxLib::CreateGraphFromGraphImage(RgbBaseImage, AlphaBaseImage, TextureFlag, ReverseFlag), false); }
			//!\~japanese 基本イメージデータから既存のグラフィックにデータを転送する
			//!\~english  Forward an image from the other BASEIMAGE
			static inline int ReCreateGraphFromGraphImage(const BASEIMAGE *RgbBaseImage, texture_2d& GrHandle, bool TextureFlag = true, bool ReverseFlag = false)DXLE_NOEXCEPT_OR_NOTHROW { return DxLib::ReCreateGraphFromGraphImage(RgbBaseImage, GrHandle.GetHandle(), TextureFlag, ReverseFlag); }
			//!\~japanese 基本イメージデータから既存のグラフィックにデータを転送する
			//!\~english  Forward images from the other BASEIMAGE
			static inline int ReCreateGraphFromGraphImage(const BASEIMAGE *RgbBaseImage, const BASEIMAGE *AlphaBaseImage, texture_2d& GrHandle, bool TextureFlag = true, bool ReverseFlag = false)DXLE_NOEXCEPT_OR_NOTHROW { return DxLib::ReCreateGraphFromGraphImage(RgbBaseImage, AlphaBaseImage, GrHandle.GetHandle(), TextureFlag, ReverseFlag); }

			//! メモリ上のビットマップイメージからグラフィックを作成する
			static inline texture_2d CreateGraph(int Width, int Height, int Pitch, const void *RGBImage, const void *AlphaImage = nullptr, const texture_2d& GrHandle = texture_2d())DXLE_NOEXCEPT_OR_NOTHROW { return texture_2d(DxLib::CreateGraph(Width, Height, Pitch, RGBImage, AlphaImage, GrHandle.GetHandle()), false); }
			//! メモリ上のビットマップイメージからグラフィックを作成する
			static inline texture_2d CreateGraph(const sizei& size, int Pitch, const void *RGBImage, const void *AlphaImage = nullptr, const texture_2d& GrHandle = texture_2d())DXLE_NOEXCEPT_OR_NOTHROW { return texture_2d(DxLib::CreateGraph(size.width, size.height, Pitch, RGBImage, AlphaImage, GrHandle.GetHandle()), false); }
			//! メモリ上のビットマップイメージからグラフィックを再作成する
			static inline int ReCreateGraph(int Width, int Height, int Pitch, const void *RGBImage, texture_2d& GrHandle, const void *AlphaImage = nullptr)DXLE_NOEXCEPT_OR_NOTHROW { return DxLib::ReCreateGraph(Width, Height, Pitch, RGBImage, GrHandle.GetHandle(), AlphaImage); }
			//! メモリ上のビットマップイメージからグラフィックを再作成する
			static inline int ReCreateGraph(const sizei& size, int Pitch, const void *RGBImage, texture_2d& GrHandle, const void *AlphaImage = nullptr)DXLE_NOEXCEPT_OR_NOTHROW { return DxLib::ReCreateGraph(size.width, size.height, Pitch, RGBImage, GrHandle.GetHandle(), AlphaImage); }
#ifndef DX_NON_SOFTIMAGE
			//extern	int			CreateBlendGraphFromSoftImage(int SIHandle);																													// ソフトウエアで扱うイメージからブレンド用画像グラフィックハンドルを作成する( -1:エラー  -1以外:ブレンド用グラフィックハンドル )
			//extern	int			CreateGraphFromSoftImage(int SIHandle);																													// ソフトウエアで扱うイメージからグラフィックハンドルを作成する( -1:エラー  -1以外:グラフィックハンドル )
			//extern	int			CreateGraphFromRectSoftImage(int SIHandle, int x, int y, int SizeX, int SizeY);																				// ソフトウエアで扱うイメージの指定の領域を使ってグラフィックハンドルを作成する( -1:エラー  -1以外:グラフィックハンドル )
			//extern	int			ReCreateGraphFromSoftImage(int SIHandle, int GrHandle);																										// ソフトウエアで扱うイメージから既存のグラフィックハンドルに画像データを転送する
			//extern	int			ReCreateGraphFromRectSoftImage(int SIHandle, int x, int y, int SizeX, int SizeY, int GrHandle);																	// ソフトウエアで扱うイメージから既存のグラフィックハンドルに画像データを転送する
			//extern	int			CreateDivGraphFromSoftImage(int SIHandle, int AllNum, int XNum, int YNum, int SizeX, int SizeY, int *HandleBuf);												// ソフトウエアで扱うイメージから分割グラフィックハンドルを作成する
	#endif // DX_NON_SOFTIMAGE
			//! 基本イメージデータからグラフィックを作成する
			static inline texture_2d CreateGraphFromBaseImage(const BASEIMAGE *BaseImage)DXLE_NOEXCEPT_OR_NOTHROW{ return texture_2d(DxLib::CreateGraphFromBaseImage(BaseImage), false); }
			//! 基本イメージデータの指定の領域を使ってグラフィックを作成する
			static inline texture_2d CreateGraphFromRectBaseImage(const BASEIMAGE *BaseImage, int x, int y, int SizeX, int SizeY)DXLE_NOEXCEPT_OR_NOTHROW{ return texture_2d(DxLib::CreateGraphFromRectBaseImage(BaseImage, x, y, SizeX, SizeY), false); }
			//! 基本イメージデータの指定の領域を使ってグラフィックを作成する
			static inline texture_2d CreateGraphFromRectBaseImage(const BASEIMAGE *BaseImage, const pointi& pos, const sizei& size)DXLE_NOEXCEPT_OR_NOTHROW { return texture_2d(DxLib::CreateGraphFromRectBaseImage(BaseImage, pos.x, pos.y, size.width, size.height), false); }
			//! 基本イメージデータから既存のグラフィックに画像データを転送する
			static inline int ReCreateGraphFromBaseImage(const BASEIMAGE *BaseImage, texture_2d& GrHandle)DXLE_NOEXCEPT_OR_NOTHROW { return DxLib::ReCreateGraphFromBaseImage(BaseImage, GrHandle.GetHandle()); }
			//! 基本イメージデータの指定の領域を使って既存のグラフィックに画像データを転送する
			static inline int ReCreateGraphFromRectBaseImage(const BASEIMAGE *BaseImage, int x, int y, int SizeX, int SizeY, texture_2d& GrHandle)DXLE_NOEXCEPT_OR_NOTHROW { return DxLib::ReCreateGraphFromRectBaseImage(BaseImage, x, y, SizeX, SizeY, GrHandle.GetHandle()); }
			//! 基本イメージデータの指定の領域を使って既存のグラフィックに画像データを転送する
			static inline int ReCreateGraphFromRectBaseImage(const BASEIMAGE *BaseImage, const pointi& pos, const sizei& size, texture_2d& GrHandle)DXLE_NOEXCEPT_OR_NOTHROW { return DxLib::ReCreateGraphFromRectBaseImage(BaseImage, pos.x, pos.y, size.width, size.height, GrHandle.GetHandle()); }
			//! 画像ファイルからグラフィックへ画像データを転送する
			static inline int ReloadGraph(const TCHAR *FileName, texture_2d& GrHandle, bool ReverseFlag = false)DXLE_NOEXCEPT_OR_NOTHROW { return DxLib::ReloadGraph(FileName, GrHandle.GetHandle(), ReverseFlag); }
			//! 画像ファイルからグラフィックへ画像データを転送する
			static inline int ReloadGraph(const dxle::tstring& FileName, texture_2d& GrHandle, bool ReverseFlag = false)DXLE_NOEXCEPT_OR_NOTHROW { return DxLib::ReloadGraph(FileName.c_str(), GrHandle.GetHandle(), ReverseFlag); }
			//! ReloadGraph の画像反転処理追加版
			static inline int ReloadReverseGraph(const TCHAR *FileName, texture_2d& GrHandle)DXLE_NOEXCEPT_OR_NOTHROW { return DxLib::ReloadReverseGraph(FileName, GrHandle.GetHandle()); }
			//! ReloadGraph の画像反転処理追加版
			static inline int ReloadReverseGraph(const dxle::tstring& FileName, texture_2d& GrHandle)DXLE_NOEXCEPT_OR_NOTHROW { return DxLib::ReloadReverseGraph(FileName.c_str(), GrHandle.GetHandle()); }

	
			//メンバ関数
	
			////! 指定のグラフィックのＡＲＧＢ８イメージを取得する( 現在動画ファイルをグラフィックハンドルで読み込んだ場合のみ使用可能 )
			//inline const unsigned int* GetFullColorImage()const DXLE_NOEXCEPT_OR_NOTHROW { return DxLib::GetFullColorImage(GetHandle()); }

			//! グラフィックメモリ領域のロック
			inline int GraphLock(int *PitchBuf, void **DataPointBuf, COLORDATA **ColorDataPP = nullptr, bool WriteOnly = false)DXLE_NOEXCEPT_OR_NOTHROW { return DxLib::GraphLock(GetHandle(), PitchBuf, DataPointBuf, ColorDataPP, WriteOnly); }
			//! グラフィックメモリ領域のロック解除
			inline int GraphUnLock()DXLE_NOEXCEPT_OR_NOTHROW { return DxLib::GraphUnLock(GetHandle()); }
			//! グラフィックスデバイスのデバイスロスト発生時に指定のグラフィックを削除するかどうかを設定する( TRUE:デバイスロスト時に削除する  FALSE:デバイスロストが発生しても削除しない )
			inline int SetDeviceLostDeleteGraphFlag(bool DeleteFlag)DXLE_NOEXCEPT_OR_NOTHROW { return DxLib::SetDeviceLostDeleteGraphFlag(GetHandle(), DeleteFlag); }
			//! グラフィックのサイズを得る
			inline int GetGraphSize(int *SizeXBuf, int *SizeYBuf)const DXLE_NOEXCEPT_OR_NOTHROW { return DxLib::GetGraphSize(GetHandle(), SizeXBuf, SizeYBuf); }
			//! グラフィックのサイズを得る
			inline sizei GetGraphSize()const DXLE_NOEXCEPT_OR_NOTHROW { int x, y; GetGraphSize(&x, &y); return { x, y }; }
			//! グラフィックが持つ一つ目のテクスチャのサイズを得る
			inline int GetGraphTextureSize(int *SizeXBuf, int *SizeYBuf)const DXLE_NOEXCEPT_OR_NOTHROW { return DxLib::GetGraphTextureSize(GetHandle(), SizeXBuf, SizeYBuf); }
			//! グラフィックが持つ一つ目のテクスチャのサイズを得る
			inline sizei GetGraphTextureSize()const DXLE_NOEXCEPT_OR_NOTHROW { int x, y; GetGraphTextureSize(&x, &y); return { x, y }; }
			//! グラフィックが持つテクスチャのミップマップレベル数を取得する
			inline int GetGraphMipmapCount()const DXLE_NOEXCEPT_OR_NOTHROW { return DxLib::GetGraphMipmapCount(GetHandle()); }
			//! グラフィックが画像ファイルから読み込まれていた場合、その画像のファイルパスを取得する
			inline int GetGraphFilePath(TCHAR FilePathBuffer[])const DXLE_NOEXCEPT_OR_NOTHROW { return DxLib::GetGraphFilePath(GetHandle(), FilePathBuffer); }
			//! グラフィックが画像ファイルから読み込まれていた場合、その画像のファイルパスを取得する
			inline tstring GetGraphFilePath()const {
				tstring buf;
				buf.resize(2048);
				if (this->GetGraphFilePath(&buf[0]) == -1) {
					return {};
				}
				buf.resize(std::strlen(buf.c_str()));
				buf.shrink_to_fit();
				return buf;
			}


			//! 画像の等倍描画
			inline int DrawGraph(int x, int y, bool TransFlag)const DXLE_NOEXCEPT_OR_NOTHROW_SINGLE{ DXLE_GET_LOCK(screen_mutex_c::mtx); return DxLib::DrawGraph(x, y, GetHandle(), TransFlag); }
			//! 画像の等倍描画
			inline int DrawGraph(const pointi& p, bool TransFlag)const DXLE_NOEXCEPT_OR_NOTHROW_SINGLE{ DXLE_GET_LOCK(screen_mutex_c::mtx); return DxLib::DrawGraph(p.x, p.y, GetHandle(), TransFlag); }
			//! 画像の拡大描画
			inline int DrawExtendGraph(int x1, int y1, int x2, int y2, bool TransFlag)const DXLE_NOEXCEPT_OR_NOTHROW_SINGLE{ DXLE_GET_LOCK(screen_mutex_c::mtx); return DxLib::DrawExtendGraph(x1, y1, x2, y2, GetHandle(), TransFlag); }
			//! 画像の拡大描画
			inline int DrawExtendGraph(const pointi& lu, const pointi& rb, bool TransFlag)const DXLE_NOEXCEPT_OR_NOTHROW_SINGLE{ DXLE_GET_LOCK(screen_mutex_c::mtx); return DxLib::DrawExtendGraph(lu.x, lu.y, rb.x, rb.y, GetHandle(), TransFlag); }
			//! 画像の回転描画
			inline int DrawRotaGraph(int x, int y, double ExRate, double Angle, bool TransFlag, bool TurnFlag = false)const DXLE_NOEXCEPT_OR_NOTHROW_SINGLE{ DXLE_GET_LOCK(screen_mutex_c::mtx); return DxLib::DrawRotaGraph(x, y, ExRate, Angle, GetHandle(), TransFlag, TurnFlag); }
			//! 画像の回転描画
			inline int DrawRotaGraph(const pointi& p, double ExRate, double Angle, bool TransFlag, bool TurnFlag = false)const DXLE_NOEXCEPT_OR_NOTHROW_SINGLE{ DXLE_GET_LOCK(screen_mutex_c::mtx); return DxLib::DrawRotaGraph(p.x, p.y, ExRate, Angle, GetHandle(), TransFlag, TurnFlag); }
			//! 画像の回転描画２( 回転中心指定付き )
			inline int DrawRotaGraph2(int x, int y, int cx, int cy, double ExtRate, double Angle, bool TransFlag, bool TurnFlag = false)const DXLE_NOEXCEPT_OR_NOTHROW_SINGLE{ DXLE_GET_LOCK(screen_mutex_c::mtx); return DxLib::DrawRotaGraph2(x, y, cx, cy, ExtRate, Angle, GetHandle(), TransFlag, TurnFlag); }
			//! 画像の回転描画２( 回転中心指定付き )
			inline int DrawRotaGraph2(const pointi& p, const pointi& c, double ExtRate, double Angle, bool TransFlag, bool TurnFlag = false)const DXLE_NOEXCEPT_OR_NOTHROW_SINGLE{ DXLE_GET_LOCK(screen_mutex_c::mtx); return DxLib::DrawRotaGraph2(p.x, p.y, c.x, c.y, ExtRate, Angle, GetHandle(), TransFlag, TurnFlag); }
			//! 画像の回転描画３( 回転中心指定付き＋縦横拡大率別指定版 )
			inline int DrawRotaGraph3(int x, int y, int cx, int cy, double ExtRateX, double ExtRateY, double Angle, bool TransFlag, bool TurnFlag = FALSE)const DXLE_NOEXCEPT_OR_NOTHROW_SINGLE{ DXLE_GET_LOCK(screen_mutex_c::mtx); return DxLib::DrawRotaGraph3(x, y, cx, cy, ExtRateX, ExtRateY, Angle, GetHandle(), TransFlag, TurnFlag); }
			//! 画像の回転描画３( 回転中心指定付き＋縦横拡大率別指定版 )
			inline int DrawRotaGraph3(const pointi& p, const pointi& c, double ExtRateX, double ExtRateY, double Angle, bool TransFlag, bool TurnFlag = FALSE)const DXLE_NOEXCEPT_OR_NOTHROW_SINGLE{ DXLE_GET_LOCK(screen_mutex_c::mtx); return DxLib::DrawRotaGraph3(p.x, p.y, c.x, c.y, ExtRateX, ExtRateY, Angle, GetHandle(), TransFlag, TurnFlag); }
			//! 画像の自由変形描画
			inline int DrawModiGraph(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4, bool TransFlag)const DXLE_NOEXCEPT_OR_NOTHROW_SINGLE{ DXLE_GET_LOCK(screen_mutex_c::mtx); return DxLib::DrawModiGraph(x1, y1, x2, y2, x3, y3, x4, y4, GetHandle(), TransFlag); }
			//! 画像の自由変形描画
			inline int DrawModiGraph(const std::array<pointi, 4>& ps, bool TransFlag)const DXLE_NOEXCEPT_OR_NOTHROW_SINGLE{ DXLE_GET_LOCK(screen_mutex_c::mtx); return DxLib::DrawModiGraph(ps[0].x, ps[0].y, ps[1].x, ps[1].y, ps[2].x, ps[2].y, ps[3].x, ps[3].y, GetHandle(), TransFlag); }
			//! 画像の左右反転描画
			inline int DrawTurnGraph(int x, int y, bool TransFlag)const DXLE_NOEXCEPT_OR_NOTHROW_SINGLE{ DXLE_GET_LOCK(screen_mutex_c::mtx); return DxLib::DrawTurnGraph(x, y, GetHandle(), TransFlag); }
			//! 画像の左右反転描画
			inline int DrawTurnGraph(const pointi& p, bool TransFlag)const DXLE_NOEXCEPT_OR_NOTHROW_SINGLE{ DXLE_GET_LOCK(screen_mutex_c::mtx); return DxLib::DrawTurnGraph(p.x, p.y, GetHandle(), TransFlag); }
			//! 画像の拡大左右反転描画
			inline int DrawExtendTurnGraph(int x1, int y1, int x2, int y2, bool TransFlag)const DXLE_NOEXCEPT_OR_NOTHROW_SINGLE{ DXLE_GET_LOCK(screen_mutex_c::mtx); return DxLib::DrawExtendGraph(x2, y1, x1, y2, GetHandle(), TransFlag); }//x1とx2はこれで正しい
			//! 画像の拡大左右反転描画
			inline int DrawExtendTurnGraph(const pointi& lu, const pointi& rb, bool TransFlag)const DXLE_NOEXCEPT_OR_NOTHROW_SINGLE{ DXLE_GET_LOCK(screen_mutex_c::mtx); return DxLib::DrawExtendGraph(rb.x, lu.y, lu.x, rb.y, GetHandle(), TransFlag); }//lu.xとrb.xはこれで正しい
	
			//! 画像の描画( 座標指定が float 版 )
			inline int DrawGraphF(float xf, float yf, bool TransFlag)const DXLE_NOEXCEPT_OR_NOTHROW_SINGLE{ DXLE_GET_LOCK(screen_mutex_c::mtx); return DxLib::DrawGraphF(xf, yf, GetHandle(), TransFlag); }
			//! 画像の描画( 座標指定が float 版 )
			inline int DrawGraphF(const pointf& p, bool TransFlag)const DXLE_NOEXCEPT_OR_NOTHROW_SINGLE { DXLE_GET_LOCK(screen_mutex_c::mtx); return DxLib::DrawGraphF(p.x, p.y, GetHandle(), TransFlag); }
			//! 画像の拡大描画( 座標指定が float 版 )
			inline int DrawExtendGraphF(float x1f, float y1f, float x2f, float y2, bool TransFlag)const DXLE_NOEXCEPT_OR_NOTHROW_SINGLE{ DXLE_GET_LOCK(screen_mutex_c::mtx); return DxLib::DrawExtendGraphF(x1f, y1f, x2f, y2, GetHandle(), TransFlag); }
			//! 画像の拡大描画( 座標指定が float 版 )
			inline int DrawExtendGraphF(const pointf& lu, const pointf& rb, bool TransFlag)const DXLE_NOEXCEPT_OR_NOTHROW_SINGLE { DXLE_GET_LOCK(screen_mutex_c::mtx); return DxLib::DrawExtendGraphF(lu.x, lu.y, rb.x, rb.y, GetHandle(), TransFlag); }
			//! 画像の回転描画( 座標指定が float 版 )
			inline int DrawRotaGraphF(float xf, float yf, double ExRate, double Angle, bool TransFlag, bool TurnFlag = FALSE)const DXLE_NOEXCEPT_OR_NOTHROW_SINGLE{ DXLE_GET_LOCK(screen_mutex_c::mtx); return DxLib::DrawRotaGraphF(xf, yf, ExRate, Angle, GetHandle(), TransFlag, TurnFlag); }
			//! 画像の回転描画( 座標指定が float 版 )
			inline int DrawRotaGraphF(const pointf& p, double ExRate, double Angle, bool TransFlag, bool TurnFlag = FALSE)const DXLE_NOEXCEPT_OR_NOTHROW_SINGLE { DXLE_GET_LOCK(screen_mutex_c::mtx); return DxLib::DrawRotaGraphF(p.x, p.y, ExRate, Angle, GetHandle(), TransFlag, TurnFlag); }
			//! 画像の回転描画２( 回転中心指定付き )( 座標指定が float 版 )
			inline int DrawRotaGraph2F(float xf, float yf, float cxf, float cyf, double ExtRate, double Angle, bool TransFlag, bool TurnFlag = FALSE)const DXLE_NOEXCEPT_OR_NOTHROW_SINGLE { DXLE_GET_LOCK(screen_mutex_c::mtx); return DxLib::DrawRotaGraph2F(xf, yf, cxf, cyf, ExtRate, Angle, GetHandle(), TransFlag, TurnFlag); }
			//! 画像の回転描画２( 回転中心指定付き )( 座標指定が float 版 )
			inline int DrawRotaGraph2F(const pointf& p, const pointf& c, double ExtRate, double Angle, bool TransFlag, bool TurnFlag = FALSE)const DXLE_NOEXCEPT_OR_NOTHROW_SINGLE { DXLE_GET_LOCK(screen_mutex_c::mtx); return DxLib::DrawRotaGraph2F(p.x, p.y, c.x, c.y, ExtRate, Angle, GetHandle(), TransFlag, TurnFlag); }
			//! 画像の回転描画３( 回転中心指定付き＋縦横拡大率別指定版 )( 座標指定が float 版 )
			inline int DrawRotaGraph3F(float xf, float yf, float cxf, float cyf, double ExtRateX, double ExtRateY, double Angle, bool TransFlag, bool TurnFlag = FALSE)const DXLE_NOEXCEPT_OR_NOTHROW_SINGLE{ DXLE_GET_LOCK(screen_mutex_c::mtx); return DxLib::DrawRotaGraph3F(xf, yf, cxf, cyf, ExtRateX, ExtRateY, Angle, GetHandle(), TransFlag, TurnFlag); }
			//! 画像の回転描画３( 回転中心指定付き＋縦横拡大率別指定版 )( 座標指定が float 版 )
			inline int DrawRotaGraph3F(const pointf& p, const pointf& c, double ExtRateX, double ExtRateY, double Angle, bool TransFlag, bool TurnFlag = FALSE)const DXLE_NOEXCEPT_OR_NOTHROW_SINGLE { DXLE_GET_LOCK(screen_mutex_c::mtx); return DxLib::DrawRotaGraph3F(p.x, p.y, c.x, c.y, ExtRateX, ExtRateY, Angle, GetHandle(), TransFlag, TurnFlag); }
			//! 画像の自由変形描画( 座標指定が float 版 )
			inline int DrawModiGraphF(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4, bool TransFlag)const DXLE_NOEXCEPT_OR_NOTHROW_SINGLE{ DXLE_GET_LOCK(screen_mutex_c::mtx); return DxLib::DrawModiGraphF(x1, y1, x2, y2, x3, y3, x4, y4, GetHandle(), TransFlag); }
			//! 画像の自由変形描画( 座標指定が float 版 )
			inline int DrawModiGraphF(const std::array<pointf, 4>& ps, bool TransFlag)const DXLE_NOEXCEPT_OR_NOTHROW_SINGLE { DXLE_GET_LOCK(screen_mutex_c::mtx); return DxLib::DrawModiGraphF(ps[0].x, ps[0].y, ps[1].x, ps[1].y, ps[2].x, ps[2].y, ps[3].x, ps[3].y, GetHandle(), TransFlag); }
			//! 画像の左右反転描画( 座標指定が float 版 )
			inline int DrawTurnGraphF(float xf, float yf, bool TransFlag)const DXLE_NOEXCEPT_OR_NOTHROW_SINGLE{ DXLE_GET_LOCK(screen_mutex_c::mtx); return DxLib::DrawTurnGraphF(xf, yf, GetHandle(), TransFlag); }
			//! 画像の左右反転描画( 座標指定が float 版 )
			inline int DrawTurnGraphF(const pointf& p, bool TransFlag)const DXLE_NOEXCEPT_OR_NOTHROW_SINGLE { DXLE_GET_LOCK(screen_mutex_c::mtx); return DxLib::DrawTurnGraphF(p.x, p.y, GetHandle(), TransFlag); }
			//! 画像の拡大左右反転描画( 座標指定が float 版 )
			inline int DrawExtendTurnGraphF(float x1f, float y1f, float x2f, float y2f, bool TransFlag)const DXLE_NOEXCEPT_OR_NOTHROW_SINGLE{ DXLE_GET_LOCK(screen_mutex_c::mtx); return DxLib::DrawExtendGraphF(x2f, y1f, x1f, y2f, GetHandle(), TransFlag); }//x1fとx2fはこれで正しい
			//! 画像の拡大左右反転描画( 座標指定が float 版 )
			inline int DrawExtendTurnGraphF(const pointf& lu, const pointf& rb, bool TransFlag)const DXLE_NOEXCEPT_OR_NOTHROW_SINGLE{ DXLE_GET_LOCK(screen_mutex_c::mtx); return DxLib::DrawExtendGraphF(rb.x, lu.y, lu.x, rb.y, GetHandle(), TransFlag); }//x1fとx2fはこれで正しい

			int filter_mono(int16_t Cb, int16_t Cr)DXLE_NOEXCEPT_OR_NOTHROW{ return GraphFilter(GetHandle(), DX_GRAPH_FILTER_MONO, Cb, Cr);}
			int filter_gaussian(uint16_t PixelWidth, int Param)DXLE_NOEXCEPT_OR_NOTHROW { return GraphFilter(GetHandle(), DX_GRAPH_FILTER_GAUSS, PixelWidth, Param); }
			int filter_down_scale(uint8_t DivNum)DXLE_NOEXCEPT_OR_NOTHROW { return GraphFilter(GetHandle(), DX_GRAPH_FILTER_DOWN_SCALE, DivNum); }
			int filter_bright_clip(bright_clip_mode clipmode, uint8_t clip_pram, unsigned int fillcolor, uint8_t fillalpha)DXLE_NOEXCEPT_OR_NOTHROW {
				return GraphFilter(GetHandle(), DX_GRAPH_FILTER_BRIGHT_CLIP, clipmode, clip_pram, fillcolor, fillalpha);
			}
			int filter_HSB(bool HueType, int16_t Hue, int Saturation, int16_t Bright)DXLE_NOEXCEPT_OR_NOTHROW {
				return GraphFilter(GetHandle(), DX_GRAPH_FILTER_HSB, HueType, Hue, Saturation, Bright);
			}
			int filter_invert()DXLE_NOEXCEPT_OR_NOTHROW { return GraphFilter(GetHandle(), DX_GRAPH_FILTER_INVERT); }
			int filter_level(uint8_t min, uint8_t max, int Gamma, uint8_t Aftermin, uint8_t Aftermax)DXLE_NOEXCEPT_OR_NOTHROW {
				return GraphFilter(GetHandle(), DX_GRAPH_FILTER_LEVEL, min, max, Gamma, Aftermin, Aftermax);
			}
			int filter_two_color(uint8_t threshold, unsigned int LowColor, uint8_t LowAlpha, unsigned int HighColor, uint8_t HighAlpha)DXLE_NOEXCEPT_OR_NOTHROW {
				return GraphFilter(GetHandle(), DX_GRAPH_FILTER_LEVEL, threshold, LowColor, LowAlpha, HighColor, HighAlpha);
			}

		private:
			texture_2d(int param_handle, bool param_NotUse3DFlag)DXLE_NOEXCEPT_OR_NOTHROW : Unique_HandledObject_Bace(param_handle), NotUse3DFlag(param_NotUse3DFlag){}
			bool NotUse3DFlag;

			//screen等の子クラスからGetHandleにアクセスできるようにする為
			static int GetTexture2DHandle(const texture_2d& texture2d_obj) { return texture2d_obj.GetHandle(); }

			friend class screen;
		};
		//! 描画可能画像クラス
		class screen : public texture_2d
		{
		public:
			//!画像を複製する
			screen cloneSc()const;
			//!画像を複製する
			texture_2d cloneGr()const override;

			//生成用static関数

			//! SetDrawScreen で描画対象にできるグラフィックを作成する
			static inline screen MakeScreen(int SizeX, int SizeY, bool UseAlphaChannel = false)DXLE_NOEXCEPT_OR_NOTHROW { return screen(DxLib::MakeScreen(SizeX, SizeY, UseAlphaChannel), UseAlphaChannel); }
			//! SetDrawScreen で描画対象にできるグラフィックを作成する
			static inline screen MakeScreen(const sizei& size, bool UseAlphaChannel = false)DXLE_NOEXCEPT_OR_NOTHROW { return screen(DxLib::MakeScreen(size.width, size.height, UseAlphaChannel), UseAlphaChannel); }

			//メンバ関数

			template<typename Func_T>
			inline void drawn_on(Func_T&& draw_func) {
				DXLE_GET_LOCK(screen_mutex_c::mtx);
				struct Finary_ {
					int old_draw_screen;
					int old_is_setting_reset;
					Finary_()
						: old_draw_screen(DxLib::GetDrawScreen())
						, old_is_setting_reset(DxLib::GetUseSetDrawScreenSettingReset())
					{}
					~Finary_() {
						DxLib::SetDrawScreen(old_draw_screen);
						DxLib::SetUseSetDrawScreenSettingReset(old_is_setting_reset);
					}
				}finally_;
				DxLib::SetUseSetDrawScreenSettingReset(FALSE);
				this->SetDrawScreen();
				draw_func();
			}

			//! グラフィック専用のＺバッファを持つかどうかを設定する
			//!@param UseFlag 専用のＺバッファを持つかどうか( true:持つ( デフォルト )  false:持たない )
			//!@param BitDepth ビット深度( 16 or 24 or 32 ) )
			inline int SetUseGraphZBuffer(bool UseFlag, int BitDepth = -1)DXLE_NOEXCEPT_OR_NOTHROW { return DxLib::SetUseGraphZBuffer(GetHandle(), UseFlag, BitDepth); }
			//! グラフィックのＺバッファの状態を別のグラフィックのＺバッファにコピーする
			//! *thisはＺバッファを持っている描画対象にできるグラフィック
			//!@param ToGraph *thisとサイズが同じであり、且つマルチサンプリング( アンチエイリアス )設定が無い、Ｚバッファを持っている描画対象にできるグラフィック
			inline int CopyGraphZBufferImage(screen& ToGraph)const DXLE_NOEXCEPT_OR_NOTHROW { return DxLib::CopyGraphZBufferImage(GetHandle(), ToGraph.GetHandle()); }
			//! SetDrawScreen で描画対象にできるグラフィックハンドルから指定領域の画像情報を別のグラフィックハンドルに転送する
			//!@param x1 転送元の矩形の左座標
			//!@param y1 転送元の矩形の上座標
			//!@param x2 転送元の矩形の右座標
			//!@param y2 転送元の矩形の下座標
			//!@param DestX, DestY 転送先の矩形の左上座標
			//!@param DestGrHandle 転送先の画像
			inline int BltDrawValidGraph(int x1, int y1, int x2, int y2, int DestX, int DestY, texture_2d& DestGrHandle)const DXLE_NOEXCEPT_OR_NOTHROW{ return DxLib::BltDrawValidGraph(GetHandle(), x1, y1, x2, y2, DestX, DestY, texture_2d::GetTexture2DHandle(DestGrHandle)); }
//
			//! SetDrawScreen で描画対象にできるグラフィックハンドルから指定領域の画像情報を別のグラフィックハンドルに転送する
			//!@param lu 転送元の矩形の左上座標
			//!@param rb 転送元の矩形の右下座標
			//!@param dest 転送先の矩形の左上座標
			//!@param DestGrHandle 転送先の画像
			inline int BltDrawValidGraph(const pointi& lu, const pointi& rb, const pointi& dest, texture_2d& DestGrHandle)const DXLE_NOEXCEPT_OR_NOTHROW { return DxLib::BltDrawValidGraph(GetHandle(), rb.x, lu.y, lu.x, rb.y, dest.x, dest.y, texture_2d::GetTexture2DHandle(DestGrHandle)); }
			//! 描画先画面を設定する
			inline int SetDrawScreen()const DXLE_NOEXCEPT_OR_NOTHROW_SINGLE { DXLE_GET_LOCK(screen_mutex_c::mtx); return DxLib::SetDrawScreen(GetHandle()); }

		public:
			screen() : texture_2d() {}
			//! コピー禁止
			screen(const screen& other) = delete;
			//!所有権の譲渡
			screen(screen&& other) DXLE_NOEXCEPT_OR_NOTHROW : texture_2d(std::move(other)), UseAlphaChannel(std::move(other.UseAlphaChannel)) {}
			//! コピー禁止
			screen& operator=(const screen& other) = delete;
			//! 所有権の譲渡
			screen& operator=(screen&& other) DXLE_NOEXCEPT_OR_NOTHROW { texture_2d::operator=(std::move(other)); UseAlphaChannel = (std::move(other.UseAlphaChannel)); return *this; }

			virtual ~screen()DXLE_NOEXCEPT_OR_NOTHROW {}
		protected:
			screen(int param_handle, bool param_UseAlphaChannel)DXLE_NOEXCEPT_OR_NOTHROW : texture_2d(param_handle, false), UseAlphaChannel(param_UseAlphaChannel) {}
			bool UseAlphaChannel;
		};
		//! 複製
		inline texture_2d texture_2d::cloneGr()const
		{
			screen temp_sc{ screen::MakeScreen(GetGraphSize()) };
			temp_sc.drawn_on([this](){
				DrawGraph({ 0, 0 }, false);
			});
			return temp_sc.cloneGr();
		}
		inline screen screen::cloneSc()const
		{
			int graph_size_x, graph_size_y;
			GetGraphSize(&graph_size_x, &graph_size_y);

			screen new_screen = screen::MakeScreen(graph_size_x, graph_size_y, UseAlphaChannel);
			this->BltDrawValidGraph(0, 0, graph_size_x, graph_size_y, 0, 0, new_screen);
			return new_screen;
		}
		inline texture_2d screen::cloneGr()const
		{
			int graph_size_x, graph_size_y;
			GetGraphSize(&graph_size_x, &graph_size_y);

			texture_2d new_graph = texture_2d::MakeGraph(graph_size_x, graph_size_y, NotUse3DFlag);
			this->BltDrawValidGraph(0, 0, graph_size_x, graph_size_y, 0, 0, new_graph);
			return new_graph;
		}
	
		//生成用関数
	
		// グラフィック作成関係関数
	
		//! 指定サイズのグラフィックを作成する
		inline texture_2d MakeGraph(int SizeX, int SizeY, bool NotUse3DFlag = false)DXLE_NOEXCEPT_OR_NOTHROW { return texture_2d::MakeGraph(SizeX, SizeY, NotUse3DFlag); }
		//! 指定サイズのグラフィックを作成する
		inline texture_2d MakeGraph(const sizei& size, bool NotUse3DFlag = false)DXLE_NOEXCEPT_OR_NOTHROW { return texture_2d::MakeGraph(size.width, size.height, NotUse3DFlag); }
		//! SetDrawScreen で描画対象にできるグラフィックを作成する
		inline screen MakeScreen(int SizeX, int SizeY, bool UseAlphaChannel = false)DXLE_NOEXCEPT_OR_NOTHROW { return screen::MakeScreen(SizeX, SizeY, UseAlphaChannel); }
		//! SetDrawScreen で描画対象にできるグラフィックを作成する
		inline screen MakeScreen(const sizei& size, bool UseAlphaChannel = false)DXLE_NOEXCEPT_OR_NOTHROW { return screen::MakeScreen(size.width, size.height, UseAlphaChannel); }
		//! 指定のグラフィックの指定部分だけを抜き出して新たなグラフィックを作成する
		inline texture_2d DerivationGraph(int SrcX, int SrcY, int Width, int Height, int SrcGraphHandle)DXLE_NOEXCEPT_OR_NOTHROW { return texture_2d::DerivationGraph(SrcX, SrcY, Width, Height, SrcGraphHandle); }
		//! 指定のグラフィックの指定部分だけを抜き出して新たなグラフィックを作成する
		inline texture_2d DerivationGraph(const pointi& src, const sizei& size, int SrcGraphHandle)DXLE_NOEXCEPT_OR_NOTHROW { return texture_2d::DerivationGraph(src.x, src.y, size.width, size.height, SrcGraphHandle); }

		// 画像からグラフィックを作成する関数
	
		//! 画像ファイルからグラフィックを作成する
		inline texture_2d LoadBmpToGraph(const TCHAR *FileName, int TextureFlag, int ReverseFlag, int SurfaceMode = DX_MOVIESURFACE_NORMAL)DXLE_NOEXCEPT_OR_NOTHROW { return texture_2d::LoadBmpToGraph(FileName, TextureFlag, ReverseFlag, SurfaceMode); }
		//! 画像ファイルからグラフィックを作成する
		inline texture_2d LoadBmpToGraph(const tstring& FileName, int TextureFlag, int ReverseFlag, int SurfaceMode = DX_MOVIESURFACE_NORMAL)DXLE_NOEXCEPT_OR_NOTHROW { return texture_2d::LoadBmpToGraph(FileName.c_str(), TextureFlag, ReverseFlag, SurfaceMode); }
		//! 画像ファイルからグラフィックを作成する
		inline texture_2d LoadGraph(const TCHAR *FileName, bool NotUse3DFlag = false)DXLE_NOEXCEPT_OR_NOTHROW { return texture_2d::LoadGraph(FileName, NotUse3DFlag); }
		//! 画像ファイルからグラフィックを作成する
		inline texture_2d LoadGraph(const tstring& FileName, bool NotUse3DFlag = false)DXLE_NOEXCEPT_OR_NOTHROW { return texture_2d::LoadGraph(FileName.c_str(), NotUse3DFlag); }
		//! 画像ファイルを反転したものでグラフィックを作成する
		inline texture_2d LoadReverseGraph(const TCHAR *FileName, bool NotUse3DFlag = false)DXLE_NOEXCEPT_OR_NOTHROW { return texture_2d::LoadReverseGraph(FileName, NotUse3DFlag); }
		//! 画像ファイルを反転したものでグラフィックを作成する
		inline texture_2d LoadReverseGraph(const tstring& FileName, bool NotUse3DFlag = false)DXLE_NOEXCEPT_OR_NOTHROW { return texture_2d::LoadReverseGraph(FileName.c_str(), NotUse3DFlag); }

		//! メモリ上の画像イメージからグラフィックを作成する
		inline texture_2d CreateGraphFromMem(const void *RGBFileImage, int RGBFileImageSize, const void *AlphaFileImage = nullptr, int AlphaFileImageSize = 0, bool TextureFlag = true, bool ReverseFlag = false)DXLE_NOEXCEPT_OR_NOTHROW { return texture_2d::CreateGraphFromMem(RGBFileImage, RGBFileImageSize, AlphaFileImage, AlphaFileImageSize, TextureFlag, ReverseFlag); }
		//! メモリ上の画像イメージから既存のグラフィックにデータを転送する
		inline int ReCreateGraphFromMem(const void *RGBFileImage, int RGBFileImageSize, texture_2d& GrHandle, const void *AlphaFileImage = nullptr, int AlphaFileImageSize = 0, bool TextureFlag = true, bool ReverseFlag = false)DXLE_NOEXCEPT_OR_NOTHROW { return texture_2d::ReCreateGraphFromMem(RGBFileImage, RGBFileImageSize, GrHandle, AlphaFileImage, AlphaFileImageSize, TextureFlag, ReverseFlag); }
	
		//! 基本イメージデータからサイズを割り出し、それに合ったグラフィックを作成する
		inline texture_2d CreateDXGraph(const BASEIMAGE *RgbBaseImage, const BASEIMAGE *AlphaBaseImage, bool TextureFlag)DXLE_NOEXCEPT_OR_NOTHROW { return texture_2d::CreateDXGraph(RgbBaseImage, AlphaBaseImage, TextureFlag); }
		//! 基本イメージデータからグラフィックを作成する
		inline texture_2d CreateGraphFromGraphImage(const BASEIMAGE *RgbBaseImage, bool TextureFlag = true, bool ReverseFlag = false)DXLE_NOEXCEPT_OR_NOTHROW { return texture_2d::CreateGraphFromGraphImage(RgbBaseImage, TextureFlag, ReverseFlag); }
		//! 基本イメージデータからグラフィックを作成する
		inline texture_2d CreateGraphFromGraphImage(const BASEIMAGE *RgbBaseImage, const BASEIMAGE *AlphaBaseImage, bool TextureFlag = true, bool ReverseFlag = false)DXLE_NOEXCEPT_OR_NOTHROW { return texture_2d::CreateGraphFromGraphImage(RgbBaseImage, AlphaBaseImage, TextureFlag, ReverseFlag); }
		//! 基本イメージデータから既存のグラフィックにデータを転送する
		inline int ReCreateGraphFromGraphImage(const BASEIMAGE *RgbBaseImage, texture_2d& GrHandle, bool TextureFlag = true, bool ReverseFlag = false)DXLE_NOEXCEPT_OR_NOTHROW { return texture_2d::ReCreateGraphFromGraphImage(RgbBaseImage, GrHandle, TextureFlag, ReverseFlag); }
		//! 基本イメージデータから既存のグラフィックにデータを転送する
		inline int ReCreateGraphFromGraphImage(const BASEIMAGE *RgbBaseImage, const BASEIMAGE *AlphaBaseImage, texture_2d& GrHandle, bool TextureFlag = true, bool ReverseFlag = false)DXLE_NOEXCEPT_OR_NOTHROW { return texture_2d::ReCreateGraphFromGraphImage(RgbBaseImage, AlphaBaseImage, GrHandle, TextureFlag, ReverseFlag); }
	
		//! メモリ上のビットマップイメージからグラフィックを作成する
		inline texture_2d CreateGraph(int Width, int Height, int Pitch, const void *RGBImage, const void *AlphaImage = nullptr, const texture_2d& GrHandle = texture_2d())DXLE_NOEXCEPT_OR_NOTHROW { return texture_2d::CreateGraph(Width, Height, Pitch, RGBImage, AlphaImage, GrHandle); }
		//! メモリ上のビットマップイメージからグラフィックを作成する
		inline texture_2d CreateGraph(const sizei& size, int Pitch, const void *RGBImage, const void *AlphaImage = nullptr, const texture_2d& GrHandle = texture_2d())DXLE_NOEXCEPT_OR_NOTHROW { return texture_2d::CreateGraph(size.width, size.height, Pitch, RGBImage, AlphaImage, GrHandle); }
		//! メモリ上のビットマップイメージからグラフィックを再作成する
		inline int ReCreateGraph(int Width, int Height, int Pitch, const void *RGBImage, texture_2d& GrHandle, const void *AlphaImage = nullptr)DXLE_NOEXCEPT_OR_NOTHROW { return texture_2d::ReCreateGraph(Width, Height, Pitch, RGBImage, GrHandle, AlphaImage); }
		//! メモリ上のビットマップイメージからグラフィックを再作成する
		inline int ReCreateGraph(const sizei& size, int Pitch, const void *RGBImage, texture_2d& GrHandle, const void *AlphaImage = nullptr)DXLE_NOEXCEPT_OR_NOTHROW { return texture_2d::ReCreateGraph(size.width, size.height, Pitch, RGBImage, GrHandle, AlphaImage); }
	#ifndef DX_NON_SOFTIMAGE
	#endif // DX_NON_SOFTIMAGE
		//! 基本イメージデータからグラフィックを作成する
		inline texture_2d CreateGraphFromBaseImage(const BASEIMAGE *BaseImage)DXLE_NOEXCEPT_OR_NOTHROW { return texture_2d::CreateGraphFromBaseImage(BaseImage); }
		//! 基本イメージデータの指定の領域を使ってグラフィックを作成する
		inline texture_2d CreateGraphFromRectBaseImage(const BASEIMAGE *BaseImage, int x, int y, int SizeX, int SizeY)DXLE_NOEXCEPT_OR_NOTHROW { return texture_2d::CreateGraphFromRectBaseImage(BaseImage, x, y, SizeX, SizeY); }
		//! 基本イメージデータの指定の領域を使ってグラフィックを作成する
		inline texture_2d CreateGraphFromRectBaseImage(const BASEIMAGE *BaseImage, const pointi& p, const sizei& size)DXLE_NOEXCEPT_OR_NOTHROW { return texture_2d::CreateGraphFromRectBaseImage(BaseImage, p.x, p.y, size.width, size.height); }
		//! 基本イメージデータから既存のグラフィックに画像データを転送する
		inline int ReCreateGraphFromBaseImage(const BASEIMAGE *BaseImage, texture_2d& GrHandle)DXLE_NOEXCEPT_OR_NOTHROW { return texture_2d::ReCreateGraphFromBaseImage(BaseImage, GrHandle); }
		//! 基本イメージデータの指定の領域を使って既存のグラフィックに画像データを転送する
		inline int ReCreateGraphFromRectBaseImage(const BASEIMAGE *BaseImage, int x, int y, int SizeX, int SizeY, texture_2d& GrHandle)DXLE_NOEXCEPT_OR_NOTHROW { return texture_2d::ReCreateGraphFromRectBaseImage(BaseImage, x, y, SizeX, SizeY, GrHandle); }
		//! 基本イメージデータの指定の領域を使って既存のグラフィックに画像データを転送する
		inline int ReCreateGraphFromRectBaseImage(const BASEIMAGE *BaseImage, const pointi& p, const sizei& size, texture_2d& GrHandle)DXLE_NOEXCEPT_OR_NOTHROW { return texture_2d::ReCreateGraphFromRectBaseImage(BaseImage, p.x, p.y, size.width, size.height, GrHandle); }
		//! 画像ファイルからグラフィックへ画像データを転送する
		inline int ReloadGraph(const TCHAR *FileName, texture_2d& GrHandle, bool ReverseFlag = false)DXLE_NOEXCEPT_OR_NOTHROW { return texture_2d::ReloadGraph(FileName, GrHandle, ReverseFlag); }
		//! 画像ファイルからグラフィックへ画像データを転送する
		inline int ReloadGraph(const tstring& FileName, texture_2d& GrHandle, bool ReverseFlag = false)DXLE_NOEXCEPT_OR_NOTHROW { return texture_2d::ReloadGraph(FileName.c_str(), GrHandle, ReverseFlag); }
		//! ReloadGraph の画像反転処理追加版
		inline int ReloadReverseGraph(const TCHAR *FileName, texture_2d& GrHandle)DXLE_NOEXCEPT_OR_NOTHROW { return texture_2d::ReloadReverseGraph(FileName, GrHandle); }
<<<<<<< HEAD
=======
		//! ReloadGraph の画像反転処理追加版
		inline int ReloadReverseGraph(const tstring& FileName, texture_2d& GrHandle)DXLE_NOEXCEPT_OR_NOTHROW { return texture_2d::ReloadReverseGraph(FileName.c_str(), GrHandle); }

	}
	namespace DxLibEx_Classes {
		using Graph2D::texture_2d;
>>>>>>> origin/master
	}
	using namespace graph2d;
}

#endif
