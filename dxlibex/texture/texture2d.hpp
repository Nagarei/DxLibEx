/*=============================================================================
  Copyright (C) 2015-2016 DxLibEx project
  https://github.com/Nagarei/DxLibEx/

  Distributed under the Boost Software License, Version 1.0.
  (See http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#ifndef DXLE_INC_TEXTURE_TEXTURE2D_HPP_
#define DXLE_INC_TEXTURE_TEXTURE2D_HPP_

#include "dxlibex/config/no_min_max.h"
#if !defined(CINTERFACE) && defined(__c2__) &&  __clang_major__ == 3 && __clang_minor__ == 8
//To avoid compile error
//C:\Program Files (x86)\Windows Kits\8.1\Include\um\combaseapi.h(229,21): error : unknown type name 'IUnknown'
//          static_cast<IUnknown*>(*pp);    // make sure everyone derives from IUnknown
#define CINTERFACE
#endif
#include "DxLib.h"
#include <mutex>
#include <array>
#include <utility>
#include <algorithm>
#include <vector>
#include <cstdint>
#include "dxlibex/config/defines.h"
#include "texture2d/prototype2d.hpp"
#include "texture2d/texture2d_handle_manager.hpp"
#include "dxlibex/basic_types.hpp"
#include "dxlibex/thread.hpp"


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
			static std::recursive_mutex mtx;
#endif
		};

		/**
		@relates texture2d
		\~japanese	@brief	texture2dとscreen_cのfilter_bright_clipメンバー関数向けのenum
		\~english	@brief	for filter_bright_clip(texture2d and screen_c )
		*/
		enum class bright_clip_mode : int {
			LESS,
			GREATER
		};

	//--------------------クラス--------------------//
		
		class texture2d /*final*/
		{
		public:
			DXLE_CONSTEXPR texture2d()DXLE_NOEXCEPT_OR_NOTHROW{}
			//! コピー禁止
			texture2d(const texture2d& other) = delete;
			texture2d& operator=(const texture2d& other) = delete;
			//!所有権の譲渡
			texture2d(texture2d&& other) DXLE_NOEXCEPT_OR_NOTHROW : handle_manager(std::move(other.handle_manager)){}
			texture2d& operator=(texture2d&& other) DXLE_NOEXCEPT_OR_NOTHROW { handle_manager = (std::move(other.handle_manager)); return *this; }

			//!\~japanese 画像を削除する
			//!\~english  Delete this image
			inline void delete_this(bool LogOutFlag = false) { DxLib::DeleteGraph(GetHandle(), LogOutFlag); }
			//!\~japanese 有効な画像を持っているか調べる（trueで持っている）
			//!\~english  Returns whether this object has valid texture (if it has, this functoin returns true)
			inline bool valid()const { return (GetHandle() != -1); }

			//! グラフィックのサイズを得る
			inline sizei size()const DXLE_NOEXCEPT_OR_NOTHROW{ return GetGraphSize(); }

			//!画像を複製する
			virtual std::unique_ptr<texture2d> clone()const;

			virtual ~texture2d()DXLE_NOEXCEPT_OR_NOTHROW{}

		public:
			//生成用コンストラクタ

			//!\~japanese 指定サイズのグラフィックを作成する
			//!\~english  Create an image with a specific size
			//inline texture2d(int SizeX, int SizeY, bool NotUse3DFlag = false): texture2d(DxLib::MakeGraph(SizeX, SizeY, NotUse3DFlag), NotUse3DFlag){}

			//!\~japanese 画像ファイルからグラフィックを作成する
			//!\~english  Create an image form an image file
			inline texture2d(const TCHAR *FileName, int TextureFlag, int ReverseFlag, int SurfaceMode = DX_MOVIESURFACE_NORMAL) : texture2d(DxLib::LoadBmpToGraph(FileName, TextureFlag, ReverseFlag, SurfaceMode), false){}
			//!\~japanese 画像ファイルからグラフィックを作成する
			//!\~english  Create an image form an image file
			inline texture2d(const TCHAR *FileName, bool NotUse3DFlag = false) : texture2d(DxLib::LoadGraph(FileName, NotUse3DFlag), NotUse3DFlag){}


			//生成用static関数

			// グラフィック作成関係関数

			//!\~japanese 指定サイズのグラフィックを作成する
			//!\~english  Create an image with a specific size
			static inline texture2d MakeGraph(int SizeX, int SizeY, bool NotUse3DFlag = false){ return texture2d(DxLib::MakeGraph(SizeX, SizeY, NotUse3DFlag), NotUse3DFlag); }
			//!\~japanese 指定サイズのグラフィックを作成する
			//!\~english  Create an image with sizes
			static inline texture2d MakeGraph(const sizei& size, bool NotUse3DFlag = false){ return texture2d(DxLib::MakeGraph(size.width, size.height, NotUse3DFlag), NotUse3DFlag); }
			//! 指定のグラフィックの指定部分だけを抜き出して新たなグラフィックを作成する
			static inline texture2d DerivationGraph(int SrcX, int SrcY, int Width, int Height, const texture2d& SrcGraphHandle){ return texture2d(DxLib::DerivationGraph(SrcX, SrcY, Width, Height, SrcGraphHandle.GetHandle()), false); }
			//! 指定のグラフィックの指定部分だけを抜き出して新たなグラフィックを作成する
			static inline texture2d DerivationGraph(const pointi& src, const sizei& size, const texture2d& SrcGraphHandle){ return texture2d(DxLib::DerivationGraph(src.x, src.y, size.width, size.height, SrcGraphHandle.GetHandle()), false); }

			// 画像からグラフィックを作成する関数

			//!\~japanese 画像ファイルからグラフィックを作成する
			//!\~english  Create an image from an image file
			static inline texture2d LoadBmpToGraph(const TCHAR *FileName, int TextureFlag, int ReverseFlag, int SurfaceMode = DX_MOVIESURFACE_NORMAL){ return texture2d(DxLib::LoadBmpToGraph(FileName, TextureFlag, ReverseFlag, SurfaceMode), false); }
			//!\~japanese 画像ファイルからグラフィックを作成する
			//!\~english  Create an image from an image file
			static inline texture2d LoadBmpToGraph(const dxle::tstring& FileName, int TextureFlag, int ReverseFlag, int SurfaceMode = DX_MOVIESURFACE_NORMAL){ return texture2d(DxLib::LoadBmpToGraph(FileName.c_str(), TextureFlag, ReverseFlag, SurfaceMode), false); }
			//!\~japanese 画像ファイルからグラフィックを作成する
			//!\~english  Create an image from an image file
			static inline texture2d LoadGraph(const TCHAR *FileName, bool NotUse3DFlag = false){ return texture2d(DxLib::LoadGraph(FileName, NotUse3DFlag), NotUse3DFlag); }
			//!\~japanese 画像ファイルからグラフィックを作成する
			//!\~english  Create an image from an image file
			static inline texture2d LoadGraph(const dxle::tstring& FileName, bool NotUse3DFlag = false){ return texture2d(DxLib::LoadGraph(FileName.c_str(), NotUse3DFlag), NotUse3DFlag); }
			//!\~japanese 画像ファイルからグラフィックを作成する
			//!\~english  Create a reversed image from an image file
			static inline texture2d LoadReverseGraph(const TCHAR *FileName, bool NotUse3DFlag = false){ return texture2d(DxLib::LoadReverseGraph(FileName, NotUse3DFlag), NotUse3DFlag); }
			//!\~japanese 画像ファイルからグラフィックを作成する
			//!\~english  Create a reversed image from an image file
			static inline texture2d LoadReverseGraph(const dxle::tstring& FileName, bool NotUse3DFlag = false){ return texture2d(DxLib::LoadReverseGraph(FileName.c_str(), NotUse3DFlag), NotUse3DFlag); }


			//!\~japanese メモリ上の画像イメージからグラフィックを作成する
			//!\~english  Create an graphic from an image stored in the computer memory
			static inline texture2d CreateGraphFromMem(const void *RGBFileImage, int RGBFileImageSize, const void *AlphaFileImage = nullptr, int AlphaFileImageSize = 0, bool TextureFlag = true, bool ReverseFlag = false){ return texture2d(DxLib::CreateGraphFromMem(RGBFileImage, RGBFileImageSize, AlphaFileImage, AlphaFileImageSize, TextureFlag, ReverseFlag), false); }
			//!\~japanese メモリ上の画像イメージから既存のグラフィックにデータを転送する
			//!\~english  Recreate an graphic from an existing image stored in the computer memory
			static inline int ReCreateGraphFromMem(const void *RGBFileImage, int RGBFileImageSize, texture2d& GrHandle, const void *AlphaFileImage = nullptr, int AlphaFileImageSize = 0, bool TextureFlag = true, bool ReverseFlag = false){ return DxLib::ReCreateGraphFromMem(RGBFileImage, RGBFileImageSize, GrHandle.GetHandle(), AlphaFileImage, AlphaFileImageSize, TextureFlag, ReverseFlag); }

			//!\~japanese 基本イメージデータからサイズを割り出し、それに合ったグラフィックを作成する
			//!\~english  Determine the size of the basic image data and create a compatible graphics
			static inline texture2d CreateDXGraph(const DxLib::BASEIMAGE *RgbBaseImage, const DxLib::BASEIMAGE *AlphaBaseImage, bool TextureFlag){ return texture2d(DxLib::CreateDXGraph(RgbBaseImage, AlphaBaseImage, TextureFlag), false); }
			//!\~japanese 基本イメージデータからグラフィックを作成する
			//!\~english  Create a graphic from a basic image data
			static inline texture2d CreateGraphFromGraphImage(const DxLib::BASEIMAGE *RgbBaseImage, bool TextureFlag = true, bool ReverseFlag = false){ return texture2d(DxLib::CreateGraphFromGraphImage(RgbBaseImage, TextureFlag, ReverseFlag), false); }
			//!\~japanese 基本イメージデータからグラフィックを作成する
			//!\~english  Create a graphic from a basic image data
			static inline texture2d CreateGraphFromGraphImage(const DxLib::BASEIMAGE *RgbBaseImage, const DxLib::BASEIMAGE *AlphaBaseImage, bool TextureFlag = true, bool ReverseFlag = false){ return texture2d(DxLib::CreateGraphFromGraphImage(RgbBaseImage, AlphaBaseImage, TextureFlag, ReverseFlag), false); }
			//!\~japanese 基本イメージデータから既存のグラフィックにデータを転送する
			//!\~english  Recreate a graphic from an existing basic image data
			static inline int ReCreateGraphFromGraphImage(const DxLib::BASEIMAGE *RgbBaseImage, texture2d& GrHandle, bool TextureFlag = true, bool ReverseFlag = false){ return DxLib::ReCreateGraphFromGraphImage(RgbBaseImage, GrHandle.GetHandle(), TextureFlag, ReverseFlag); }
			//!\~japanese 基本イメージデータから既存のグラフィックにデータを転送する
			//!\~english  Recreate a graphic from an existing basic image data
			static inline int ReCreateGraphFromGraphImage(const DxLib::BASEIMAGE *RgbBaseImage, const DxLib::BASEIMAGE *AlphaBaseImage, texture2d& GrHandle, bool TextureFlag = true, bool ReverseFlag = false){ return DxLib::ReCreateGraphFromGraphImage(RgbBaseImage, AlphaBaseImage, GrHandle.GetHandle(), TextureFlag, ReverseFlag); }

			//! メモリ上のビットマップイメージからグラフィックを作成する
			//!\~english  Create a graphic from a bitmap image stored in the computer memory
			static inline texture2d CreateGraph(int Width, int Height, int Pitch, const void *RGBImage, const void *AlphaImage = nullptr, const texture2d& GrHandle = texture2d()){ return texture2d(DxLib::CreateGraph(Width, Height, Pitch, RGBImage, AlphaImage, GrHandle.GetHandle()), false); }
			//! メモリ上のビットマップイメージからグラフィックを作成する
			//!\~english  Create a graphic from a bitmap image stored in the computer memory
			static inline texture2d CreateGraph(const sizei& size, int Pitch, const void *RGBImage, const void *AlphaImage = nullptr, const texture2d& GrHandle = texture2d()){ return texture2d(DxLib::CreateGraph(size.width, size.height, Pitch, RGBImage, AlphaImage, GrHandle.GetHandle()), false); }
			//! メモリ上のビットマップイメージからグラフィックを再作成する
			//!\~english  Recreate a graphic from a bitmap image stored in the computer memory
			static inline int ReCreateGraph(int Width, int Height, int Pitch, const void *RGBImage, texture2d& GrHandle, const void *AlphaImage = nullptr){ return DxLib::ReCreateGraph(Width, Height, Pitch, RGBImage, GrHandle.GetHandle(), AlphaImage); }
			//! メモリ上のビットマップイメージからグラフィックを再作成する
			//!\~english  Recreate a graphic from a bitmap image stored in the computer memory
			static inline int ReCreateGraph(const sizei& size, int Pitch, const void *RGBImage, texture2d& GrHandle, const void *AlphaImage = nullptr){ return DxLib::ReCreateGraph(size.width, size.height, Pitch, RGBImage, GrHandle.GetHandle(), AlphaImage); }
#ifndef DX_NON_SOFTIMAGE
			//extern	int			CreateBlendGraphFromSoftImage(int SIHandle);																										// ソフトウエアで扱うイメージからブレンド用画像グラフィックハンドルを作成する( -1:エラー  -1以外:ブレンド用グラフィックハンドル )
			//extern	int			CreateGraphFromSoftImage(int SIHandle);																												// ソフトウエアで扱うイメージからグラフィックハンドルを作成する( -1:エラー  -1以外:グラフィックハンドル )
			//extern	int			CreateGraphFromRectSoftImage(int SIHandle, int x, int y, int SizeX, int SizeY);																		// ソフトウエアで扱うイメージの指定の領域を使ってグラフィックハンドルを作成する( -1:エラー  -1以外:グラフィックハンドル )
			//extern	int			ReCreateGraphFromSoftImage(int SIHandle, int GrHandle);																								// ソフトウエアで扱うイメージから既存のグラフィックハンドルに画像データを転送する
			//extern	int			ReCreateGraphFromRectSoftImage(int SIHandle, int x, int y, int SizeX, int SizeY, int GrHandle);														// ソフトウエアで扱うイメージから既存のグラフィックハンドルに画像データを転送する
			//extern	int			CreateDivGraphFromSoftImage(int SIHandle, int AllNum, int XNum, int YNum, int SizeX, int SizeY, int *HandleBuf);									// ソフトウエアで扱うイメージから分割グラフィックハンドルを作成する
#endif // DX_NON_SOFTIMAGE
			//! 基本イメージデータからグラフィックを作成する
			//!\~english  Create a graphic from a basic image data
			static inline texture2d CreateGraphFromBaseImage(const DxLib::BASEIMAGE *BaseImage){ return texture2d(DxLib::CreateGraphFromBaseImage(BaseImage), false); }
			//! 基本イメージデータの指定の領域を使ってグラフィックを作成する
			//!\~english  Create a graphic from a degsinated area clipped from a basic image data 
			static inline texture2d CreateGraphFromRectBaseImage(const DxLib::BASEIMAGE *BaseImage, int x, int y, int SizeX, int SizeY){ return texture2d(DxLib::CreateGraphFromRectBaseImage(BaseImage, x, y, SizeX, SizeY), false); }
			//! 基本イメージデータの指定の領域を使ってグラフィックを作成する
			static inline texture2d CreateGraphFromRectBaseImage(const DxLib::BASEIMAGE *BaseImage, const pointi& pos, const sizei& size){ return texture2d(DxLib::CreateGraphFromRectBaseImage(BaseImage, pos.x, pos.y, size.width, size.height), false); }
			//! 基本イメージデータから既存のグラフィックに画像データを転送する
			static inline int ReCreateGraphFromBaseImage(const DxLib::BASEIMAGE *BaseImage, texture2d& GrHandle){ return DxLib::ReCreateGraphFromBaseImage(BaseImage, GrHandle.GetHandle()); }
			//! 基本イメージデータの指定の領域を使って既存のグラフィックに画像データを転送する
			static inline int ReCreateGraphFromRectBaseImage(const DxLib::BASEIMAGE *BaseImage, int x, int y, int SizeX, int SizeY, texture2d& GrHandle){ return DxLib::ReCreateGraphFromRectBaseImage(BaseImage, x, y, SizeX, SizeY, GrHandle.GetHandle()); }
			//! 基本イメージデータの指定の領域を使って既存のグラフィックに画像データを転送する
			static inline int ReCreateGraphFromRectBaseImage(const DxLib::BASEIMAGE *BaseImage, const pointi& pos, const sizei& size, texture2d& GrHandle){ return DxLib::ReCreateGraphFromRectBaseImage(BaseImage, pos.x, pos.y, size.width, size.height, GrHandle.GetHandle()); }
			//! 画像ファイルからグラフィックへ画像データを転送する
			static inline int ReloadGraph(const TCHAR *FileName, texture2d& GrHandle, bool ReverseFlag = false){ return DxLib::ReloadGraph(FileName, GrHandle.GetHandle(), ReverseFlag); }
			//! 画像ファイルからグラフィックへ画像データを転送する
			static inline int ReloadGraph(const dxle::tstring& FileName, texture2d& GrHandle, bool ReverseFlag = false){ return DxLib::ReloadGraph(FileName.c_str(), GrHandle.GetHandle(), ReverseFlag); }
			//! ReloadGraph の画像反転処理追加版
			static inline int ReloadReverseGraph(const TCHAR *FileName, texture2d& GrHandle){ return DxLib::ReloadReverseGraph(FileName, GrHandle.GetHandle()); }
			//! ReloadGraph の画像反転処理追加版
			static inline int ReloadReverseGraph(const dxle::tstring& FileName, texture2d& GrHandle){ return DxLib::ReloadReverseGraph(FileName.c_str(), GrHandle.GetHandle()); }


				//メンバ関数

			inline texture2d DerivationGraph(int SrcX, int SrcY, int Width, int Height)const  { return texture2d(DxLib::DerivationGraph(SrcX, SrcY, Width, Height, GetHandle()), false); }
			//! 指定のグラフィックの指定部分だけを抜き出して新たなグラフィックを作成する
			inline texture2d DerivationGraph(const pointi& src, const sizei& size)const  { return texture2d(DxLib::DerivationGraph(src.x, src.y, size.width, size.height, GetHandle()), false); }


			////! 指定のグラフィックのＡＲＧＢ８イメージを取得する( 現在動画ファイルをグラフィックハンドルで読み込んだ場合のみ使用可能 )
			//inline const unsigned int* GetFullColorImage()const  { return DxLib::GetFullColorImage(GetHandle()); }

			//! グラフィックメモリ領域のロック
			inline int GraphLock(int *PitchBuf, void **DataPointBuf, DxLib::COLORDATA **ColorDataPP = nullptr, bool WriteOnly = false){ return DxLib::GraphLock(GetHandle(), PitchBuf, DataPointBuf, ColorDataPP, WriteOnly); }
			//! グラフィックメモリ領域のロック解除
			inline int GraphUnLock(){ return DxLib::GraphUnLock(GetHandle()); }
			//! グラフィックスデバイスのデバイスロスト発生時に指定のグラフィックを削除するかどうかを設定する( TRUE:デバイスロスト時に削除する  FALSE:デバイスロストが発生しても削除しない )
			inline int SetDeviceLostDeleteGraphFlag(bool DeleteFlag){ return DxLib::SetDeviceLostDeleteGraphFlag(GetHandle(), DeleteFlag); }
			//! グラフィックのサイズを得る
			inline int GetGraphSize(int *SizeXBuf, int *SizeYBuf)const { return DxLib::GetGraphSize(GetHandle(), SizeXBuf, SizeYBuf); }
			//! グラフィックのサイズを得る
			inline sizei GetGraphSize()const { int x, y; GetGraphSize(&x, &y); return{ x, y }; }
			//! グラフィックが持つ一つ目のテクスチャのサイズを得る
			inline int GetGraphTextureSize(int *SizeXBuf, int *SizeYBuf)const { return DxLib::GetGraphTextureSize(GetHandle(), SizeXBuf, SizeYBuf); }
			//! グラフィックが持つ一つ目のテクスチャのサイズを得る
			inline sizei GetGraphTextureSize()const { int x, y; GetGraphTextureSize(&x, &y); return{ x, y }; }
			//! グラフィックが持つテクスチャのミップマップレベル数を取得する
			inline int GetGraphMipmapCount()const { return DxLib::GetGraphMipmapCount(GetHandle()); }
			//! グラフィックが画像ファイルから読み込まれていた場合、その画像のファイルパスを取得する
			inline int GetGraphFilePath(TCHAR FilePathBuffer[])const { return DxLib::GetGraphFilePath(GetHandle(), FilePathBuffer); }
			//! グラフィックが画像ファイルから読み込まれていた場合、その画像のファイルパスを取得する
			inline tstring GetGraphFilePath()const;

			//! 画像の等倍描画
			inline int DrawGraph(int x, int y, bool TransFlag)const ;
			//! 画像の等倍描画
			inline int DrawGraph(const pointi& p, bool TransFlag)const ;
			//! 画像の拡大描画
			inline int DrawExtendGraph(int x1, int y1, int x2, int y2, bool TransFlag)const ;
			//! 画像の拡大描画
			inline int DrawExtendGraph(const pointi& lu, const pointi& rb, bool TransFlag)const ;
			//! 画像の回転描画
			inline int DrawRotaGraph(int x, int y, double ExRate, double Angle, bool TransFlag, bool TurnFlag = false)const ;
			//! 画像の回転描画
			inline int DrawRotaGraph(const pointi& p, double ExRate, double Angle, bool TransFlag, bool TurnFlag = false)const ;
			//! 画像の回転描画２( 回転中心指定付き )
			inline int DrawRotaGraph2(int x, int y, int cx, int cy, double ExtRate, double Angle, bool TransFlag, bool TurnFlag = false)const ;
			//! 画像の回転描画２( 回転中心指定付き )
			inline int DrawRotaGraph2(const pointi& p, const pointi& c, double ExtRate, double Angle, bool TransFlag, bool TurnFlag = false)const ;
			//! 画像の回転描画３( 回転中心指定付き＋縦横拡大率別指定版 )
			inline int DrawRotaGraph3(int x, int y, int cx, int cy, double ExtRateX, double ExtRateY, double Angle, bool TransFlag, bool TurnFlag = FALSE)const ;
			//! 画像の回転描画３( 回転中心指定付き＋縦横拡大率別指定版 )
			inline int DrawRotaGraph3(const pointi& p, const pointi& c, double ExtRateX, double ExtRateY, double Angle, bool TransFlag, bool TurnFlag = FALSE)const ;
			//! 画像の自由変形描画
			inline int DrawModiGraph(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4, bool TransFlag)const ;
			//! 画像の自由変形描画
			inline int DrawModiGraph(const std::array<pointi, 4>& ps, bool TransFlag)const ;
			//! 画像の左右反転描画
			inline int DrawTurnGraph(int x, int y, bool TransFlag)const ;
			//! 画像の左右反転描画
			inline int DrawTurnGraph(const pointi& p, bool TransFlag)const ;
			//! 画像の拡大左右反転描画
			inline int DrawExtendTurnGraph(int x1, int y1, int x2, int y2, bool TransFlag)const ;//x1とx2はこれで正しい
			//! 画像の拡大左右反転描画
			inline int DrawExtendTurnGraph(const pointi& lu, const pointi& rb, bool TransFlag)const ;//lu.xとrb.xはこれで正しい

			//! 画像の描画( 座標指定が float 版 )
			inline int DrawGraphF(float xf, float yf, bool TransFlag)const ;
			//! 画像の描画( 座標指定が float 版 )
			inline int DrawGraphF(const pointf& p, bool TransFlag)const ;
			//! 画像の拡大描画( 座標指定が float 版 )
			inline int DrawExtendGraphF(float x1f, float y1f, float x2f, float y2, bool TransFlag)const ;
			//! 画像の拡大描画( 座標指定が float 版 )
			inline int DrawExtendGraphF(const pointf& lu, const pointf& rb, bool TransFlag)const ;
			//! 画像の回転描画( 座標指定が float 版 )
			inline int DrawRotaGraphF(float xf, float yf, double ExRate, double Angle, bool TransFlag, bool TurnFlag = FALSE)const ;
			//! 画像の回転描画( 座標指定が float 版 )
			inline int DrawRotaGraphF(const pointf& p, double ExRate, double Angle, bool TransFlag, bool TurnFlag = FALSE)const ;
			//! 画像の回転描画２( 回転中心指定付き )( 座標指定が float 版 )
			inline int DrawRotaGraph2F(float xf, float yf, float cxf, float cyf, double ExtRate, double Angle, bool TransFlag, bool TurnFlag = FALSE)const ;
			//! 画像の回転描画２( 回転中心指定付き )( 座標指定が float 版 )
			inline int DrawRotaGraph2F(const pointf& p, const pointf& c, double ExtRate, double Angle, bool TransFlag, bool TurnFlag = FALSE)const ;
			//! 画像の回転描画３( 回転中心指定付き＋縦横拡大率別指定版 )( 座標指定が float 版 )
			inline int DrawRotaGraph3F(float xf, float yf, float cxf, float cyf, double ExtRateX, double ExtRateY, double Angle, bool TransFlag, bool TurnFlag = FALSE)const ;
			//! 画像の回転描画３( 回転中心指定付き＋縦横拡大率別指定版 )( 座標指定が float 版 )
			inline int DrawRotaGraph3F(const pointf& p, const pointf& c, double ExtRateX, double ExtRateY, double Angle, bool TransFlag, bool TurnFlag = FALSE)const ;
			//! 画像の自由変形描画( 座標指定が float 版 )
			inline int DrawModiGraphF(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4, bool TransFlag)const ;
			//! 画像の自由変形描画( 座標指定が float 版 )
			inline int DrawModiGraphF(const std::array<pointf, 4>& ps, bool TransFlag)const ;
			//! 画像の左右反転描画( 座標指定が float 版 )
			inline int DrawTurnGraphF(float xf, float yf, bool TransFlag)const ;
			//! 画像の左右反転描画( 座標指定が float 版 )
			inline int DrawTurnGraphF(const pointf& p, bool TransFlag)const ;
			//! 画像の拡大左右反転描画( 座標指定が float 版 )
			inline int DrawExtendTurnGraphF(float x1f, float y1f, float x2f, float y2f, bool TransFlag)const ;//x1fとx2fはこれで正しい
			//! 画像の拡大左右反転描画( 座標指定が float 版 )
			inline int DrawExtendTurnGraphF(const pointf& lu, const pointf& rb, bool TransFlag)const ;//x1fとx2fはこれで正しい

			int filter_mono			(int16_t Cb, int16_t Cr);
			int filter_gaussian		(uint16_t PixelWidth, int Param);
			int filter_down_scale	(uint8_t DivNum);
			int filter_bright_clip	(bright_clip_mode clipmode, uint8_t clip_pram, unsigned int fillcolor, uint8_t fillalpha);
			int filter_HSB			(bool HueType, int16_t Hue, int Saturation, int16_t Bright);
			int filter_invert		();
			int filter_level		(uint8_t min, uint8_t max, int Gamma, uint8_t Aftermin, uint8_t Aftermax);
			int filter_two_color	(uint8_t threshold, unsigned int LowColor, uint8_t LowAlpha, unsigned int HighColor, uint8_t HighAlpha);


		private:
			texture2d(int handle, bool NotUse3DFlag)
				: texture2d(gr_impl::simple_graph_handle_manager::get_unique(handle, NotUse3DFlag))
			{}
			texture2d(std::unique_ptr<gr_impl::texture2d_handle_manager>&& handle_manager_)
				: handle_manager(std::move(handle_manager_))
			{}
			std::unique_ptr<gr_impl::texture2d_handle_manager> handle_manager;

			int GetHandle()const{ if (!handle_manager){ return -1; } return handle_manager->get_handle(); }

			//ユーザーが継承するのを防止するためprotectedではなくfriendを使う

			friend screen;
			friend animation_graph;

			//実装用

			template<typename BuffT, typename OutFunc>
			friend void gr_impl::LoadDivGraph_impl(OutFunc&& out_func, BuffT* HandleBuf, const TCHAR *FileName, int AllNum, const dxle::sizei& Num, const dxle::sizei& Size, bool NotUse3DFlag);
			friend gr_impl::texture2d_handle_manager;
		};
		
		//! 描画可能画像クラス
		class screen final : public texture2d
		{
		public:
			//!画像を複製する
			std::unique_ptr<texture2d> clone()const override{ return cloneSc(); }
			//!画像を複製する
			std::unique_ptr<screen> cloneSc()const;

			//生成用static関数

			//! SetDrawScreen で描画対象にできるグラフィックを作成する
			static inline screen MakeScreen(int SizeX, int SizeY, bool UseAlphaChannel = false)DXLE_NOEXCEPT_OR_NOTHROW{ return screen(DxLib::MakeScreen(SizeX, SizeY, UseAlphaChannel), UseAlphaChannel); }
			//! SetDrawScreen で描画対象にできるグラフィックを作成する
			static inline screen MakeScreen(const sizei& size, bool UseAlphaChannel = false)DXLE_NOEXCEPT_OR_NOTHROW{ return screen(DxLib::MakeScreen(size.width, size.height, UseAlphaChannel), UseAlphaChannel); }

			//メンバ関数

			template<typename Func_T>
			screen& draw_on_this(Func_T&& draw_func);

			//! グラフィック専用のＺバッファを持つかどうかを設定する
			//!@param UseFlag 専用のＺバッファを持つかどうか( true:持つ( デフォルト )  false:持たない )
			//!@param BitDepth ビット深度( 16 or 24 or 32 ) )
			inline int SetUseGraphZBuffer(bool UseFlag, int BitDepth = -1)DXLE_NOEXCEPT_OR_NOTHROW{ return DxLib::SetUseGraphZBuffer(GetHandle(), UseFlag, BitDepth); }
			//! グラフィックのＺバッファの状態を別のグラフィックのＺバッファにコピーする
			//! *thisはＺバッファを持っている描画対象にできるグラフィック
			//!@param ToGraph *thisとサイズが同じであり、且つマルチサンプリング( アンチエイリアス )設定が無い、Ｚバッファを持っている描画対象にできるグラフィック
			inline int CopyGraphZBufferImage(screen& ToGraph)const DXLE_NOEXCEPT_OR_NOTHROW{ return DxLib::CopyGraphZBufferImage(GetHandle(), ToGraph.GetHandle()); }
			//! SetDrawScreen で描画対象にできるグラフィックハンドルから指定領域の画像情報を別のグラフィックハンドルに転送する
			//!@param x1 転送元の矩形の左座標
			//!@param y1 転送元の矩形の上座標
			//!@param x2 転送元の矩形の右座標
			//!@param y2 転送元の矩形の下座標
			//!@param DestX, DestY 転送先の矩形の左上座標
			//!@param DestGrHandle 転送先の画像
			inline int BltDrawValidGraph(int x1, int y1, int x2, int y2, int DestX, int DestY, texture2d& DestGrHandle)const DXLE_NOEXCEPT_OR_NOTHROW{ return DxLib::BltDrawValidGraph(GetHandle(), x1, y1, x2, y2, DestX, DestY, DestGrHandle.GetHandle()); }
			//! SetDrawScreen で描画対象にできるグラフィックハンドルから指定領域の画像情報を別のグラフィックハンドルに転送する
			//!@param lu 転送元の矩形の左上座標
			//!@param rb 転送元の矩形の右下座標
			//!@param dest 転送先の矩形の左上座標
			//!@param DestGrHandle 転送先の画像
			inline int BltDrawValidGraph(const pointi& lu, const pointi& rb, const pointi& dest, texture2d& DestGrHandle)const DXLE_NOEXCEPT_OR_NOTHROW{ return DxLib::BltDrawValidGraph(GetHandle(), rb.x, lu.y, lu.x, rb.y, dest.x, dest.y, DestGrHandle.GetHandle()); }
			//! 描画先画面を設定する
			inline int SetDrawScreen()const DXLE_NOEXCEPT_OR_NOTHROW_SINGLE{ DXLE_GET_LOCK(screen_mutex_c::mtx); return DxLib::SetDrawScreen(GetHandle()); }

		public:
			screen()DXLE_NOEXCEPT_OR_NOTHROW {}
			//! コピー禁止
			screen(const screen& other) = delete;
			//!所有権の譲渡
			screen(screen&& other) DXLE_NOEXCEPT_OR_NOTHROW : texture2d(std::move(other)){}
			//! コピー禁止
			screen& operator=(const screen& other) = delete;
			//! 所有権の譲渡
			screen& operator=(screen&& other) DXLE_NOEXCEPT_OR_NOTHROW{ texture2d::operator=(std::move(other)); return *this; }

		protected:
			screen(int handle, bool UseAlphaChannel)
				: texture2d(gr_impl::screen_handle_manager::get_unique(handle, UseAlphaChannel))
			{}
		};
		class shared_texture2d final
		{
		public:
			shared_texture2d()DXLE_NOEXCEPT_OR_NOTHROW{}
			template<typename texture2d_t> shared_texture2d(const texture2d_t& unique_tex) = delete;
			template<typename texture2d_t, enable_if_t<std::is_base_of<texture2d, texture2d_t>::value, nullptr_t> = nullptr>
			shared_texture2d(texture2d_t&& unique_tex)
				: impl(std::make_shared<texture2d_t>(std::forward<texture2d_t>(unique_tex)))
			{}

			shared_texture2d(const shared_texture2d& other)DXLE_NOEXCEPT_OR_NOTHROW : impl(other.impl){}
			shared_texture2d& operator=(const shared_texture2d& other)DXLE_NOEXCEPT_OR_NOTHROW { impl = (other.impl); return *this; }
			shared_texture2d(shared_texture2d&& other) DXLE_NOEXCEPT_OR_NOTHROW : impl(std::move(other.impl)){}
			shared_texture2d& operator=(shared_texture2d&& other) DXLE_NOEXCEPT_OR_NOTHROW{ impl = (std::move(other.impl)); return *this; }

#if 1
			texture2d& get()DXLE_NOEXCEPT_OR_NOTHROW{ return *impl; }
			const texture2d& get()const DXLE_NOEXCEPT_OR_NOTHROW{ return *impl; }
			operator texture2d&()DXLE_NOEXCEPT_OR_NOTHROW{ return *impl; }
			operator const texture2d&()const DXLE_NOEXCEPT_OR_NOTHROW{ return *impl; }
			//!\~japanese 有効な画像を持っているか調べる（trueで持っている）
			//!\~english  Returns whether this object has valid texture (if it has, this functoin returns true)
			inline bool valid()const { return (impl != nullptr && impl->valid()); }
#else
			texture2d* operator->()DXLE_NOEXCEPT_OR_NOTHROW{ return impl.get(); }
			const texture2d* operator->()const DXLE_NOEXCEPT_OR_NOTHROW{ return impl.get(); }
			texture2d& operator*()DXLE_NOEXCEPT_OR_NOTHROW{ return impl.get(); }
			const texture2d& operator*()const DXLE_NOEXCEPT_OR_NOTHROW{ return impl.get(); }
#endif

		private:
			std::shared_ptr<texture2d> impl;
		};

		//!N = AllNum
		template<size_t N>
		//!画像ファイルを分割したグラフィック
		//!DxLib::LoadDivGraphのdxle::textureバージョン
		//!AllSizeが定数の場合
		//!moveに線形時間がかかることに注意
		class static_derivative_texture2d final
		{
		private:
			std::array<texture2d, N> textures;
			using cont_type = decltype(textures);
		public:

			DXLE_CONSTEXPR static_derivative_texture2d()DXLE_NOEXCEPT_OR_NOTHROW{}
			static_derivative_texture2d(const static_derivative_texture2d&) = delete;
			static_derivative_texture2d(static_derivative_texture2d&&)DXLE_NOEXCEPT_OR_NOTHROW;
			static_derivative_texture2d& operator=(const static_derivative_texture2d&) = delete;
			static_derivative_texture2d& operator=(static_derivative_texture2d&&)DXLE_NOEXCEPT_OR_NOTHROW;

		//----------生成関数----------//

			static_derivative_texture2d(const TCHAR *FileName, const dxle::sizei& Num, const dxle::sizei& Size, bool NotUse3DFlag = false);
			static_derivative_texture2d(const tstring& FileName, const dxle::sizei& Num, const dxle::sizei& Size, bool NotUse3DFlag = false);

			void load(const TCHAR *FileName, const dxle::sizei& Num, const dxle::sizei& Size, bool NotUse3DFlag = false);
			void load(const tstring& FileName, const dxle::sizei& Num, const dxle::sizei& Size, bool NotUse3DFlag = false);

		//----------utility----------//

			void delete_all();

		//----------コンテナ部----------//

			using reference              = typename cont_type::reference;
			using const_reference        = typename cont_type::const_reference;
			using iterator               = typename cont_type::iterator;
			using const_iterator         = typename cont_type::const_iterator;
			using reverse_iterator       = typename cont_type::reverse_iterator;
			using const_reverse_iterator = typename cont_type::const_reverse_iterator;
			using size_type              = typename cont_type::size_type;
			using difference_type        = typename cont_type::difference_type;
			using pointer                = typename cont_type::pointer;
			using const_pointer          = typename cont_type::const_pointer;
			using value_type             = typename cont_type::value_type;

			      reference at(size_t index)     { return textures.at(index); }
			const_reference at(size_t index)const{ return textures.at(index); }
			      reference operator[](size_t index)     { return textures[index]; }
			const_reference operator[](size_t index)const{ return textures[index]; }
			      reference front()     { return textures.front(); }
			const_reference front()const{ return textures.front(); }
			      reference back()     { return textures.back(); }
			const_reference back()const{ return textures.back(); }
			      pointer data()      DXLE_NOEXCEPT_OR_NOTHROW{ return textures.data(); }
			const_pointer data()const DXLE_NOEXCEPT_OR_NOTHROW{ return textures.data(); }

			      iterator begin ()      DXLE_NOEXCEPT_OR_NOTHROW{ return textures.begin (); }
			const_iterator begin ()const DXLE_NOEXCEPT_OR_NOTHROW{ return textures.begin (); }
			      iterator end   ()      DXLE_NOEXCEPT_OR_NOTHROW{ return textures.end   (); }
			const_iterator end   ()const DXLE_NOEXCEPT_OR_NOTHROW{ return textures.end   (); }
			const_iterator cbegin()const DXLE_NOEXCEPT_OR_NOTHROW{ return textures.cbegin(); }
			const_iterator cend  ()const DXLE_NOEXCEPT_OR_NOTHROW{ return textures.cend  (); }
			      reverse_iterator rbegin ()      DXLE_NOEXCEPT_OR_NOTHROW{ return textures.rbegin (); }
			const_reverse_iterator rbegin ()const DXLE_NOEXCEPT_OR_NOTHROW{ return textures.rbegin (); }
			      reverse_iterator rend   ()      DXLE_NOEXCEPT_OR_NOTHROW{ return textures.rend   (); }
			const_reverse_iterator rend   ()const DXLE_NOEXCEPT_OR_NOTHROW{ return textures.rend   (); }
			const_reverse_iterator crbegin()const DXLE_NOEXCEPT_OR_NOTHROW{ return textures.crbegin(); }
			const_reverse_iterator crend  ()const DXLE_NOEXCEPT_OR_NOTHROW{ return textures.crend  (); }

			DXLE_CONSTEXPR bool empty()const DXLE_NOEXCEPT_OR_NOTHROW{ return textures.empty(); }
			DXLE_CONSTEXPR size_type size()const DXLE_NOEXCEPT_OR_NOTHROW { return textures.size(); }
			DXLE_CONSTEXPR size_type max_size()const DXLE_NOEXCEPT_OR_NOTHROW{ return textures.max_size(); }

			void swap(static_derivative_texture2d& other){ textures.swap(other.textures); }
		};
		//!画像ファイルを分割したグラフィック
		//!DxLib::LoadDivGraphのdxle::textureバージョン
		//!moveは定数時間
		class derivative_texture2d final
		{
		private:
			std::vector<texture2d> textures;
			using cont_type = decltype(textures);
		public:

			derivative_texture2d()DXLE_NOEXCEPT_OR_NOTHROW{}
			derivative_texture2d(const derivative_texture2d&) = delete;
			derivative_texture2d(derivative_texture2d&&)DXLE_NOEXCEPT_IF((std::is_nothrow_move_constructible<cont_type>::value));
			derivative_texture2d& operator=(const derivative_texture2d&) = delete;
			derivative_texture2d& operator=(derivative_texture2d&&)DXLE_NOEXCEPT_IF((std::is_nothrow_move_assignable<cont_type>::value));

			//----------生成関数----------//

			derivative_texture2d(const TCHAR *FileName, int AllNum, const dxle::sizei& Num, const dxle::sizei& Size, bool NotUse3DFlag = false);
			derivative_texture2d(const tstring& FileName, int AllNum, const dxle::sizei& Num, const dxle::sizei& Size, bool NotUse3DFlag = false);

			void load(const TCHAR *FileName, int AllNum, const dxle::sizei& Num, const dxle::sizei& Size, bool NotUse3DFlag = false);
			void load(const tstring& FileName, int AllNum, const dxle::sizei& Num, const dxle::sizei& Size, bool NotUse3DFlag = false);

			static inline derivative_texture2d LoadDivGraph(const TCHAR *FileName, int AllNum, const dxle::sizei& Num, const dxle::sizei& Size, bool NotUse3DFlag = false);
			static inline derivative_texture2d LoadDivGraph(const tstring& FileName, int AllNum, const dxle::sizei& Num, const dxle::sizei& Size, bool NotUse3DFlag = false);

			//----------utility----------//

			void delete_all();

			//----------コンテナ部----------//

			using reference              = cont_type::reference;
			using const_reference        = cont_type::const_reference;
			using iterator               = cont_type::iterator;
			using const_iterator         = cont_type::const_iterator;
			using reverse_iterator       = cont_type::reverse_iterator;
			using const_reverse_iterator = cont_type::const_reverse_iterator;
			using size_type              = cont_type::size_type;
			using difference_type        = cont_type::difference_type;
			using pointer                = cont_type::pointer;
			using const_pointer          = cont_type::const_pointer;
			using value_type             = cont_type::value_type;

			      reference at(size_t index)     { return textures.at(index); }
			const_reference at(size_t index)const{ return textures.at(index); }
			      reference operator[](size_t index)     { return textures[index]; }
			const_reference operator[](size_t index)const{ return textures[index]; }
			      reference front()     { return textures.front(); }
			const_reference front()const{ return textures.front(); }
			      reference back()     { return textures.back(); }
			const_reference back()const{ return textures.back(); }
			      pointer data()      DXLE_NOEXCEPT_OR_NOTHROW{ return textures.data(); }
			const_pointer data()const DXLE_NOEXCEPT_OR_NOTHROW{ return textures.data(); }

			      iterator begin ()      DXLE_NOEXCEPT_OR_NOTHROW{ return textures.begin (); }
			const_iterator begin ()const DXLE_NOEXCEPT_OR_NOTHROW{ return textures.begin (); }
			      iterator end   ()      DXLE_NOEXCEPT_OR_NOTHROW{ return textures.end   (); }
			const_iterator end   ()const DXLE_NOEXCEPT_OR_NOTHROW{ return textures.end   (); }
			const_iterator cbegin()const DXLE_NOEXCEPT_OR_NOTHROW{ return textures.cbegin(); }
			const_iterator cend  ()const DXLE_NOEXCEPT_OR_NOTHROW{ return textures.cend  (); }
			      reverse_iterator rbegin ()      DXLE_NOEXCEPT_OR_NOTHROW{ return textures.rbegin (); }
			const_reverse_iterator rbegin ()const DXLE_NOEXCEPT_OR_NOTHROW{ return textures.rbegin (); }
			      reverse_iterator rend   ()      DXLE_NOEXCEPT_OR_NOTHROW{ return textures.rend   (); }
			const_reverse_iterator rend   ()const DXLE_NOEXCEPT_OR_NOTHROW{ return textures.rend   (); }
			const_reverse_iterator crbegin()const DXLE_NOEXCEPT_OR_NOTHROW{ return textures.crbegin(); }
			const_reverse_iterator crend  ()const DXLE_NOEXCEPT_OR_NOTHROW{ return textures.crend  (); }

			bool empty()const DXLE_NOEXCEPT_OR_NOTHROW{ return textures.empty(); }
			size_type size()const DXLE_NOEXCEPT_OR_NOTHROW { return textures.size(); }
			size_type max_size()const DXLE_NOEXCEPT_OR_NOTHROW{ return textures.max_size(); }

			void swap(derivative_texture2d& other){ textures.swap(other.textures); }
		};

		
	//--------------------生成用関数--------------------//

		// グラフィック作成関係関数

		//! 指定サイズのグラフィックを作成する
		inline texture2d MakeGraph(int SizeX, int SizeY, bool NotUse3DFlag = false)DXLE_NOEXCEPT_OR_NOTHROW { return texture2d::MakeGraph(SizeX, SizeY, NotUse3DFlag); }
		//! 指定サイズのグラフィックを作成する
		inline texture2d MakeGraph(const sizei& size, bool NotUse3DFlag = false)DXLE_NOEXCEPT_OR_NOTHROW { return texture2d::MakeGraph(size.width, size.height, NotUse3DFlag); }
		//! SetDrawScreen で描画対象にできるグラフィックを作成する
		inline screen MakeScreen(int SizeX, int SizeY, bool UseAlphaChannel = false)DXLE_NOEXCEPT_OR_NOTHROW { return screen::MakeScreen(SizeX, SizeY, UseAlphaChannel); }
		//! SetDrawScreen で描画対象にできるグラフィックを作成する
		inline screen MakeScreen(const sizei& size, bool UseAlphaChannel = false)DXLE_NOEXCEPT_OR_NOTHROW { return screen::MakeScreen(size.width, size.height, UseAlphaChannel); }
		//! 指定のグラフィックの指定部分だけを抜き出して新たなグラフィックを作成する
		inline texture2d DerivationGraph(int SrcX, int SrcY, int Width, int Height, const texture2d& SrcGraphHandle)DXLE_NOEXCEPT_OR_NOTHROW{ return texture2d::DerivationGraph(SrcX, SrcY, Width, Height, SrcGraphHandle); }
		//! 指定のグラフィックの指定部分だけを抜き出して新たなグラフィックを作成する
		inline texture2d DerivationGraph(const pointi& src, const sizei& size, const texture2d& SrcGraphHandle)DXLE_NOEXCEPT_OR_NOTHROW{ return texture2d::DerivationGraph(src.x, src.y, size.width, size.height, SrcGraphHandle); }

		// 画像からグラフィックを作成する関数

		//! 画像ファイルからグラフィックを作成する
		inline texture2d LoadBmpToGraph(const TCHAR *FileName, int TextureFlag, int ReverseFlag, int SurfaceMode = DX_MOVIESURFACE_NORMAL)DXLE_NOEXCEPT_OR_NOTHROW { return texture2d::LoadBmpToGraph(FileName, TextureFlag, ReverseFlag, SurfaceMode); }
		//! 画像ファイルからグラフィックを作成する
		inline texture2d LoadBmpToGraph(const tstring& FileName, int TextureFlag, int ReverseFlag, int SurfaceMode = DX_MOVIESURFACE_NORMAL)DXLE_NOEXCEPT_OR_NOTHROW { return texture2d::LoadBmpToGraph(FileName.c_str(), TextureFlag, ReverseFlag, SurfaceMode); }
		//! 画像ファイルからグラフィックを作成する
		inline texture2d LoadGraph(const TCHAR *FileName, bool NotUse3DFlag = false)DXLE_NOEXCEPT_OR_NOTHROW { return texture2d::LoadGraph(FileName, NotUse3DFlag); }
		//! 画像ファイルからグラフィックを作成する
		inline texture2d LoadGraph(const tstring& FileName, bool NotUse3DFlag = false)DXLE_NOEXCEPT_OR_NOTHROW { return texture2d::LoadGraph(FileName.c_str(), NotUse3DFlag); }
		//! 画像ファイルを反転したものでグラフィックを作成する
		inline texture2d LoadReverseGraph(const TCHAR *FileName, bool NotUse3DFlag = false)DXLE_NOEXCEPT_OR_NOTHROW { return texture2d::LoadReverseGraph(FileName, NotUse3DFlag); }
		//! 画像ファイルを反転したものでグラフィックを作成する
		inline texture2d LoadReverseGraph(const tstring& FileName, bool NotUse3DFlag = false)DXLE_NOEXCEPT_OR_NOTHROW { return texture2d::LoadReverseGraph(FileName.c_str(), NotUse3DFlag); }

		//! 画像ファイルを分割してグラフィックハンドルを作成する
		inline derivative_texture2d LoadDivGraph(const TCHAR *FileName, int AllNum, const dxle::sizei& Num, const dxle::sizei& Size, bool NotUse3DFlag = false);
		//! 画像ファイルを分割してグラフィックハンドルを作成する
		inline derivative_texture2d LoadDivGraph(const tstring& FileName, int AllNum, const dxle::sizei& Num, const dxle::sizei& Size, bool NotUse3DFlag = false);

		//! メモリ上の画像イメージからグラフィックを作成する
		inline texture2d CreateGraphFromMem(const void *RGBFileImage, int RGBFileImageSize, const void *AlphaFileImage = nullptr, int AlphaFileImageSize = 0, bool TextureFlag = true, bool ReverseFlag = false)DXLE_NOEXCEPT_OR_NOTHROW { return texture2d::CreateGraphFromMem(RGBFileImage, RGBFileImageSize, AlphaFileImage, AlphaFileImageSize, TextureFlag, ReverseFlag); }
		//! メモリ上の画像イメージから既存のグラフィックにデータを転送する
		inline int ReCreateGraphFromMem(const void *RGBFileImage, int RGBFileImageSize, texture2d& GrHandle, const void *AlphaFileImage = nullptr, int AlphaFileImageSize = 0, bool TextureFlag = true, bool ReverseFlag = false)DXLE_NOEXCEPT_OR_NOTHROW { return texture2d::ReCreateGraphFromMem(RGBFileImage, RGBFileImageSize, GrHandle, AlphaFileImage, AlphaFileImageSize, TextureFlag, ReverseFlag); }

		//! 基本イメージデータからサイズを割り出し、それに合ったグラフィックを作成する
		inline texture2d CreateDXGraph(const DxLib::BASEIMAGE *RgbBaseImage, const DxLib::BASEIMAGE *AlphaBaseImage, bool TextureFlag)DXLE_NOEXCEPT_OR_NOTHROW { return texture2d::CreateDXGraph(RgbBaseImage, AlphaBaseImage, TextureFlag); }
		//! 基本イメージデータからグラフィックを作成する
		inline texture2d CreateGraphFromGraphImage(const DxLib::BASEIMAGE *RgbBaseImage, bool TextureFlag = true, bool ReverseFlag = false)DXLE_NOEXCEPT_OR_NOTHROW { return texture2d::CreateGraphFromGraphImage(RgbBaseImage, TextureFlag, ReverseFlag); }
		//! 基本イメージデータからグラフィックを作成する
		inline texture2d CreateGraphFromGraphImage(const DxLib::BASEIMAGE *RgbBaseImage, const DxLib::BASEIMAGE *AlphaBaseImage, bool TextureFlag = true, bool ReverseFlag = false)DXLE_NOEXCEPT_OR_NOTHROW { return texture2d::CreateGraphFromGraphImage(RgbBaseImage, AlphaBaseImage, TextureFlag, ReverseFlag); }
		//! 基本イメージデータから既存のグラフィックにデータを転送する
		inline int ReCreateGraphFromGraphImage(const DxLib::BASEIMAGE *RgbBaseImage, texture2d& GrHandle, bool TextureFlag = true, bool ReverseFlag = false)DXLE_NOEXCEPT_OR_NOTHROW { return texture2d::ReCreateGraphFromGraphImage(RgbBaseImage, GrHandle, TextureFlag, ReverseFlag); }
		//! 基本イメージデータから既存のグラフィックにデータを転送する
		inline int ReCreateGraphFromGraphImage(const DxLib::BASEIMAGE *RgbBaseImage, const DxLib::BASEIMAGE *AlphaBaseImage, texture2d& GrHandle, bool TextureFlag = true, bool ReverseFlag = false)DXLE_NOEXCEPT_OR_NOTHROW { return texture2d::ReCreateGraphFromGraphImage(RgbBaseImage, AlphaBaseImage, GrHandle, TextureFlag, ReverseFlag); }

		//! メモリ上のビットマップイメージからグラフィックを作成する
		inline texture2d CreateGraph(int Width, int Height, int Pitch, const void *RGBImage, const void *AlphaImage = nullptr, const texture2d& GrHandle = texture2d())DXLE_NOEXCEPT_OR_NOTHROW { return texture2d::CreateGraph(Width, Height, Pitch, RGBImage, AlphaImage, GrHandle); }
		//! メモリ上のビットマップイメージからグラフィックを作成する
		inline texture2d CreateGraph(const sizei& size, int Pitch, const void *RGBImage, const void *AlphaImage = nullptr, const texture2d& GrHandle = texture2d())DXLE_NOEXCEPT_OR_NOTHROW { return texture2d::CreateGraph(size.width, size.height, Pitch, RGBImage, AlphaImage, GrHandle); }
		//! メモリ上のビットマップイメージからグラフィックを再作成する
		inline int ReCreateGraph(int Width, int Height, int Pitch, const void *RGBImage, texture2d& GrHandle, const void *AlphaImage = nullptr)DXLE_NOEXCEPT_OR_NOTHROW { return texture2d::ReCreateGraph(Width, Height, Pitch, RGBImage, GrHandle, AlphaImage); }
		//! メモリ上のビットマップイメージからグラフィックを再作成する
		inline int ReCreateGraph(const sizei& size, int Pitch, const void *RGBImage, texture2d& GrHandle, const void *AlphaImage = nullptr)DXLE_NOEXCEPT_OR_NOTHROW { return texture2d::ReCreateGraph(size.width, size.height, Pitch, RGBImage, GrHandle, AlphaImage); }
#ifndef DX_NON_SOFTIMAGE
#endif // DX_NON_SOFTIMAGE
		//! 基本イメージデータからグラフィックを作成する
		inline texture2d CreateGraphFromBaseImage(const DxLib::BASEIMAGE *BaseImage)DXLE_NOEXCEPT_OR_NOTHROW { return texture2d::CreateGraphFromBaseImage(BaseImage); }
		//! 基本イメージデータの指定の領域を使ってグラフィックを作成する
		inline texture2d CreateGraphFromRectBaseImage(const DxLib::BASEIMAGE *BaseImage, int x, int y, int SizeX, int SizeY)DXLE_NOEXCEPT_OR_NOTHROW { return texture2d::CreateGraphFromRectBaseImage(BaseImage, x, y, SizeX, SizeY); }
		//! 基本イメージデータの指定の領域を使ってグラフィックを作成する
		inline texture2d CreateGraphFromRectBaseImage(const DxLib::BASEIMAGE *BaseImage, const pointi& p, const sizei& size)DXLE_NOEXCEPT_OR_NOTHROW { return texture2d::CreateGraphFromRectBaseImage(BaseImage, p.x, p.y, size.width, size.height); }
		//! 基本イメージデータから既存のグラフィックに画像データを転送する
		inline int ReCreateGraphFromBaseImage(const DxLib::BASEIMAGE *BaseImage, texture2d& GrHandle)DXLE_NOEXCEPT_OR_NOTHROW { return texture2d::ReCreateGraphFromBaseImage(BaseImage, GrHandle); }
		//! 基本イメージデータの指定の領域を使って既存のグラフィックに画像データを転送する
		inline int ReCreateGraphFromRectBaseImage(const DxLib::BASEIMAGE *BaseImage, int x, int y, int SizeX, int SizeY, texture2d& GrHandle)DXLE_NOEXCEPT_OR_NOTHROW { return texture2d::ReCreateGraphFromRectBaseImage(BaseImage, x, y, SizeX, SizeY, GrHandle); }
		//! 基本イメージデータの指定の領域を使って既存のグラフィックに画像データを転送する
		inline int ReCreateGraphFromRectBaseImage(const DxLib::BASEIMAGE *BaseImage, const pointi& p, const sizei& size, texture2d& GrHandle)DXLE_NOEXCEPT_OR_NOTHROW { return texture2d::ReCreateGraphFromRectBaseImage(BaseImage, p.x, p.y, size.width, size.height, GrHandle); }
		//! 画像ファイルからグラフィックへ画像データを転送する
		inline int ReloadGraph(const TCHAR *FileName, texture2d& GrHandle, bool ReverseFlag = false)DXLE_NOEXCEPT_OR_NOTHROW { return texture2d::ReloadGraph(FileName, GrHandle, ReverseFlag); }
		//! 画像ファイルからグラフィックへ画像データを転送する
		inline int ReloadGraph(const tstring& FileName, texture2d& GrHandle, bool ReverseFlag = false)DXLE_NOEXCEPT_OR_NOTHROW { return texture2d::ReloadGraph(FileName.c_str(), GrHandle, ReverseFlag); }
		//! ReloadGraph の画像反転処理追加版
		inline int ReloadReverseGraph(const TCHAR *FileName, texture2d& GrHandle)DXLE_NOEXCEPT_OR_NOTHROW { return texture2d::ReloadReverseGraph(FileName, GrHandle); }
		//! ReloadGraph の画像反転処理追加版
		inline int ReloadReverseGraph(const tstring& FileName, texture2d& GrHandle)DXLE_NOEXCEPT_OR_NOTHROW { return texture2d::ReloadReverseGraph(FileName.c_str(), GrHandle); }


	}
	using namespace graph2d;
}

#include "texture2d/texture2d.hpp"
#endif
