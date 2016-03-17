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
#include <algorithm>
#include <vector>
#include <algorithm>
#include "dxlibex/Helper.h"
#include "dxlibex/config/defines.h"
#include "dxlibex/basic_types.hpp"
#include "dxlibex/type_traits/enable_if.hpp"
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

		class texture2d;
		class screen;
		class shared_texture2d;
		//class divided_texture2d;
		//template<size_t N> class derivative_texture2d;

		namespace gr_impl
		{
			//!interface
			class texture_2d_handle_manager
			{
			public:
				virtual int get_handle()const = 0;
			protected:
				texture_2d_handle_manager()DXLE_NOEXCEPT_OR_NOTHROW {}
				texture_2d_handle_manager(const texture_2d_handle_manager&)DXLE_NOEXCEPT_OR_NOTHROW {}
				texture_2d_handle_manager& operator=(const texture_2d_handle_manager&)DXLE_NOEXCEPT_OR_NOTHROW {}
			};

			class simple_graph_handle_manager : public texture_2d_handle_manager, public dxle::impl::Unique_HandledObject_Bace<simple_graph_handle_manager>
			{
			public:
				simple_graph_handle_manager() {}
				//! コピー禁止
				simple_graph_handle_manager(const simple_graph_handle_manager& other) = delete;
				simple_graph_handle_manager& operator=(const simple_graph_handle_manager& other) = delete;
				//!所有権の譲渡
				simple_graph_handle_manager(simple_graph_handle_manager&& other) DXLE_NOEXCEPT_OR_NOTHROW : Unique_HandledObject_Bace(std::move(other)), NotUse3DFlag(std::move(other.NotUse3DFlag)) {}
				simple_graph_handle_manager& operator=(simple_graph_handle_manager&& other) DXLE_NOEXCEPT_OR_NOTHROW{ Unique_HandledObject_Bace::operator=(std::move(other)); NotUse3DFlag = (std::move(other.NotUse3DFlag)); return *this; }

				int get_handle()const override{ return Unique_HandledObject_Bace::GetHandle(); }

				//!\~japanese 画像を削除する
				//!\~english  Delete this image
				inline void delete_this(bool LogOutFlag = false) { DxLib::DeleteGraph(GetHandle(), LogOutFlag); }
			private:
				simple_graph_handle_manager(int param_handle, bool param_NotUse3DFlag)DXLE_NOEXCEPT_OR_NOTHROW : Unique_HandledObject_Bace(param_handle), NotUse3DFlag(param_NotUse3DFlag) {}
				bool NotUse3DFlag;

				friend texture2d;
				template<typename... T> static inline std::unique_ptr<texture_2d_handle_manager> get_unique(T&&... Args){
					return std::unique_ptr<texture_2d_handle_manager>(new simple_graph_handle_manager(std::forward<T>(Args)...));
				}
			};
			class screen_handle_manager : public texture_2d_handle_manager, public dxle::impl::Unique_HandledObject_Bace<screen_handle_manager>
			{
			public:
				screen_handle_manager() {}
				//! コピー禁止
				screen_handle_manager(const screen_handle_manager& other) = delete;
				screen_handle_manager& operator=(const screen_handle_manager& other) = delete;
				//!所有権の譲渡
				screen_handle_manager(screen_handle_manager&& other) DXLE_NOEXCEPT_OR_NOTHROW : Unique_HandledObject_Bace(std::move(other)), UseAlphaChannel(std::move(other.UseAlphaChannel)) {}
				screen_handle_manager& operator=(screen_handle_manager&& other) DXLE_NOEXCEPT_OR_NOTHROW{ Unique_HandledObject_Bace::operator=(std::move(other)); UseAlphaChannel = (std::move(other.UseAlphaChannel)); return *this; }

				int get_handle()const override{ return Unique_HandledObject_Bace::GetHandle(); }

				//!\~japanese 画像を削除する
				//!\~english  Delete this image
				inline void delete_this(bool LogOutFlag = false) { DxLib::DeleteGraph(GetHandle(), LogOutFlag); }
			private:
				screen_handle_manager(int param_handle, bool param_UseAlphaChannel)DXLE_NOEXCEPT_OR_NOTHROW : Unique_HandledObject_Bace(param_handle), UseAlphaChannel(param_UseAlphaChannel) {}
				bool UseAlphaChannel;

				friend screen;
				template<typename... T> static inline std::unique_ptr<texture_2d_handle_manager> get_unique(T&&... Args){
					return std::unique_ptr<texture_2d_handle_manager>(new screen_handle_manager(std::forward<T>(Args)...));
				}
			};
			//class lookup_handle_manager : public texture_2d_handle_manager
			//{
			//private:
			//	lookup_handle_manager(int& raw_handle) : handle(raw_handle){}
			//	int& handle;
			//	friend divided_texture2d;
			//	template<typename... T> static inline std::unique_ptr<texture_2d_handle_manager> get_unique(T&&... Args){
			//		return std::unique_ptr<texture_2d_handle_manager>(new lookup_handle_manager(std::forward<T>(Args)...));
			//	}
			//public:
			//	int get_handle()const override{ return handle; }
			//};

			template<typename BuffT, typename OutFunc>
			//! 画像ファイルを分割してグラフィックハンドルを作成する
			inline void LoadDivGraph_impl(OutFunc&& out_func, BuffT* HandleBuf, const TCHAR *FileName, int AllNum, const dxle::sizei& Num, const dxle::sizei& Size, bool NotUse3DFlag)
			{
				DxLib::LoadDivGraph(FileName, AllNum, Num.width, Num.height, Size.width, Size.height, HandleBuf, NotUse3DFlag);
				try{
					std::for_each(HandleBuf, HandleBuf + AllNum, [&out_func, &NotUse3DFlag](int& handle){
						texture2d temp{ handle, NotUse3DFlag };
						handle = -1;
						out_func(std::move(temp));
					});
					return;
				}
				catch (...){
					std::for_each(HandleBuf, HandleBuf + AllNum, [](int handle){
						::DxLib::DeleteGraph(handle);
					});
					throw;
				}
			}
		}

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

			//! グラフィックのサイズを得る
			inline sizei size()const DXLE_NOEXCEPT_OR_NOTHROW{ return GetGraphSize(); }

			//!画像を複製する
			virtual std::unique_ptr<texture2d> clone()const;

			virtual ~texture2d()DXLE_NOEXCEPT_OR_NOTHROW{}

		public:
			//生成用コンストラクタ

			//!\~japanese 指定サイズのグラフィックを作成する
			//!\~english  Create an image with a specific size
			//inline texture2d(int SizeX, int SizeY, bool NotUse3DFlag = false)DXLE_NOEXCEPT_OR_NOTHROW : texture2d(DxLib::MakeGraph(SizeX, SizeY, NotUse3DFlag), NotUse3DFlag){}

			//!\~japanese 画像ファイルからグラフィックを作成する
			//!\~english  Create an image form an image file
			inline texture2d(const TCHAR *FileName, int TextureFlag, int ReverseFlag, int SurfaceMode = DX_MOVIESURFACE_NORMAL)DXLE_NOEXCEPT_OR_NOTHROW : texture2d(DxLib::LoadBmpToGraph(FileName, TextureFlag, ReverseFlag, SurfaceMode), false){}


			//生成用static関数

			// グラフィック作成関係関数

			//!\~japanese 指定サイズのグラフィックを作成する
			//!\~english  Create an image with a specific size
			static inline texture2d MakeGraph(int SizeX, int SizeY, bool NotUse3DFlag = false)DXLE_NOEXCEPT_OR_NOTHROW{ return texture2d(DxLib::MakeGraph(SizeX, SizeY, NotUse3DFlag), NotUse3DFlag); }
			//!\~japanese 指定サイズのグラフィックを作成する
			//!\~english  Create an image with sizes
			static inline texture2d MakeGraph(const sizei& size, bool NotUse3DFlag = false)DXLE_NOEXCEPT_OR_NOTHROW{ return texture2d(DxLib::MakeGraph(size.width, size.height, NotUse3DFlag), NotUse3DFlag); }
			//! 指定のグラフィックの指定部分だけを抜き出して新たなグラフィックを作成する
			static inline texture2d DerivationGraph(int SrcX, int SrcY, int Width, int Height, const texture2d& SrcGraphHandle)DXLE_NOEXCEPT_OR_NOTHROW{ return texture2d(DxLib::DerivationGraph(SrcX, SrcY, Width, Height, SrcGraphHandle.GetHandle()), false); }
			//! 指定のグラフィックの指定部分だけを抜き出して新たなグラフィックを作成する
			static inline texture2d DerivationGraph(const pointi& src, const sizei& size, const texture2d& SrcGraphHandle)DXLE_NOEXCEPT_OR_NOTHROW{ return texture2d(DxLib::DerivationGraph(src.x, src.y, size.width, size.height, SrcGraphHandle.GetHandle()), false); }

			// 画像からグラフィックを作成する関数

			//!\~japanese 画像ファイルからグラフィックを作成する
			//!\~english  Create an image from an image file
			static inline texture2d LoadBmpToGraph(const TCHAR *FileName, int TextureFlag, int ReverseFlag, int SurfaceMode = DX_MOVIESURFACE_NORMAL)DXLE_NOEXCEPT_OR_NOTHROW{ return texture2d(DxLib::LoadBmpToGraph(FileName, TextureFlag, ReverseFlag, SurfaceMode), false); }
			//!\~japanese 画像ファイルからグラフィックを作成する
			//!\~english  Create an image from an image file
			static inline texture2d LoadBmpToGraph(const dxle::tstring& FileName, int TextureFlag, int ReverseFlag, int SurfaceMode = DX_MOVIESURFACE_NORMAL)DXLE_NOEXCEPT_OR_NOTHROW{ return texture2d(DxLib::LoadBmpToGraph(FileName.c_str(), TextureFlag, ReverseFlag, SurfaceMode), false); }
			//!\~japanese 画像ファイルからグラフィックを作成する
			//!\~english  Create an image from an image file
			static inline texture2d LoadGraph(const TCHAR *FileName, bool NotUse3DFlag = false)DXLE_NOEXCEPT_OR_NOTHROW{ return texture2d(DxLib::LoadGraph(FileName, NotUse3DFlag), NotUse3DFlag); }
			//!\~japanese 画像ファイルからグラフィックを作成する
			//!\~english  Create an image from an image file
			static inline texture2d LoadGraph(const dxle::tstring& FileName, bool NotUse3DFlag = false)DXLE_NOEXCEPT_OR_NOTHROW{ return texture2d(DxLib::LoadGraph(FileName.c_str(), NotUse3DFlag), NotUse3DFlag); }
			//!\~japanese 画像ファイルからグラフィックを作成する
			//!\~english  Create a reversed image from an image file
			static inline texture2d LoadReverseGraph(const TCHAR *FileName, bool NotUse3DFlag = false)DXLE_NOEXCEPT_OR_NOTHROW{ return texture2d(DxLib::LoadReverseGraph(FileName, NotUse3DFlag), NotUse3DFlag); }
			//!\~japanese 画像ファイルからグラフィックを作成する
			//!\~english  Create a reversed image from an image file
			static inline texture2d LoadReverseGraph(const dxle::tstring& FileName, bool NotUse3DFlag = false)DXLE_NOEXCEPT_OR_NOTHROW{ return texture2d(DxLib::LoadReverseGraph(FileName.c_str(), NotUse3DFlag), NotUse3DFlag); }


			//!\~japanese メモリ上の画像イメージからグラフィックを作成する
			//!\~english  Create an graphic from an image stored in the computer memory
			static inline texture2d CreateGraphFromMem(const void *RGBFileImage, int RGBFileImageSize, const void *AlphaFileImage = nullptr, int AlphaFileImageSize = 0, bool TextureFlag = true, bool ReverseFlag = false)DXLE_NOEXCEPT_OR_NOTHROW{ return texture2d(DxLib::CreateGraphFromMem(RGBFileImage, RGBFileImageSize, AlphaFileImage, AlphaFileImageSize, TextureFlag, ReverseFlag), false); }
			//!\~japanese メモリ上の画像イメージから既存のグラフィックにデータを転送する
			//!\~english  Recreate an graphic from an existing image stored in the computer memory
			static inline int ReCreateGraphFromMem(const void *RGBFileImage, int RGBFileImageSize, texture2d& GrHandle, const void *AlphaFileImage = nullptr, int AlphaFileImageSize = 0, bool TextureFlag = true, bool ReverseFlag = false)DXLE_NOEXCEPT_OR_NOTHROW{ return DxLib::ReCreateGraphFromMem(RGBFileImage, RGBFileImageSize, GrHandle.GetHandle(), AlphaFileImage, AlphaFileImageSize, TextureFlag, ReverseFlag); }

			//!\~japanese 基本イメージデータからサイズを割り出し、それに合ったグラフィックを作成する
			//!\~english  Determine the size of the basic image data and create a compatible graphics
			static inline texture2d CreateDXGraph(const DxLib::BASEIMAGE *RgbBaseImage, const DxLib::BASEIMAGE *AlphaBaseImage, bool TextureFlag)DXLE_NOEXCEPT_OR_NOTHROW{ return texture2d(DxLib::CreateDXGraph(RgbBaseImage, AlphaBaseImage, TextureFlag), false); }
			//!\~japanese 基本イメージデータからグラフィックを作成する
			//!\~english  Create a graphic from a basic image data
			static inline texture2d CreateGraphFromGraphImage(const DxLib::BASEIMAGE *RgbBaseImage, bool TextureFlag = true, bool ReverseFlag = false)DXLE_NOEXCEPT_OR_NOTHROW{ return texture2d(DxLib::CreateGraphFromGraphImage(RgbBaseImage, TextureFlag, ReverseFlag), false); }
			//!\~japanese 基本イメージデータからグラフィックを作成する
			//!\~english  Create a graphic from a basic image data
			static inline texture2d CreateGraphFromGraphImage(const DxLib::BASEIMAGE *RgbBaseImage, const DxLib::BASEIMAGE *AlphaBaseImage, bool TextureFlag = true, bool ReverseFlag = false)DXLE_NOEXCEPT_OR_NOTHROW{ return texture2d(DxLib::CreateGraphFromGraphImage(RgbBaseImage, AlphaBaseImage, TextureFlag, ReverseFlag), false); }
			//!\~japanese 基本イメージデータから既存のグラフィックにデータを転送する
			//!\~english  Recreate a graphic from an existing basic image data
			static inline int ReCreateGraphFromGraphImage(const DxLib::BASEIMAGE *RgbBaseImage, texture2d& GrHandle, bool TextureFlag = true, bool ReverseFlag = false)DXLE_NOEXCEPT_OR_NOTHROW{ return DxLib::ReCreateGraphFromGraphImage(RgbBaseImage, GrHandle.GetHandle(), TextureFlag, ReverseFlag); }
			//!\~japanese 基本イメージデータから既存のグラフィックにデータを転送する
			//!\~english  Recreate a graphic from an existing basic image data
			static inline int ReCreateGraphFromGraphImage(const DxLib::BASEIMAGE *RgbBaseImage, const DxLib::BASEIMAGE *AlphaBaseImage, texture2d& GrHandle, bool TextureFlag = true, bool ReverseFlag = false)DXLE_NOEXCEPT_OR_NOTHROW{ return DxLib::ReCreateGraphFromGraphImage(RgbBaseImage, AlphaBaseImage, GrHandle.GetHandle(), TextureFlag, ReverseFlag); }

			//! メモリ上のビットマップイメージからグラフィックを作成する
			//!\~english  Create a graphic from a bitmap image stored in the computer memory
			static inline texture2d CreateGraph(int Width, int Height, int Pitch, const void *RGBImage, const void *AlphaImage = nullptr, const texture2d& GrHandle = texture2d())DXLE_NOEXCEPT_OR_NOTHROW{ return texture2d(DxLib::CreateGraph(Width, Height, Pitch, RGBImage, AlphaImage, GrHandle.GetHandle()), false); }
			//! メモリ上のビットマップイメージからグラフィックを作成する
			//!\~english  Create a graphic from a bitmap image stored in the computer memory
			static inline texture2d CreateGraph(const sizei& size, int Pitch, const void *RGBImage, const void *AlphaImage = nullptr, const texture2d& GrHandle = texture2d())DXLE_NOEXCEPT_OR_NOTHROW{ return texture2d(DxLib::CreateGraph(size.width, size.height, Pitch, RGBImage, AlphaImage, GrHandle.GetHandle()), false); }
			//! メモリ上のビットマップイメージからグラフィックを再作成する
			//!\~english  Recreate a graphic from a bitmap image stored in the computer memory
			static inline int ReCreateGraph(int Width, int Height, int Pitch, const void *RGBImage, texture2d& GrHandle, const void *AlphaImage = nullptr)DXLE_NOEXCEPT_OR_NOTHROW{ return DxLib::ReCreateGraph(Width, Height, Pitch, RGBImage, GrHandle.GetHandle(), AlphaImage); }
			//! メモリ上のビットマップイメージからグラフィックを再作成する
			//!\~english  Recreate a graphic from a bitmap image stored in the computer memory
			static inline int ReCreateGraph(const sizei& size, int Pitch, const void *RGBImage, texture2d& GrHandle, const void *AlphaImage = nullptr)DXLE_NOEXCEPT_OR_NOTHROW{ return DxLib::ReCreateGraph(size.width, size.height, Pitch, RGBImage, GrHandle.GetHandle(), AlphaImage); }
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
			static inline texture2d CreateGraphFromBaseImage(const DxLib::BASEIMAGE *BaseImage)DXLE_NOEXCEPT_OR_NOTHROW{ return texture2d(DxLib::CreateGraphFromBaseImage(BaseImage), false); }
			//! 基本イメージデータの指定の領域を使ってグラフィックを作成する
			//!\~english  Create a graphic from a degsinated area clipped from a basic image data 
			static inline texture2d CreateGraphFromRectBaseImage(const DxLib::BASEIMAGE *BaseImage, int x, int y, int SizeX, int SizeY)DXLE_NOEXCEPT_OR_NOTHROW{ return texture2d(DxLib::CreateGraphFromRectBaseImage(BaseImage, x, y, SizeX, SizeY), false); }
			//! 基本イメージデータの指定の領域を使ってグラフィックを作成する
			static inline texture2d CreateGraphFromRectBaseImage(const DxLib::BASEIMAGE *BaseImage, const pointi& pos, const sizei& size)DXLE_NOEXCEPT_OR_NOTHROW{ return texture2d(DxLib::CreateGraphFromRectBaseImage(BaseImage, pos.x, pos.y, size.width, size.height), false); }
			//! 基本イメージデータから既存のグラフィックに画像データを転送する
			static inline int ReCreateGraphFromBaseImage(const DxLib::BASEIMAGE *BaseImage, texture2d& GrHandle)DXLE_NOEXCEPT_OR_NOTHROW{ return DxLib::ReCreateGraphFromBaseImage(BaseImage, GrHandle.GetHandle()); }
			//! 基本イメージデータの指定の領域を使って既存のグラフィックに画像データを転送する
			static inline int ReCreateGraphFromRectBaseImage(const DxLib::BASEIMAGE *BaseImage, int x, int y, int SizeX, int SizeY, texture2d& GrHandle)DXLE_NOEXCEPT_OR_NOTHROW{ return DxLib::ReCreateGraphFromRectBaseImage(BaseImage, x, y, SizeX, SizeY, GrHandle.GetHandle()); }
			//! 基本イメージデータの指定の領域を使って既存のグラフィックに画像データを転送する
			static inline int ReCreateGraphFromRectBaseImage(const DxLib::BASEIMAGE *BaseImage, const pointi& pos, const sizei& size, texture2d& GrHandle)DXLE_NOEXCEPT_OR_NOTHROW{ return DxLib::ReCreateGraphFromRectBaseImage(BaseImage, pos.x, pos.y, size.width, size.height, GrHandle.GetHandle()); }
			//! 画像ファイルからグラフィックへ画像データを転送する
			static inline int ReloadGraph(const TCHAR *FileName, texture2d& GrHandle, bool ReverseFlag = false)DXLE_NOEXCEPT_OR_NOTHROW{ return DxLib::ReloadGraph(FileName, GrHandle.GetHandle(), ReverseFlag); }
			//! 画像ファイルからグラフィックへ画像データを転送する
			static inline int ReloadGraph(const dxle::tstring& FileName, texture2d& GrHandle, bool ReverseFlag = false)DXLE_NOEXCEPT_OR_NOTHROW{ return DxLib::ReloadGraph(FileName.c_str(), GrHandle.GetHandle(), ReverseFlag); }
			//! ReloadGraph の画像反転処理追加版
			static inline int ReloadReverseGraph(const TCHAR *FileName, texture2d& GrHandle)DXLE_NOEXCEPT_OR_NOTHROW{ return DxLib::ReloadReverseGraph(FileName, GrHandle.GetHandle()); }
			//! ReloadGraph の画像反転処理追加版
			static inline int ReloadReverseGraph(const dxle::tstring& FileName, texture2d& GrHandle)DXLE_NOEXCEPT_OR_NOTHROW{ return DxLib::ReloadReverseGraph(FileName.c_str(), GrHandle.GetHandle()); }


				//メンバ関数

			inline texture2d DerivationGraph(int SrcX, int SrcY, int Width, int Height)const DXLE_NOEXCEPT_OR_NOTHROW { return texture2d(DxLib::DerivationGraph(SrcX, SrcY, Width, Height, GetHandle()), false); }
			//! 指定のグラフィックの指定部分だけを抜き出して新たなグラフィックを作成する
			inline texture2d DerivationGraph(const pointi& src, const sizei& size)const DXLE_NOEXCEPT_OR_NOTHROW { return texture2d(DxLib::DerivationGraph(src.x, src.y, size.width, size.height, GetHandle()), false); }


			////! 指定のグラフィックのＡＲＧＢ８イメージを取得する( 現在動画ファイルをグラフィックハンドルで読み込んだ場合のみ使用可能 )
			//inline const unsigned int* GetFullColorImage()const DXLE_NOEXCEPT_OR_NOTHROW { return DxLib::GetFullColorImage(GetHandle()); }

			//! グラフィックメモリ領域のロック
			inline int GraphLock(int *PitchBuf, void **DataPointBuf, DxLib::COLORDATA **ColorDataPP = nullptr, bool WriteOnly = false)DXLE_NOEXCEPT_OR_NOTHROW{ return DxLib::GraphLock(GetHandle(), PitchBuf, DataPointBuf, ColorDataPP, WriteOnly); }
			//! グラフィックメモリ領域のロック解除
			inline int GraphUnLock()DXLE_NOEXCEPT_OR_NOTHROW{ return DxLib::GraphUnLock(GetHandle()); }
			//! グラフィックスデバイスのデバイスロスト発生時に指定のグラフィックを削除するかどうかを設定する( TRUE:デバイスロスト時に削除する  FALSE:デバイスロストが発生しても削除しない )
			inline int SetDeviceLostDeleteGraphFlag(bool DeleteFlag)DXLE_NOEXCEPT_OR_NOTHROW{ return DxLib::SetDeviceLostDeleteGraphFlag(GetHandle(), DeleteFlag); }
			//! グラフィックのサイズを得る
			inline int GetGraphSize(int *SizeXBuf, int *SizeYBuf)const DXLE_NOEXCEPT_OR_NOTHROW{ return DxLib::GetGraphSize(GetHandle(), SizeXBuf, SizeYBuf); }
			//! グラフィックのサイズを得る
			inline sizei GetGraphSize()const DXLE_NOEXCEPT_OR_NOTHROW{ int x, y; GetGraphSize(&x, &y); return{ x, y }; }
			//! グラフィックが持つ一つ目のテクスチャのサイズを得る
			inline int GetGraphTextureSize(int *SizeXBuf, int *SizeYBuf)const DXLE_NOEXCEPT_OR_NOTHROW{ return DxLib::GetGraphTextureSize(GetHandle(), SizeXBuf, SizeYBuf); }
			//! グラフィックが持つ一つ目のテクスチャのサイズを得る
			inline sizei GetGraphTextureSize()const DXLE_NOEXCEPT_OR_NOTHROW{ int x, y; GetGraphTextureSize(&x, &y); return{ x, y }; }
			//! グラフィックが持つテクスチャのミップマップレベル数を取得する
			inline int GetGraphMipmapCount()const DXLE_NOEXCEPT_OR_NOTHROW{ return DxLib::GetGraphMipmapCount(GetHandle()); }
			//! グラフィックが画像ファイルから読み込まれていた場合、その画像のファイルパスを取得する
			inline int GetGraphFilePath(TCHAR FilePathBuffer[])const DXLE_NOEXCEPT_OR_NOTHROW{ return DxLib::GetGraphFilePath(GetHandle(), FilePathBuffer); }
			//! グラフィックが画像ファイルから読み込まれていた場合、その画像のファイルパスを取得する
			inline tstring GetGraphFilePath()const;

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
			inline int DrawGraphF(const pointf& p, bool TransFlag)const DXLE_NOEXCEPT_OR_NOTHROW_SINGLE{ DXLE_GET_LOCK(screen_mutex_c::mtx); return DxLib::DrawGraphF(p.x, p.y, GetHandle(), TransFlag); }
			//! 画像の拡大描画( 座標指定が float 版 )
			inline int DrawExtendGraphF(float x1f, float y1f, float x2f, float y2, bool TransFlag)const DXLE_NOEXCEPT_OR_NOTHROW_SINGLE{ DXLE_GET_LOCK(screen_mutex_c::mtx); return DxLib::DrawExtendGraphF(x1f, y1f, x2f, y2, GetHandle(), TransFlag); }
			//! 画像の拡大描画( 座標指定が float 版 )
			inline int DrawExtendGraphF(const pointf& lu, const pointf& rb, bool TransFlag)const DXLE_NOEXCEPT_OR_NOTHROW_SINGLE{ DXLE_GET_LOCK(screen_mutex_c::mtx); return DxLib::DrawExtendGraphF(lu.x, lu.y, rb.x, rb.y, GetHandle(), TransFlag); }
			//! 画像の回転描画( 座標指定が float 版 )
			inline int DrawRotaGraphF(float xf, float yf, double ExRate, double Angle, bool TransFlag, bool TurnFlag = FALSE)const DXLE_NOEXCEPT_OR_NOTHROW_SINGLE{ DXLE_GET_LOCK(screen_mutex_c::mtx); return DxLib::DrawRotaGraphF(xf, yf, ExRate, Angle, GetHandle(), TransFlag, TurnFlag); }
			//! 画像の回転描画( 座標指定が float 版 )
			inline int DrawRotaGraphF(const pointf& p, double ExRate, double Angle, bool TransFlag, bool TurnFlag = FALSE)const DXLE_NOEXCEPT_OR_NOTHROW_SINGLE{ DXLE_GET_LOCK(screen_mutex_c::mtx); return DxLib::DrawRotaGraphF(p.x, p.y, ExRate, Angle, GetHandle(), TransFlag, TurnFlag); }
			//! 画像の回転描画２( 回転中心指定付き )( 座標指定が float 版 )
			inline int DrawRotaGraph2F(float xf, float yf, float cxf, float cyf, double ExtRate, double Angle, bool TransFlag, bool TurnFlag = FALSE)const DXLE_NOEXCEPT_OR_NOTHROW_SINGLE{ DXLE_GET_LOCK(screen_mutex_c::mtx); return DxLib::DrawRotaGraph2F(xf, yf, cxf, cyf, ExtRate, Angle, GetHandle(), TransFlag, TurnFlag); }
			//! 画像の回転描画２( 回転中心指定付き )( 座標指定が float 版 )
			inline int DrawRotaGraph2F(const pointf& p, const pointf& c, double ExtRate, double Angle, bool TransFlag, bool TurnFlag = FALSE)const DXLE_NOEXCEPT_OR_NOTHROW_SINGLE{ DXLE_GET_LOCK(screen_mutex_c::mtx); return DxLib::DrawRotaGraph2F(p.x, p.y, c.x, c.y, ExtRate, Angle, GetHandle(), TransFlag, TurnFlag); }
			//! 画像の回転描画３( 回転中心指定付き＋縦横拡大率別指定版 )( 座標指定が float 版 )
			inline int DrawRotaGraph3F(float xf, float yf, float cxf, float cyf, double ExtRateX, double ExtRateY, double Angle, bool TransFlag, bool TurnFlag = FALSE)const DXLE_NOEXCEPT_OR_NOTHROW_SINGLE{ DXLE_GET_LOCK(screen_mutex_c::mtx); return DxLib::DrawRotaGraph3F(xf, yf, cxf, cyf, ExtRateX, ExtRateY, Angle, GetHandle(), TransFlag, TurnFlag); }
			//! 画像の回転描画３( 回転中心指定付き＋縦横拡大率別指定版 )( 座標指定が float 版 )
			inline int DrawRotaGraph3F(const pointf& p, const pointf& c, double ExtRateX, double ExtRateY, double Angle, bool TransFlag, bool TurnFlag = FALSE)const DXLE_NOEXCEPT_OR_NOTHROW_SINGLE{ DXLE_GET_LOCK(screen_mutex_c::mtx); return DxLib::DrawRotaGraph3F(p.x, p.y, c.x, c.y, ExtRateX, ExtRateY, Angle, GetHandle(), TransFlag, TurnFlag); }
			//! 画像の自由変形描画( 座標指定が float 版 )
			inline int DrawModiGraphF(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4, bool TransFlag)const DXLE_NOEXCEPT_OR_NOTHROW_SINGLE{ DXLE_GET_LOCK(screen_mutex_c::mtx); return DxLib::DrawModiGraphF(x1, y1, x2, y2, x3, y3, x4, y4, GetHandle(), TransFlag); }
			//! 画像の自由変形描画( 座標指定が float 版 )
			inline int DrawModiGraphF(const std::array<pointf, 4>& ps, bool TransFlag)const DXLE_NOEXCEPT_OR_NOTHROW_SINGLE{ DXLE_GET_LOCK(screen_mutex_c::mtx); return DxLib::DrawModiGraphF(ps[0].x, ps[0].y, ps[1].x, ps[1].y, ps[2].x, ps[2].y, ps[3].x, ps[3].y, GetHandle(), TransFlag); }
			//! 画像の左右反転描画( 座標指定が float 版 )
			inline int DrawTurnGraphF(float xf, float yf, bool TransFlag)const DXLE_NOEXCEPT_OR_NOTHROW_SINGLE{ DXLE_GET_LOCK(screen_mutex_c::mtx); return DxLib::DrawTurnGraphF(xf, yf, GetHandle(), TransFlag); }
			//! 画像の左右反転描画( 座標指定が float 版 )
			inline int DrawTurnGraphF(const pointf& p, bool TransFlag)const DXLE_NOEXCEPT_OR_NOTHROW_SINGLE{ DXLE_GET_LOCK(screen_mutex_c::mtx); return DxLib::DrawTurnGraphF(p.x, p.y, GetHandle(), TransFlag); }
			//! 画像の拡大左右反転描画( 座標指定が float 版 )
			inline int DrawExtendTurnGraphF(float x1f, float y1f, float x2f, float y2f, bool TransFlag)const DXLE_NOEXCEPT_OR_NOTHROW_SINGLE{ DXLE_GET_LOCK(screen_mutex_c::mtx); return DxLib::DrawExtendGraphF(x2f, y1f, x1f, y2f, GetHandle(), TransFlag); }//x1fとx2fはこれで正しい
			//! 画像の拡大左右反転描画( 座標指定が float 版 )
			inline int DrawExtendTurnGraphF(const pointf& lu, const pointf& rb, bool TransFlag)const DXLE_NOEXCEPT_OR_NOTHROW_SINGLE{ DXLE_GET_LOCK(screen_mutex_c::mtx); return DxLib::DrawExtendGraphF(rb.x, lu.y, lu.x, rb.y, GetHandle(), TransFlag); }//x1fとx2fはこれで正しい

			int filter_mono			(int16_t Cb, int16_t Cr)																				DXLE_NOEXCEPT_OR_NOTHROW;
			int filter_gaussian		(uint16_t PixelWidth, int Param)																		DXLE_NOEXCEPT_OR_NOTHROW;
			int filter_down_scale	(uint8_t DivNum)																						DXLE_NOEXCEPT_OR_NOTHROW;
			int filter_bright_clip	(bright_clip_mode clipmode, uint8_t clip_pram, unsigned int fillcolor, uint8_t fillalpha)				DXLE_NOEXCEPT_OR_NOTHROW;
			int filter_HSB			(bool HueType, int16_t Hue, int Saturation, int16_t Bright)												DXLE_NOEXCEPT_OR_NOTHROW;
			int filter_invert		()																										DXLE_NOEXCEPT_OR_NOTHROW;
			int filter_level		(uint8_t min, uint8_t max, int Gamma, uint8_t Aftermin, uint8_t Aftermax)								DXLE_NOEXCEPT_OR_NOTHROW;
			int filter_two_color	(uint8_t threshold, unsigned int LowColor, uint8_t LowAlpha, unsigned int HighColor, uint8_t HighAlpha)	DXLE_NOEXCEPT_OR_NOTHROW;


		private:
			texture2d(int handle, bool NotUse3DFlag)
				: texture2d(gr_impl::simple_graph_handle_manager::get_unique(handle, NotUse3DFlag))
			{}
			texture2d(std::unique_ptr<gr_impl::texture_2d_handle_manager>&& handle_manager_)
				: handle_manager(std::move(handle_manager_))
			{}
			std::unique_ptr<gr_impl::texture_2d_handle_manager> handle_manager;

			int GetHandle()const{ if (!handle_manager){ return -1; } return handle_manager->get_handle(); }

			//ユーザーが継承するのを防止するためprotectedではなくfriendを使う

			friend screen;
			//friend divided_texture2d;

			//実装用

			 template<typename BuffT, typename OutFunc>
			 friend void gr_impl::LoadDivGraph_impl(OutFunc&& out_func, BuffT* HandleBuf, const TCHAR *FileName, int AllNum, const dxle::sizei& Num, const dxle::sizei& Size, bool NotUse3DFlag);
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
#else
			texture2d* operator->()DXLE_NOEXCEPT_OR_NOTHROW{ return *impl; }
			const texture2d* operator->()const DXLE_NOEXCEPT_OR_NOTHROW{ return *impl; }
			texture2d& operator*()DXLE_NOEXCEPT_OR_NOTHROW{ return *impl; }
			const texture2d& operator*()const DXLE_NOEXCEPT_OR_NOTHROW{ return *impl; }
#endif

		private:
			std::shared_ptr<texture2d> impl;
		};

		//!N = AllNum
		template<size_t N>
		//!画像ファイルを分割したグラフィック
		//!AllSizeが定数の場合
		//!コンテナの用件を満たさない
		//!moveに線形時間がかかることに注意
		class static_derivative_texture2d final
		{
		private:
			std::array<texture2d, N> textures;
			using cont_type = decltype(textures);
		public:
		//----------生成関数----------//

			static_derivative_texture2d(const TCHAR *FileName, const dxle::sizei& Num, const dxle::sizei& Size, bool NotUse3DFlag = false);
			static_derivative_texture2d(const std::string& FileName, const dxle::sizei& Num, const dxle::sizei& Size, bool NotUse3DFlag = false);

			void load(const TCHAR *FileName, const dxle::sizei& Num, const dxle::sizei& Size, bool NotUse3DFlag = false);
			void load(const std::string& FileName, const dxle::sizei& Num, const dxle::sizei& Size, bool NotUse3DFlag = false);

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

			DXLE_CONSTEXPR static_derivative_texture2d()DXLE_NOEXCEPT_OR_NOTHROW{}
			static_derivative_texture2d(const static_derivative_texture2d&) = delete;
			static_derivative_texture2d(static_derivative_texture2d&&)DXLE_NOEXCEPT_OR_NOTHROW;
			static_derivative_texture2d& operator=(const static_derivative_texture2d&) = delete;
			static_derivative_texture2d& operator=(static_derivative_texture2d&&)DXLE_NOEXCEPT_OR_NOTHROW;

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
		//!コンテナの用件を満たさない
		//!moveは定数時間
		class derivative_texture2d final
		{
		private:
			std::vector<texture2d> textures;
			using cont_type = decltype(textures);
		public:
			//----------生成関数----------//

			derivative_texture2d(const TCHAR *FileName, int AllNum, const dxle::sizei& Num, const dxle::sizei& Size, bool NotUse3DFlag = false);
			derivative_texture2d(const std::string& FileName, int AllNum, const dxle::sizei& Num, const dxle::sizei& Size, bool NotUse3DFlag = false);

			void load(const TCHAR *FileName, int AllNum, const dxle::sizei& Num, const dxle::sizei& Size, bool NotUse3DFlag = false);
			void load(const std::string& FileName, int AllNum, const dxle::sizei& Num, const dxle::sizei& Size, bool NotUse3DFlag = false);

			static inline derivative_texture2d LoadDivGraph(const TCHAR *FileName, int AllNum, const dxle::sizei& Num, const dxle::sizei& Size, bool NotUse3DFlag = false);
			static inline derivative_texture2d LoadDivGraph(const std::string& FileName, int AllNum, const dxle::sizei& Num, const dxle::sizei& Size, bool NotUse3DFlag = false);

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

			derivative_texture2d()DXLE_NOEXCEPT_OR_NOTHROW{}
			derivative_texture2d(const derivative_texture2d&) = delete;
			derivative_texture2d(derivative_texture2d&&)DXLE_NOEXCEPT_IF((std::is_nothrow_move_constructible<cont_type>::value));
			derivative_texture2d& operator=(const derivative_texture2d&) = delete;
			derivative_texture2d& operator=(derivative_texture2d&&)DXLE_NOEXCEPT_IF((std::is_nothrow_move_assignable<cont_type>::value));

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
		inline derivative_texture2d LoadDivGraph(const std::string& FileName, int AllNum, const dxle::sizei& Num, const dxle::sizei& Size, bool NotUse3DFlag = false);

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



	//--------------------実装--------------------//

		//! グラフィックが画像ファイルから読み込まれていた場合、その画像のファイルパスを取得する
		inline tstring texture2d::GetGraphFilePath()const {
			tstring buf;
			buf.resize(2048);
			if (this->GetGraphFilePath(&buf[0]) == -1) {
				return{};
			}
			buf.resize(dxle::tstrlen(buf.c_str()));
			buf.shrink_to_fit();
			return buf;
		}
		int texture2d::filter_mono(int16_t Cb, int16_t Cr)													DXLE_NOEXCEPT_OR_NOTHROW
		{
			return DxLib::GraphFilter(GetHandle(), DX_GRAPH_FILTER_MONO, Cb, Cr);
		}
		int texture2d::filter_gaussian(uint16_t PixelWidth, int Param)											DXLE_NOEXCEPT_OR_NOTHROW
		{
			return DxLib::GraphFilter(GetHandle(), DX_GRAPH_FILTER_GAUSS, PixelWidth, Param);
		}
		int texture2d::filter_down_scale(uint8_t DivNum)															DXLE_NOEXCEPT_OR_NOTHROW
		{
			return DxLib::GraphFilter(GetHandle(), DX_GRAPH_FILTER_DOWN_SCALE, DivNum);
		}
		int texture2d::filter_bright_clip(bright_clip_mode clipmode, uint8_t clip_pram, unsigned int fillcolor, uint8_t fillalpha)DXLE_NOEXCEPT_OR_NOTHROW
		{
			return DxLib::GraphFilter(GetHandle(), DX_GRAPH_FILTER_BRIGHT_CLIP, clipmode, clip_pram, fillcolor, fillalpha);
		}
		int texture2d::filter_HSB(bool HueType, int16_t Hue, int Saturation, int16_t Bright)					DXLE_NOEXCEPT_OR_NOTHROW
		{
			return DxLib::GraphFilter(GetHandle(), DX_GRAPH_FILTER_HSB, HueType, Hue, Saturation, Bright);
		}
		int texture2d::filter_invert()																			DXLE_NOEXCEPT_OR_NOTHROW
		{
			return DxLib::GraphFilter(GetHandle(), DX_GRAPH_FILTER_INVERT);
		}
		int texture2d::filter_level(uint8_t min, uint8_t max, int Gamma, uint8_t Aftermin, uint8_t Aftermax)	DXLE_NOEXCEPT_OR_NOTHROW
		{
			return DxLib::GraphFilter(GetHandle(), DX_GRAPH_FILTER_LEVEL, min, max, Gamma, Aftermin, Aftermax);
		}
		int texture2d::filter_two_color(uint8_t threshold, unsigned int LowColor, uint8_t LowAlpha, unsigned int HighColor, uint8_t HighAlpha)	DXLE_NOEXCEPT_OR_NOTHROW
		{
			return DxLib::GraphFilter(GetHandle(), DX_GRAPH_FILTER_LEVEL, threshold, LowColor, LowAlpha, HighColor, HighAlpha);
		}


		template<typename Func_T>
		screen& screen::draw_on_this(Func_T&& draw_func) {
			DXLE_GET_LOCK(screen_mutex_c::mtx);
			struct Finary_ {
				int old_draw_screen;
				int old_is_setting_reset;
				Finary_()
					: old_draw_screen(DxLib::GetDrawScreen())
					, old_is_setting_reset(DxLib::GetUseSetDrawScreenSettingReset())
				{}
				~Finary_()
				{
					DxLib::SetDrawScreen(old_draw_screen);
					DxLib::SetUseSetDrawScreenSettingReset(old_is_setting_reset);
				}
			}finally_;

			DxLib::SetUseSetDrawScreenSettingReset(FALSE);
			this->SetDrawScreen();
			draw_func();

			return *this;
		}

		inline std::unique_ptr<texture2d> texture2d::clone()const
		{
			return std::make_unique<texture2d>(this->DerivationGraph({ 0,0 }, size()));
		}
		inline std::unique_ptr<screen> screen::cloneSc()const
		{
			auto graph_size = GetGraphSize();

			screen new_screen = screen::MakeScreen(graph_size, static_cast<gr_impl::screen_handle_manager&>(*handle_manager).UseAlphaChannel);
			this->BltDrawValidGraph({ 0, 0 }, graph_size, { 0, 0 }, new_screen);
			return std::make_unique<screen>(std::move(new_screen));
		}

		template<size_t N>
		inline static_derivative_texture2d<N>::static_derivative_texture2d(const TCHAR *FileName, const dxle::sizei& Num, const dxle::sizei& Size, bool NotUse3DFlag)
		{
			load(FileName, Num, Size, NotUse3DFlag);
		}
		template<size_t N>
		inline static_derivative_texture2d<N>::static_derivative_texture2d(const std::string& FileName, const dxle::sizei& Num, const dxle::sizei& Size, bool NotUse3DFlag)
		{
			load(FileName, Num, Size, NotUse3DFlag);
		}
		template<size_t N>
		inline void static_derivative_texture2d<N>::load(const TCHAR *FileName, const dxle::sizei& Num, const dxle::sizei& Size, bool NotUse3DFlag = false)
		{
			int HandleBuf[N];
			auto iter = textures.begin();
			gr_impl::LoadDivGraph_impl([&iter](texture2d&& new_obj){ *iter++ = std::move(new_obj); }, HandleBuf, FileName, N, Num, Size, NotUse3DFlag);
			return;
		}
		template<size_t N>
		inline void static_derivative_texture2d<N>::load(const std::string& FileName, const dxle::sizei& Num, const dxle::sizei& Size, bool NotUse3DFlag = false)
		{
			load(FileName.c_str(), Num, Size, NotUse3DFlag);
		}
		template<size_t N>
		inline static_derivative_texture2d<N>::static_derivative_texture2d(static_derivative_texture2d&& other)DXLE_NOEXCEPT_OR_NOTHROW
			//: textures(std::move(other.textures))//VS2013はmoveコンストラクタの暗黙の定義に対応していない
		{
			std::move(std::begin(other.textures), std::end(other.textures), std::begin(textures));
		}
		template<size_t N>
		inline static_derivative_texture2d<N>& static_derivative_texture2d<N>::operator=(static_derivative_texture2d&& other)DXLE_NOEXCEPT_OR_NOTHROW
		{
			//textures = std::move(other.textures);//VS2013はmoveコンストラクタの暗黙の定義に対応していない
			std::move(std::begin(other.textures), std::end(other.textures), std::begin(textures));
		}
		template<size_t N>
		inline void static_derivative_texture2d<N>::delete_all()
		{
			for (auto& i : textures)
			{
				i.delete_this();
			}
		}

		inline derivative_texture2d::derivative_texture2d(const TCHAR *FileName, int AllNum, const dxle::sizei& Num, const dxle::sizei& Size, bool NotUse3DFlag)
		{
			load(FileName, AllNum, Num, Size, NotUse3DFlag);
		}
		inline derivative_texture2d::derivative_texture2d(const std::string& FileName, int AllNum, const dxle::sizei& Num, const dxle::sizei& Size, bool NotUse3DFlag)
		{
			load(FileName, AllNum, Num, Size, NotUse3DFlag);
		}
		inline void derivative_texture2d::load(const TCHAR *FileName, int AllNum, const dxle::sizei& Num, const dxle::sizei& Size, bool NotUse3DFlag)
		{
			auto HandleBuf = std::make_unique<int[]>(AllNum);
			gr_impl::LoadDivGraph_impl([this](texture2d&& new_obj){ this->textures.emplace_back(std::move(new_obj)); }, HandleBuf.get(), FileName, AllNum, Num, Size, NotUse3DFlag);
			return;
		}
		inline void derivative_texture2d::load(const std::string& FileName, int AllNum, const dxle::sizei& Num, const dxle::sizei& Size, bool NotUse3DFlag)
		{
			load(FileName.c_str(), AllNum, Num, Size, NotUse3DFlag);
		}
		inline derivative_texture2d::derivative_texture2d(derivative_texture2d&& other)DXLE_NOEXCEPT_IF((std::is_nothrow_move_constructible<cont_type>::value))
			: textures(std::move(other.textures))
		{}
		inline derivative_texture2d& derivative_texture2d::operator=(derivative_texture2d&& other)DXLE_NOEXCEPT_IF((std::is_nothrow_move_assignable<cont_type>::value))
		{
			textures = std::move(other.textures);
		}
		inline void derivative_texture2d::delete_all()
		{
			textures.clear();
		}

		inline derivative_texture2d LoadDivGraph(const TCHAR *FileName, int AllNum, const dxle::sizei& Num, const dxle::sizei& Size, bool NotUse3DFlag)
		{
			return derivative_texture2d(FileName, AllNum, Num, Size, NotUse3DFlag);
		}
		inline derivative_texture2d LoadDivGraph(const std::string& FileName, int AllNum, const dxle::sizei& Num, const dxle::sizei& Size, bool NotUse3DFlag)
		{
			return derivative_texture2d(FileName, AllNum, Num, Size, NotUse3DFlag);
		}
		inline derivative_texture2d derivative_texture2d::LoadDivGraph(const TCHAR *FileName, int AllNum, const dxle::sizei& Num, const dxle::sizei& Size, bool NotUse3DFlag)
		{
			return derivative_texture2d(FileName, AllNum, Num, Size, NotUse3DFlag);
		}
		inline derivative_texture2d derivative_texture2d::LoadDivGraph(const std::string& FileName, int AllNum, const dxle::sizei& Num, const dxle::sizei& Size, bool NotUse3DFlag)
		{
			return derivative_texture2d(FileName, AllNum, Num, Size, NotUse3DFlag);
		}
























		namespace old_ver
		{

			class screen;

			//!\~japanese 画像クラス(画像ハンドルクラスではない)
			//!\~english  A image class (NOT an image handle class)
			class texture_2d /*final*/ : public impl::Unique_HandledObject_Bace<texture_2d>
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
				inline void delete_this(bool LogOutFlag = false) { DxLib::DeleteGraph(GetHandle(), LogOutFlag); }

				//! グラフィックのサイズを得る
				inline sizei size()const DXLE_NOEXCEPT_OR_NOTHROW{ return GetGraphSize(); }


					virtual ~texture_2d()DXLE_NOEXCEPT_OR_NOTHROW{}

			public:
				//生成用コンストラクタ

				//!\~japanese 指定サイズのグラフィックを作成する
				//!\~english  Create an image with a specific size
				//inline texture_2d(int SizeX, int SizeY, bool NotUse3DFlag = false)DXLE_NOEXCEPT_OR_NOTHROW : texture_2d(DxLib::MakeGraph(SizeX, SizeY, NotUse3DFlag), NotUse3DFlag){}

				//!\~japanese 画像ファイルからグラフィックを作成する
				//!\~english  Create an image form an image file
				inline texture_2d(const TCHAR *FileName, int TextureFlag, int ReverseFlag, int SurfaceMode = DX_MOVIESURFACE_NORMAL)DXLE_NOEXCEPT_OR_NOTHROW : texture_2d(DxLib::LoadBmpToGraph(FileName, TextureFlag, ReverseFlag, SurfaceMode), false){}


				//生成用static関数

				// グラフィック作成関係関数

				//!\~japanese 指定サイズのグラフィックを作成する
				//!\~english  Create an image with a specific size
				static inline texture_2d MakeGraph(int SizeX, int SizeY, bool NotUse3DFlag = false)DXLE_NOEXCEPT_OR_NOTHROW{ return texture_2d(DxLib::MakeGraph(SizeX, SizeY, NotUse3DFlag), NotUse3DFlag); }
					//!\~japanese 指定サイズのグラフィックを作成する
					//!\~english  Create an image with sizes
				static inline texture_2d MakeGraph(const sizei& size, bool NotUse3DFlag = false)DXLE_NOEXCEPT_OR_NOTHROW{ return texture_2d(DxLib::MakeGraph(size.width, size.height, NotUse3DFlag), NotUse3DFlag); }
					//! 指定のグラフィックの指定部分だけを抜き出して新たなグラフィックを作成する
				static inline texture_2d DerivationGraph(int SrcX, int SrcY, int Width, int Height, int SrcGraphHandle)DXLE_NOEXCEPT_OR_NOTHROW{ return texture_2d(DxLib::DerivationGraph(SrcX, SrcY, Width, Height, SrcGraphHandle), false); }
					//! 指定のグラフィックの指定部分だけを抜き出して新たなグラフィックを作成する
				static inline texture_2d DerivationGraph(const pointi& src, const sizei& size, int SrcGraphHandle)DXLE_NOEXCEPT_OR_NOTHROW{ return texture_2d(DxLib::DerivationGraph(src.x, src.y, size.width, size.height, SrcGraphHandle), false); }

					// 画像からグラフィックを作成する関数

					//!\~japanese 画像ファイルからグラフィックを作成する
					//!\~english  Create an image from an image file
				static inline texture_2d LoadBmpToGraph(const TCHAR *FileName, int TextureFlag, int ReverseFlag, int SurfaceMode = DX_MOVIESURFACE_NORMAL)DXLE_NOEXCEPT_OR_NOTHROW{ return texture_2d(DxLib::LoadBmpToGraph(FileName, TextureFlag, ReverseFlag, SurfaceMode), false); }
					//!\~japanese 画像ファイルからグラフィックを作成する
					//!\~english  Create an image from an image file
				static inline texture_2d LoadBmpToGraph(const dxle::tstring& FileName, int TextureFlag, int ReverseFlag, int SurfaceMode = DX_MOVIESURFACE_NORMAL)DXLE_NOEXCEPT_OR_NOTHROW{ return texture_2d(DxLib::LoadBmpToGraph(FileName.c_str(), TextureFlag, ReverseFlag, SurfaceMode), false); }
					//!\~japanese 画像ファイルからグラフィックを作成する
					//!\~english  Create an image from an image file
				static inline texture_2d LoadGraph(const TCHAR *FileName, bool NotUse3DFlag = false)DXLE_NOEXCEPT_OR_NOTHROW{ return texture_2d(DxLib::LoadGraph(FileName, NotUse3DFlag), NotUse3DFlag); }
					//!\~japanese 画像ファイルからグラフィックを作成する
					//!\~english  Create an image from an image file
				static inline texture_2d LoadGraph(const dxle::tstring& FileName, bool NotUse3DFlag = false)DXLE_NOEXCEPT_OR_NOTHROW{ return texture_2d(DxLib::LoadGraph(FileName.c_str(), NotUse3DFlag), NotUse3DFlag); }
					//!\~japanese 画像ファイルからグラフィックを作成する
					//!\~english  Create a reversed image from an image file
				static inline texture_2d LoadReverseGraph(const TCHAR *FileName, bool NotUse3DFlag = false)DXLE_NOEXCEPT_OR_NOTHROW{ return texture_2d(DxLib::LoadReverseGraph(FileName, NotUse3DFlag), NotUse3DFlag); }
					//!\~japanese 画像ファイルからグラフィックを作成する
					//!\~english  Create a reversed image from an image file
				static inline texture_2d LoadReverseGraph(const dxle::tstring& FileName, bool NotUse3DFlag = false)DXLE_NOEXCEPT_OR_NOTHROW{ return texture_2d(DxLib::LoadReverseGraph(FileName.c_str(), NotUse3DFlag), NotUse3DFlag); }

					//LoadDivGraph

					template<typename Cont = std::vector<texture_2d>, enable_if_t<std::is_same<typename Cont::value_type, texture_2d>::value/* && dxle::ignore_type<decltype(std::declval<Cont>().emplace_back())>::value*/, nullptr_t> = nullptr>
				//! 画像ファイルを分割してグラフィックハンドルを作成する(DxLib互換)
				static inline Cont LoadDivGraph(const TCHAR *FileName, int AllNum, int XNum, int YNum, int XSize, int YSize, bool NotUse3DFlag = FALSE){ return texture_2d::LoadDivGraph<Cont>(FileName, AllNum, { XNum, YNum }, { XSize, YSize }, NotUse3DFlag); }

				template<typename Cont = std::vector<texture_2d>, enable_if_t<std::is_same<typename Cont::value_type, texture_2d>::value/* && dxle::ignore_type<decltype(std::declval<Cont>().emplace_back())>::value*/, nullptr_t> = nullptr>
				//! 画像ファイルを分割してグラフィックハンドルを作成する(char[], dxle::sizei指定)
				static inline Cont LoadDivGraph(const TCHAR *FileName, int AllNum, const dxle::sizei& Num, const dxle::sizei& Size, bool NotUse3DFlag = FALSE);

				template<typename Cont = std::vector<texture_2d>, enable_if_t<std::is_same<typename Cont::value_type, texture_2d>::value/* && dxle::ignore_type<decltype(std::declval<Cont>().emplace_back())>::value*/, nullptr_t> = nullptr>
				//! 画像ファイルを分割してグラフィックハンドルを作成する(string, dxle::sizei指定)
				static inline Cont LoadDivGraph(const dxle::tstring& FileName, int AllNum, const dxle::sizei& Num, const dxle::sizei& Size, bool NotUse3DFlag = FALSE){ return texture_2d::LoadDivGraph<Cont>(FileName.c_str(), AllNum, Num, Size, NotUse3DFlag); }

				template<size_t AllNum, typename Cont = std::vector<texture_2d>, enable_if_t<std::is_same<typename Cont::value_type, texture_2d>::value/* && dxle::ignore_type<decltype(std::declval<Cont>().emplace_back())>::value*/, nullptr_t> = nullptr>
				//! 画像ファイルを分割してグラフィックハンドルを作成する(char[], dxle::sizei指定, AllNumがテンプレート引数)
				static inline Cont LoadDivGraph(const TCHAR *FileName, const dxle::sizei& Num, const dxle::sizei& Size, bool NotUse3DFlag = FALSE);
				template<size_t AllNum, typename Cont = std::vector<texture_2d>, enable_if_t<std::is_same<typename Cont::value_type, texture_2d>::value/* && dxle::ignore_type<decltype(std::declval<Cont>().emplace_back())>::value*/, nullptr_t> = nullptr>
				//! 画像ファイルを分割してグラフィックハンドルを作成する(string, dxle::sizei指定, AllNumがテンプレート引数)
				static inline Cont LoadDivGraph(const dxle::tstring& FileName, const dxle::sizei& Num, const dxle::sizei& Size, bool NotUse3DFlag = FALSE){ return texture_2d::LoadDivGraph<AllNum, Cont>(FileName.c_str(), Num, Size, NotUse3DFlag); }

				template<size_t AllNum>
				//! 画像ファイルを分割してグラフィックハンドルを作成する(char[], dxle::sizei指定, 出力先指定(std::array))
				static inline void LoadDivGraph(std::array<texture_2d, AllNum>& out, const TCHAR *FileName, const dxle::sizei& Num, const dxle::sizei& Size, bool NotUse3DFlag = FALSE);
				template<size_t AllNum>
				//! 画像ファイルを分割してグラフィックハンドルを作成する(char[], dxle::sizei指定, 出力先指定(texture_2d[]))
				static inline void LoadDivGraph(texture_2d(&out)[AllNum], const TCHAR *FileName, const dxle::sizei& Num, const dxle::sizei& Size, bool NotUse3DFlag = FALSE);
				template<size_t AllNum>
				//! 画像ファイルを分割してグラフィックハンドルを作成する(string, dxle::sizei指定, 出力先指定(std::array))
				static inline void LoadDivGraph(std::array<texture_2d, AllNum>& out, const dxle::tstring& FileName, const dxle::sizei& Num, const dxle::sizei& Size, bool NotUse3DFlag = FALSE){ return texture_2d::LoadDivGraph(out, FileName.c_str(), Num, Size, NotUse3DFlag); }
				template<size_t AllNum>
				//! 画像ファイルを分割してグラフィックハンドルを作成する(string, dxle::sizei指定, 出力先指定(texture_2d[]))
				static inline void LoadDivGraph(texture_2d(&out)[AllNum], const dxle::tstring& FileName, const dxle::sizei& Num, const dxle::sizei& Size, bool NotUse3DFlag = FALSE){ return texture_2d::LoadDivGraph(out, FileName.c_str(), Num, Size, NotUse3DFlag); }



				//!\~japanese メモリ上の画像イメージからグラフィックを作成する
				//!\~english  Create an graphic from an image stored in the computer memory
				static inline texture_2d CreateGraphFromMem(const void *RGBFileImage, int RGBFileImageSize, const void *AlphaFileImage = nullptr, int AlphaFileImageSize = 0, bool TextureFlag = true, bool ReverseFlag = false)DXLE_NOEXCEPT_OR_NOTHROW{ return texture_2d(DxLib::CreateGraphFromMem(RGBFileImage, RGBFileImageSize, AlphaFileImage, AlphaFileImageSize, TextureFlag, ReverseFlag), false); }
				//!\~japanese メモリ上の画像イメージから既存のグラフィックにデータを転送する
				//!\~english  Recreate an graphic from an existing image stored in the computer memory
				static inline int ReCreateGraphFromMem(const void *RGBFileImage, int RGBFileImageSize, texture_2d& GrHandle, const void *AlphaFileImage = nullptr, int AlphaFileImageSize = 0, bool TextureFlag = true, bool ReverseFlag = false)DXLE_NOEXCEPT_OR_NOTHROW{ return DxLib::ReCreateGraphFromMem(RGBFileImage, RGBFileImageSize, GrHandle.GetHandle(), AlphaFileImage, AlphaFileImageSize, TextureFlag, ReverseFlag); }

				//!\~japanese 基本イメージデータからサイズを割り出し、それに合ったグラフィックを作成する
				//!\~english  Determine the size of the basic image data and create a compatible graphics
				static inline texture_2d CreateDXGraph(const DxLib::BASEIMAGE *RgbBaseImage, const DxLib::BASEIMAGE *AlphaBaseImage, bool TextureFlag)DXLE_NOEXCEPT_OR_NOTHROW{ return texture_2d(DxLib::CreateDXGraph(RgbBaseImage, AlphaBaseImage, TextureFlag), false); }
				//!\~japanese 基本イメージデータからグラフィックを作成する
				//!\~english  Create a graphic from a basic image data
				static inline texture_2d CreateGraphFromGraphImage(const DxLib::BASEIMAGE *RgbBaseImage, bool TextureFlag = true, bool ReverseFlag = false)DXLE_NOEXCEPT_OR_NOTHROW{ return texture_2d(DxLib::CreateGraphFromGraphImage(RgbBaseImage, TextureFlag, ReverseFlag), false); }
				//!\~japanese 基本イメージデータからグラフィックを作成する
				//!\~english  Create a graphic from a basic image data
				static inline texture_2d CreateGraphFromGraphImage(const DxLib::BASEIMAGE *RgbBaseImage, const DxLib::BASEIMAGE *AlphaBaseImage, bool TextureFlag = true, bool ReverseFlag = false)DXLE_NOEXCEPT_OR_NOTHROW{ return texture_2d(DxLib::CreateGraphFromGraphImage(RgbBaseImage, AlphaBaseImage, TextureFlag, ReverseFlag), false); }
				//!\~japanese 基本イメージデータから既存のグラフィックにデータを転送する
				//!\~english  Recreate a graphic from an existing basic image data
				static inline int ReCreateGraphFromGraphImage(const DxLib::BASEIMAGE *RgbBaseImage, texture_2d& GrHandle, bool TextureFlag = true, bool ReverseFlag = false)DXLE_NOEXCEPT_OR_NOTHROW{ return DxLib::ReCreateGraphFromGraphImage(RgbBaseImage, GrHandle.GetHandle(), TextureFlag, ReverseFlag); }
				//!\~japanese 基本イメージデータから既存のグラフィックにデータを転送する
				//!\~english  Recreate a graphic from an existing basic image data
				static inline int ReCreateGraphFromGraphImage(const DxLib::BASEIMAGE *RgbBaseImage, const DxLib::BASEIMAGE *AlphaBaseImage, texture_2d& GrHandle, bool TextureFlag = true, bool ReverseFlag = false)DXLE_NOEXCEPT_OR_NOTHROW{ return DxLib::ReCreateGraphFromGraphImage(RgbBaseImage, AlphaBaseImage, GrHandle.GetHandle(), TextureFlag, ReverseFlag); }

				//! メモリ上のビットマップイメージからグラフィックを作成する
				//!\~english  Create a graphic from a bitmap image stored in the computer memory
				static inline texture_2d CreateGraph(int Width, int Height, int Pitch, const void *RGBImage, const void *AlphaImage = nullptr, const texture_2d& GrHandle = texture_2d())DXLE_NOEXCEPT_OR_NOTHROW{ return texture_2d(DxLib::CreateGraph(Width, Height, Pitch, RGBImage, AlphaImage, GrHandle.GetHandle()), false); }
				//! メモリ上のビットマップイメージからグラフィックを作成する
				//!\~english  Create a graphic from a bitmap image stored in the computer memory
				static inline texture_2d CreateGraph(const sizei& size, int Pitch, const void *RGBImage, const void *AlphaImage = nullptr, const texture_2d& GrHandle = texture_2d())DXLE_NOEXCEPT_OR_NOTHROW{ return texture_2d(DxLib::CreateGraph(size.width, size.height, Pitch, RGBImage, AlphaImage, GrHandle.GetHandle()), false); }
				//! メモリ上のビットマップイメージからグラフィックを再作成する
				//!\~english  Recreate a graphic from a bitmap image stored in the computer memory
				static inline int ReCreateGraph(int Width, int Height, int Pitch, const void *RGBImage, texture_2d& GrHandle, const void *AlphaImage = nullptr)DXLE_NOEXCEPT_OR_NOTHROW{ return DxLib::ReCreateGraph(Width, Height, Pitch, RGBImage, GrHandle.GetHandle(), AlphaImage); }
				//! メモリ上のビットマップイメージからグラフィックを再作成する
				//!\~english  Recreate a graphic from a bitmap image stored in the computer memory
				static inline int ReCreateGraph(const sizei& size, int Pitch, const void *RGBImage, texture_2d& GrHandle, const void *AlphaImage = nullptr)DXLE_NOEXCEPT_OR_NOTHROW{ return DxLib::ReCreateGraph(size.width, size.height, Pitch, RGBImage, GrHandle.GetHandle(), AlphaImage); }
#ifndef DX_NON_SOFTIMAGE
					//extern	int			CreateBlendGraphFromSoftImage(int SIHandle);																													// ソフトウエアで扱うイメージからブレンド用画像グラフィックハンドルを作成する( -1:エラー  -1以外:ブレンド用グラフィックハンドル )
					//extern	int			CreateGraphFromSoftImage(int SIHandle);																													// ソフトウエアで扱うイメージからグラフィックハンドルを作成する( -1:エラー  -1以外:グラフィックハンドル )
					//extern	int			CreateGraphFromRectSoftImage(int SIHandle, int x, int y, int SizeX, int SizeY);																				// ソフトウエアで扱うイメージの指定の領域を使ってグラフィックハンドルを作成する( -1:エラー  -1以外:グラフィックハンドル )
					//extern	int			ReCreateGraphFromSoftImage(int SIHandle, int GrHandle);																										// ソフトウエアで扱うイメージから既存のグラフィックハンドルに画像データを転送する
					//extern	int			ReCreateGraphFromRectSoftImage(int SIHandle, int x, int y, int SizeX, int SizeY, int GrHandle);																	// ソフトウエアで扱うイメージから既存のグラフィックハンドルに画像データを転送する
					//extern	int			CreateDivGraphFromSoftImage(int SIHandle, int AllNum, int XNum, int YNum, int SizeX, int SizeY, int *HandleBuf);												// ソフトウエアで扱うイメージから分割グラフィックハンドルを作成する
#endif // DX_NON_SOFTIMAGE
					//! 基本イメージデータからグラフィックを作成する
					//!\~english  Create a graphic from a basic image data
				static inline texture_2d CreateGraphFromBaseImage(const DxLib::BASEIMAGE *BaseImage)DXLE_NOEXCEPT_OR_NOTHROW{ return texture_2d(DxLib::CreateGraphFromBaseImage(BaseImage), false); }
					//! 基本イメージデータの指定の領域を使ってグラフィックを作成する
					//!\~english  Create a graphic from a degsinated area clipped from a basic image data 
				static inline texture_2d CreateGraphFromRectBaseImage(const DxLib::BASEIMAGE *BaseImage, int x, int y, int SizeX, int SizeY)DXLE_NOEXCEPT_OR_NOTHROW{ return texture_2d(DxLib::CreateGraphFromRectBaseImage(BaseImage, x, y, SizeX, SizeY), false); }
					//! 基本イメージデータの指定の領域を使ってグラフィックを作成する
				static inline texture_2d CreateGraphFromRectBaseImage(const DxLib::BASEIMAGE *BaseImage, const pointi& pos, const sizei& size)DXLE_NOEXCEPT_OR_NOTHROW{ return texture_2d(DxLib::CreateGraphFromRectBaseImage(BaseImage, pos.x, pos.y, size.width, size.height), false); }
					//! 基本イメージデータから既存のグラフィックに画像データを転送する
				static inline int ReCreateGraphFromBaseImage(const DxLib::BASEIMAGE *BaseImage, texture_2d& GrHandle)DXLE_NOEXCEPT_OR_NOTHROW{ return DxLib::ReCreateGraphFromBaseImage(BaseImage, GrHandle.GetHandle()); }
					//! 基本イメージデータの指定の領域を使って既存のグラフィックに画像データを転送する
				static inline int ReCreateGraphFromRectBaseImage(const DxLib::BASEIMAGE *BaseImage, int x, int y, int SizeX, int SizeY, texture_2d& GrHandle)DXLE_NOEXCEPT_OR_NOTHROW{ return DxLib::ReCreateGraphFromRectBaseImage(BaseImage, x, y, SizeX, SizeY, GrHandle.GetHandle()); }
					//! 基本イメージデータの指定の領域を使って既存のグラフィックに画像データを転送する
				static inline int ReCreateGraphFromRectBaseImage(const DxLib::BASEIMAGE *BaseImage, const pointi& pos, const sizei& size, texture_2d& GrHandle)DXLE_NOEXCEPT_OR_NOTHROW{ return DxLib::ReCreateGraphFromRectBaseImage(BaseImage, pos.x, pos.y, size.width, size.height, GrHandle.GetHandle()); }
					//! 画像ファイルからグラフィックへ画像データを転送する
				static inline int ReloadGraph(const TCHAR *FileName, texture_2d& GrHandle, bool ReverseFlag = false)DXLE_NOEXCEPT_OR_NOTHROW{ return DxLib::ReloadGraph(FileName, GrHandle.GetHandle(), ReverseFlag); }
					//! 画像ファイルからグラフィックへ画像データを転送する
				static inline int ReloadGraph(const dxle::tstring& FileName, texture_2d& GrHandle, bool ReverseFlag = false)DXLE_NOEXCEPT_OR_NOTHROW{ return DxLib::ReloadGraph(FileName.c_str(), GrHandle.GetHandle(), ReverseFlag); }
					//! ReloadGraph の画像反転処理追加版
				static inline int ReloadReverseGraph(const TCHAR *FileName, texture_2d& GrHandle)DXLE_NOEXCEPT_OR_NOTHROW{ return DxLib::ReloadReverseGraph(FileName, GrHandle.GetHandle()); }
					//! ReloadGraph の画像反転処理追加版
				static inline int ReloadReverseGraph(const dxle::tstring& FileName, texture_2d& GrHandle)DXLE_NOEXCEPT_OR_NOTHROW{ return DxLib::ReloadReverseGraph(FileName.c_str(), GrHandle.GetHandle()); }


					//メンバ関数

					////! 指定のグラフィックのＡＲＧＢ８イメージを取得する( 現在動画ファイルをグラフィックハンドルで読み込んだ場合のみ使用可能 )
					//inline const unsigned int* GetFullColorImage()const DXLE_NOEXCEPT_OR_NOTHROW { return DxLib::GetFullColorImage(GetHandle()); }

					//! グラフィックメモリ領域のロック
				inline int GraphLock(int *PitchBuf, void **DataPointBuf, DxLib::COLORDATA **ColorDataPP = nullptr, bool WriteOnly = false)DXLE_NOEXCEPT_OR_NOTHROW{ return DxLib::GraphLock(GetHandle(), PitchBuf, DataPointBuf, ColorDataPP, WriteOnly); }
					//! グラフィックメモリ領域のロック解除
				inline int GraphUnLock()DXLE_NOEXCEPT_OR_NOTHROW{ return DxLib::GraphUnLock(GetHandle()); }
					//! グラフィックスデバイスのデバイスロスト発生時に指定のグラフィックを削除するかどうかを設定する( TRUE:デバイスロスト時に削除する  FALSE:デバイスロストが発生しても削除しない )
				inline int SetDeviceLostDeleteGraphFlag(bool DeleteFlag)DXLE_NOEXCEPT_OR_NOTHROW{ return DxLib::SetDeviceLostDeleteGraphFlag(GetHandle(), DeleteFlag); }
					//! グラフィックのサイズを得る
				inline int GetGraphSize(int *SizeXBuf, int *SizeYBuf)const DXLE_NOEXCEPT_OR_NOTHROW{ return DxLib::GetGraphSize(GetHandle(), SizeXBuf, SizeYBuf); }
					//! グラフィックのサイズを得る
				inline sizei GetGraphSize()const DXLE_NOEXCEPT_OR_NOTHROW{ int x, y; GetGraphSize(&x, &y); return{ x, y }; }
					//! グラフィックが持つ一つ目のテクスチャのサイズを得る
				inline int GetGraphTextureSize(int *SizeXBuf, int *SizeYBuf)const DXLE_NOEXCEPT_OR_NOTHROW{ return DxLib::GetGraphTextureSize(GetHandle(), SizeXBuf, SizeYBuf); }
					//! グラフィックが持つ一つ目のテクスチャのサイズを得る
				inline sizei GetGraphTextureSize()const DXLE_NOEXCEPT_OR_NOTHROW{ int x, y; GetGraphTextureSize(&x, &y); return{ x, y }; }
					//! グラフィックが持つテクスチャのミップマップレベル数を取得する
				inline int GetGraphMipmapCount()const DXLE_NOEXCEPT_OR_NOTHROW{ return DxLib::GetGraphMipmapCount(GetHandle()); }
					//! グラフィックが画像ファイルから読み込まれていた場合、その画像のファイルパスを取得する
				inline int GetGraphFilePath(TCHAR FilePathBuffer[])const DXLE_NOEXCEPT_OR_NOTHROW{ return DxLib::GetGraphFilePath(GetHandle(), FilePathBuffer); }
					//! グラフィックが画像ファイルから読み込まれていた場合、その画像のファイルパスを取得する
					inline tstring GetGraphFilePath()const {
					tstring buf;
					buf.resize(2048);
					if (this->GetGraphFilePath(&buf[0]) == -1) {
						return{};
					}
					buf.resize(dxle::tstrlen(buf.c_str()));
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
				inline int DrawGraphF(const pointf& p, bool TransFlag)const DXLE_NOEXCEPT_OR_NOTHROW_SINGLE{ DXLE_GET_LOCK(screen_mutex_c::mtx); return DxLib::DrawGraphF(p.x, p.y, GetHandle(), TransFlag); }
					//! 画像の拡大描画( 座標指定が float 版 )
				inline int DrawExtendGraphF(float x1f, float y1f, float x2f, float y2, bool TransFlag)const DXLE_NOEXCEPT_OR_NOTHROW_SINGLE{ DXLE_GET_LOCK(screen_mutex_c::mtx); return DxLib::DrawExtendGraphF(x1f, y1f, x2f, y2, GetHandle(), TransFlag); }
					//! 画像の拡大描画( 座標指定が float 版 )
				inline int DrawExtendGraphF(const pointf& lu, const pointf& rb, bool TransFlag)const DXLE_NOEXCEPT_OR_NOTHROW_SINGLE{ DXLE_GET_LOCK(screen_mutex_c::mtx); return DxLib::DrawExtendGraphF(lu.x, lu.y, rb.x, rb.y, GetHandle(), TransFlag); }
					//! 画像の回転描画( 座標指定が float 版 )
				inline int DrawRotaGraphF(float xf, float yf, double ExRate, double Angle, bool TransFlag, bool TurnFlag = FALSE)const DXLE_NOEXCEPT_OR_NOTHROW_SINGLE{ DXLE_GET_LOCK(screen_mutex_c::mtx); return DxLib::DrawRotaGraphF(xf, yf, ExRate, Angle, GetHandle(), TransFlag, TurnFlag); }
					//! 画像の回転描画( 座標指定が float 版 )
				inline int DrawRotaGraphF(const pointf& p, double ExRate, double Angle, bool TransFlag, bool TurnFlag = FALSE)const DXLE_NOEXCEPT_OR_NOTHROW_SINGLE{ DXLE_GET_LOCK(screen_mutex_c::mtx); return DxLib::DrawRotaGraphF(p.x, p.y, ExRate, Angle, GetHandle(), TransFlag, TurnFlag); }
					//! 画像の回転描画２( 回転中心指定付き )( 座標指定が float 版 )
				inline int DrawRotaGraph2F(float xf, float yf, float cxf, float cyf, double ExtRate, double Angle, bool TransFlag, bool TurnFlag = FALSE)const DXLE_NOEXCEPT_OR_NOTHROW_SINGLE{ DXLE_GET_LOCK(screen_mutex_c::mtx); return DxLib::DrawRotaGraph2F(xf, yf, cxf, cyf, ExtRate, Angle, GetHandle(), TransFlag, TurnFlag); }
					//! 画像の回転描画２( 回転中心指定付き )( 座標指定が float 版 )
				inline int DrawRotaGraph2F(const pointf& p, const pointf& c, double ExtRate, double Angle, bool TransFlag, bool TurnFlag = FALSE)const DXLE_NOEXCEPT_OR_NOTHROW_SINGLE{ DXLE_GET_LOCK(screen_mutex_c::mtx); return DxLib::DrawRotaGraph2F(p.x, p.y, c.x, c.y, ExtRate, Angle, GetHandle(), TransFlag, TurnFlag); }
					//! 画像の回転描画３( 回転中心指定付き＋縦横拡大率別指定版 )( 座標指定が float 版 )
				inline int DrawRotaGraph3F(float xf, float yf, float cxf, float cyf, double ExtRateX, double ExtRateY, double Angle, bool TransFlag, bool TurnFlag = FALSE)const DXLE_NOEXCEPT_OR_NOTHROW_SINGLE{ DXLE_GET_LOCK(screen_mutex_c::mtx); return DxLib::DrawRotaGraph3F(xf, yf, cxf, cyf, ExtRateX, ExtRateY, Angle, GetHandle(), TransFlag, TurnFlag); }
					//! 画像の回転描画３( 回転中心指定付き＋縦横拡大率別指定版 )( 座標指定が float 版 )
				inline int DrawRotaGraph3F(const pointf& p, const pointf& c, double ExtRateX, double ExtRateY, double Angle, bool TransFlag, bool TurnFlag = FALSE)const DXLE_NOEXCEPT_OR_NOTHROW_SINGLE{ DXLE_GET_LOCK(screen_mutex_c::mtx); return DxLib::DrawRotaGraph3F(p.x, p.y, c.x, c.y, ExtRateX, ExtRateY, Angle, GetHandle(), TransFlag, TurnFlag); }
					//! 画像の自由変形描画( 座標指定が float 版 )
				inline int DrawModiGraphF(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4, bool TransFlag)const DXLE_NOEXCEPT_OR_NOTHROW_SINGLE{ DXLE_GET_LOCK(screen_mutex_c::mtx); return DxLib::DrawModiGraphF(x1, y1, x2, y2, x3, y3, x4, y4, GetHandle(), TransFlag); }
					//! 画像の自由変形描画( 座標指定が float 版 )
				inline int DrawModiGraphF(const std::array<pointf, 4>& ps, bool TransFlag)const DXLE_NOEXCEPT_OR_NOTHROW_SINGLE{ DXLE_GET_LOCK(screen_mutex_c::mtx); return DxLib::DrawModiGraphF(ps[0].x, ps[0].y, ps[1].x, ps[1].y, ps[2].x, ps[2].y, ps[3].x, ps[3].y, GetHandle(), TransFlag); }
					//! 画像の左右反転描画( 座標指定が float 版 )
				inline int DrawTurnGraphF(float xf, float yf, bool TransFlag)const DXLE_NOEXCEPT_OR_NOTHROW_SINGLE{ DXLE_GET_LOCK(screen_mutex_c::mtx); return DxLib::DrawTurnGraphF(xf, yf, GetHandle(), TransFlag); }
					//! 画像の左右反転描画( 座標指定が float 版 )
				inline int DrawTurnGraphF(const pointf& p, bool TransFlag)const DXLE_NOEXCEPT_OR_NOTHROW_SINGLE{ DXLE_GET_LOCK(screen_mutex_c::mtx); return DxLib::DrawTurnGraphF(p.x, p.y, GetHandle(), TransFlag); }
					//! 画像の拡大左右反転描画( 座標指定が float 版 )
				inline int DrawExtendTurnGraphF(float x1f, float y1f, float x2f, float y2f, bool TransFlag)const DXLE_NOEXCEPT_OR_NOTHROW_SINGLE{ DXLE_GET_LOCK(screen_mutex_c::mtx); return DxLib::DrawExtendGraphF(x2f, y1f, x1f, y2f, GetHandle(), TransFlag); }//x1fとx2fはこれで正しい
					//! 画像の拡大左右反転描画( 座標指定が float 版 )
				inline int DrawExtendTurnGraphF(const pointf& lu, const pointf& rb, bool TransFlag)const DXLE_NOEXCEPT_OR_NOTHROW_SINGLE{ DXLE_GET_LOCK(screen_mutex_c::mtx); return DxLib::DrawExtendGraphF(rb.x, lu.y, lu.x, rb.y, GetHandle(), TransFlag); }//x1fとx2fはこれで正しい

				int filter_mono(int16_t Cb, int16_t Cr)DXLE_NOEXCEPT_OR_NOTHROW{ return DxLib::GraphFilter(GetHandle(), DX_GRAPH_FILTER_MONO, Cb, Cr); }
				int filter_gaussian(uint16_t PixelWidth, int Param)DXLE_NOEXCEPT_OR_NOTHROW{ return DxLib::GraphFilter(GetHandle(), DX_GRAPH_FILTER_GAUSS, PixelWidth, Param); }
				int filter_down_scale(uint8_t DivNum)DXLE_NOEXCEPT_OR_NOTHROW{ return DxLib::GraphFilter(GetHandle(), DX_GRAPH_FILTER_DOWN_SCALE, DivNum); }
					int filter_bright_clip(bright_clip_mode clipmode, uint8_t clip_pram, unsigned int fillcolor, uint8_t fillalpha)DXLE_NOEXCEPT_OR_NOTHROW{
					return DxLib::GraphFilter(GetHandle(), DX_GRAPH_FILTER_BRIGHT_CLIP, clipmode, clip_pram, fillcolor, fillalpha);
				}
					int filter_HSB(bool HueType, int16_t Hue, int Saturation, int16_t Bright)DXLE_NOEXCEPT_OR_NOTHROW{
					return DxLib::GraphFilter(GetHandle(), DX_GRAPH_FILTER_HSB, HueType, Hue, Saturation, Bright);
				}
				int filter_invert()DXLE_NOEXCEPT_OR_NOTHROW{ return DxLib::GraphFilter(GetHandle(), DX_GRAPH_FILTER_INVERT); }
					int filter_level(uint8_t min, uint8_t max, int Gamma, uint8_t Aftermin, uint8_t Aftermax)DXLE_NOEXCEPT_OR_NOTHROW{
					return DxLib::GraphFilter(GetHandle(), DX_GRAPH_FILTER_LEVEL, min, max, Gamma, Aftermin, Aftermax);
				}
					int filter_two_color(uint8_t threshold, unsigned int LowColor, uint8_t LowAlpha, unsigned int HighColor, uint8_t HighAlpha)DXLE_NOEXCEPT_OR_NOTHROW{
					return DxLib::GraphFilter(GetHandle(), DX_GRAPH_FILTER_LEVEL, threshold, LowColor, LowAlpha, HighColor, HighAlpha);
				}

			private:
				texture_2d(int param_handle, bool param_NotUse3DFlag)DXLE_NOEXCEPT_OR_NOTHROW : Unique_HandledObject_Bace(param_handle), NotUse3DFlag(param_NotUse3DFlag){}
				bool NotUse3DFlag;

				//screen等の子クラスからGetHandleにアクセスできるようにする為
				static int GetTexture2DHandle(const texture_2d& texture2d_obj) { return texture2d_obj.GetHandle(); }

				//画像ファイルを分割してグラフィックハンドルを作成する
				template<typename BuffT, typename OutFunc>
				static void LoadDivGraph_impl(OutFunc&& out_func, BuffT* HandleBuf, const TCHAR *FileName, int AllNum, const dxle::sizei& Num, const dxle::sizei& Size, bool NotUse3DFlag);

				friend screen;
			};
			//! 描画可能画像クラス
			class screen final : public texture_2d
			{
			public:
				//!画像を複製する
				screen cloneSc()const;
				//!画像を複製する
				texture_2d cloneGr()const override;

				//生成用static関数

				//! SetDrawScreen で描画対象にできるグラフィックを作成する
				static inline screen MakeScreen(int SizeX, int SizeY, bool UseAlphaChannel = false)DXLE_NOEXCEPT_OR_NOTHROW{ return screen(DxLib::MakeScreen(SizeX, SizeY, UseAlphaChannel), UseAlphaChannel); }
					//! SetDrawScreen で描画対象にできるグラフィックを作成する
				static inline screen MakeScreen(const sizei& size, bool UseAlphaChannel = false)DXLE_NOEXCEPT_OR_NOTHROW{ return screen(DxLib::MakeScreen(size.width, size.height, UseAlphaChannel), UseAlphaChannel); }

					//メンバ関数

					template<typename Func_T>
				inline screen& drawn_on(Func_T&& draw_func) {
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
					return *this;
				}

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
				inline int BltDrawValidGraph(int x1, int y1, int x2, int y2, int DestX, int DestY, texture_2d& DestGrHandle)const DXLE_NOEXCEPT_OR_NOTHROW{ return DxLib::BltDrawValidGraph(GetHandle(), x1, y1, x2, y2, DestX, DestY, texture_2d::GetTexture2DHandle(DestGrHandle)); }
					//
					//! SetDrawScreen で描画対象にできるグラフィックハンドルから指定領域の画像情報を別のグラフィックハンドルに転送する
					//!@param lu 転送元の矩形の左上座標
					//!@param rb 転送元の矩形の右下座標
					//!@param dest 転送先の矩形の左上座標
					//!@param DestGrHandle 転送先の画像
				inline int BltDrawValidGraph(const pointi& lu, const pointi& rb, const pointi& dest, texture_2d& DestGrHandle)const DXLE_NOEXCEPT_OR_NOTHROW{ return DxLib::BltDrawValidGraph(GetHandle(), rb.x, lu.y, lu.x, rb.y, dest.x, dest.y, texture_2d::GetTexture2DHandle(DestGrHandle)); }
					//! 描画先画面を設定する
				inline int SetDrawScreen()const DXLE_NOEXCEPT_OR_NOTHROW_SINGLE{ DXLE_GET_LOCK(screen_mutex_c::mtx); return DxLib::SetDrawScreen(GetHandle()); }

			public:
				screen() : texture_2d() {}
				//! コピー禁止
				screen(const screen& other) = delete;
				//!所有権の譲渡
				screen(screen&& other) DXLE_NOEXCEPT_OR_NOTHROW : texture_2d(std::move(other)), UseAlphaChannel(std::move(other.UseAlphaChannel)) {}
				//! コピー禁止
				screen& operator=(const screen& other) = delete;
				//! 所有権の譲渡
				screen& operator=(screen&& other) DXLE_NOEXCEPT_OR_NOTHROW{ texture_2d::operator=(std::move(other)); UseAlphaChannel = (std::move(other.UseAlphaChannel)); return *this; }

					virtual ~screen()DXLE_NOEXCEPT_OR_NOTHROW{}
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
				auto graph_size = GetGraphSize();

				screen new_screen = screen::MakeScreen(graph_size, UseAlphaChannel);
				this->BltDrawValidGraph({ 0, 0 }, graph_size, { 0, 0 }, new_screen);
				return new_screen;
			}
			inline texture_2d screen::cloneGr()const
			{
				auto graph_size = GetGraphSize();

				texture_2d new_graph = texture_2d::MakeGraph(graph_size, NotUse3DFlag);
				this->BltDrawValidGraph({ 0, 0 }, graph_size, { 0, 0 }, new_graph);
				return new_graph;
			}

			//生成用関数

			// グラフィック作成関係関数

			//! 指定サイズのグラフィックを作成する
			inline texture_2d MakeGraph(int SizeX, int SizeY, bool NotUse3DFlag = false)DXLE_NOEXCEPT_OR_NOTHROW{ return texture_2d::MakeGraph(SizeX, SizeY, NotUse3DFlag); }
				//! 指定サイズのグラフィックを作成する
			inline texture_2d MakeGraph(const sizei& size, bool NotUse3DFlag = false)DXLE_NOEXCEPT_OR_NOTHROW{ return texture_2d::MakeGraph(size.width, size.height, NotUse3DFlag); }
				//! SetDrawScreen で描画対象にできるグラフィックを作成する
			inline screen MakeScreen(int SizeX, int SizeY, bool UseAlphaChannel = false)DXLE_NOEXCEPT_OR_NOTHROW{ return screen::MakeScreen(SizeX, SizeY, UseAlphaChannel); }
				//! SetDrawScreen で描画対象にできるグラフィックを作成する
			inline screen MakeScreen(const sizei& size, bool UseAlphaChannel = false)DXLE_NOEXCEPT_OR_NOTHROW{ return screen::MakeScreen(size.width, size.height, UseAlphaChannel); }
				//! 指定のグラフィックの指定部分だけを抜き出して新たなグラフィックを作成する
			inline texture_2d DerivationGraph(int SrcX, int SrcY, int Width, int Height, int SrcGraphHandle)DXLE_NOEXCEPT_OR_NOTHROW{ return texture_2d::DerivationGraph(SrcX, SrcY, Width, Height, SrcGraphHandle); }
				//! 指定のグラフィックの指定部分だけを抜き出して新たなグラフィックを作成する
			inline texture_2d DerivationGraph(const pointi& src, const sizei& size, int SrcGraphHandle)DXLE_NOEXCEPT_OR_NOTHROW{ return texture_2d::DerivationGraph(src.x, src.y, size.width, size.height, SrcGraphHandle); }

				// 画像からグラフィックを作成する関数

				//! 画像ファイルからグラフィックを作成する
			inline texture_2d LoadBmpToGraph(const TCHAR *FileName, int TextureFlag, int ReverseFlag, int SurfaceMode = DX_MOVIESURFACE_NORMAL)DXLE_NOEXCEPT_OR_NOTHROW{ return texture_2d::LoadBmpToGraph(FileName, TextureFlag, ReverseFlag, SurfaceMode); }
				//! 画像ファイルからグラフィックを作成する
			inline texture_2d LoadBmpToGraph(const tstring& FileName, int TextureFlag, int ReverseFlag, int SurfaceMode = DX_MOVIESURFACE_NORMAL)DXLE_NOEXCEPT_OR_NOTHROW{ return texture_2d::LoadBmpToGraph(FileName.c_str(), TextureFlag, ReverseFlag, SurfaceMode); }
				//! 画像ファイルからグラフィックを作成する
			inline texture_2d LoadGraph(const TCHAR *FileName, bool NotUse3DFlag = false)DXLE_NOEXCEPT_OR_NOTHROW{ return texture_2d::LoadGraph(FileName, NotUse3DFlag); }
				//! 画像ファイルからグラフィックを作成する
			inline texture_2d LoadGraph(const tstring& FileName, bool NotUse3DFlag = false)DXLE_NOEXCEPT_OR_NOTHROW{ return texture_2d::LoadGraph(FileName.c_str(), NotUse3DFlag); }
				//! 画像ファイルを反転したものでグラフィックを作成する
			inline texture_2d LoadReverseGraph(const TCHAR *FileName, bool NotUse3DFlag = false)DXLE_NOEXCEPT_OR_NOTHROW{ return texture_2d::LoadReverseGraph(FileName, NotUse3DFlag); }
				//! 画像ファイルを反転したものでグラフィックを作成する
			inline texture_2d LoadReverseGraph(const tstring& FileName, bool NotUse3DFlag = false)DXLE_NOEXCEPT_OR_NOTHROW{ return texture_2d::LoadReverseGraph(FileName.c_str(), NotUse3DFlag); }

				//! メモリ上の画像イメージからグラフィックを作成する
			inline texture_2d CreateGraphFromMem(const void *RGBFileImage, int RGBFileImageSize, const void *AlphaFileImage = nullptr, int AlphaFileImageSize = 0, bool TextureFlag = true, bool ReverseFlag = false)DXLE_NOEXCEPT_OR_NOTHROW{ return texture_2d::CreateGraphFromMem(RGBFileImage, RGBFileImageSize, AlphaFileImage, AlphaFileImageSize, TextureFlag, ReverseFlag); }
				//! メモリ上の画像イメージから既存のグラフィックにデータを転送する
			inline int ReCreateGraphFromMem(const void *RGBFileImage, int RGBFileImageSize, texture_2d& GrHandle, const void *AlphaFileImage = nullptr, int AlphaFileImageSize = 0, bool TextureFlag = true, bool ReverseFlag = false)DXLE_NOEXCEPT_OR_NOTHROW{ return texture_2d::ReCreateGraphFromMem(RGBFileImage, RGBFileImageSize, GrHandle, AlphaFileImage, AlphaFileImageSize, TextureFlag, ReverseFlag); }

				//! 基本イメージデータからサイズを割り出し、それに合ったグラフィックを作成する
			inline texture_2d CreateDXGraph(const DxLib::BASEIMAGE *RgbBaseImage, const DxLib::BASEIMAGE *AlphaBaseImage, bool TextureFlag)DXLE_NOEXCEPT_OR_NOTHROW{ return texture_2d::CreateDXGraph(RgbBaseImage, AlphaBaseImage, TextureFlag); }
				//! 基本イメージデータからグラフィックを作成する
			inline texture_2d CreateGraphFromGraphImage(const DxLib::BASEIMAGE *RgbBaseImage, bool TextureFlag = true, bool ReverseFlag = false)DXLE_NOEXCEPT_OR_NOTHROW{ return texture_2d::CreateGraphFromGraphImage(RgbBaseImage, TextureFlag, ReverseFlag); }
				//! 基本イメージデータからグラフィックを作成する
			inline texture_2d CreateGraphFromGraphImage(const DxLib::BASEIMAGE *RgbBaseImage, const DxLib::BASEIMAGE *AlphaBaseImage, bool TextureFlag = true, bool ReverseFlag = false)DXLE_NOEXCEPT_OR_NOTHROW{ return texture_2d::CreateGraphFromGraphImage(RgbBaseImage, AlphaBaseImage, TextureFlag, ReverseFlag); }
				//! 基本イメージデータから既存のグラフィックにデータを転送する
			inline int ReCreateGraphFromGraphImage(const DxLib::BASEIMAGE *RgbBaseImage, texture_2d& GrHandle, bool TextureFlag = true, bool ReverseFlag = false)DXLE_NOEXCEPT_OR_NOTHROW{ return texture_2d::ReCreateGraphFromGraphImage(RgbBaseImage, GrHandle, TextureFlag, ReverseFlag); }
				//! 基本イメージデータから既存のグラフィックにデータを転送する
			inline int ReCreateGraphFromGraphImage(const DxLib::BASEIMAGE *RgbBaseImage, const DxLib::BASEIMAGE *AlphaBaseImage, texture_2d& GrHandle, bool TextureFlag = true, bool ReverseFlag = false)DXLE_NOEXCEPT_OR_NOTHROW{ return texture_2d::ReCreateGraphFromGraphImage(RgbBaseImage, AlphaBaseImage, GrHandle, TextureFlag, ReverseFlag); }

				//! メモリ上のビットマップイメージからグラフィックを作成する
			inline texture_2d CreateGraph(int Width, int Height, int Pitch, const void *RGBImage, const void *AlphaImage = nullptr, const texture_2d& GrHandle = texture_2d())DXLE_NOEXCEPT_OR_NOTHROW{ return texture_2d::CreateGraph(Width, Height, Pitch, RGBImage, AlphaImage, GrHandle); }
				//! メモリ上のビットマップイメージからグラフィックを作成する
			inline texture_2d CreateGraph(const sizei& size, int Pitch, const void *RGBImage, const void *AlphaImage = nullptr, const texture_2d& GrHandle = texture_2d())DXLE_NOEXCEPT_OR_NOTHROW{ return texture_2d::CreateGraph(size.width, size.height, Pitch, RGBImage, AlphaImage, GrHandle); }
				//! メモリ上のビットマップイメージからグラフィックを再作成する
			inline int ReCreateGraph(int Width, int Height, int Pitch, const void *RGBImage, texture_2d& GrHandle, const void *AlphaImage = nullptr)DXLE_NOEXCEPT_OR_NOTHROW{ return texture_2d::ReCreateGraph(Width, Height, Pitch, RGBImage, GrHandle, AlphaImage); }
				//! メモリ上のビットマップイメージからグラフィックを再作成する
			inline int ReCreateGraph(const sizei& size, int Pitch, const void *RGBImage, texture_2d& GrHandle, const void *AlphaImage = nullptr)DXLE_NOEXCEPT_OR_NOTHROW{ return texture_2d::ReCreateGraph(size.width, size.height, Pitch, RGBImage, GrHandle, AlphaImage); }
#ifndef DX_NON_SOFTIMAGE
#endif // DX_NON_SOFTIMAGE
				//! 基本イメージデータからグラフィックを作成する
			inline texture_2d CreateGraphFromBaseImage(const DxLib::BASEIMAGE *BaseImage)DXLE_NOEXCEPT_OR_NOTHROW{ return texture_2d::CreateGraphFromBaseImage(BaseImage); }
				//! 基本イメージデータの指定の領域を使ってグラフィックを作成する
			inline texture_2d CreateGraphFromRectBaseImage(const DxLib::BASEIMAGE *BaseImage, int x, int y, int SizeX, int SizeY)DXLE_NOEXCEPT_OR_NOTHROW{ return texture_2d::CreateGraphFromRectBaseImage(BaseImage, x, y, SizeX, SizeY); }
				//! 基本イメージデータの指定の領域を使ってグラフィックを作成する
			inline texture_2d CreateGraphFromRectBaseImage(const DxLib::BASEIMAGE *BaseImage, const pointi& p, const sizei& size)DXLE_NOEXCEPT_OR_NOTHROW{ return texture_2d::CreateGraphFromRectBaseImage(BaseImage, p.x, p.y, size.width, size.height); }
				//! 基本イメージデータから既存のグラフィックに画像データを転送する
			inline int ReCreateGraphFromBaseImage(const DxLib::BASEIMAGE *BaseImage, texture_2d& GrHandle)DXLE_NOEXCEPT_OR_NOTHROW{ return texture_2d::ReCreateGraphFromBaseImage(BaseImage, GrHandle); }
				//! 基本イメージデータの指定の領域を使って既存のグラフィックに画像データを転送する
			inline int ReCreateGraphFromRectBaseImage(const DxLib::BASEIMAGE *BaseImage, int x, int y, int SizeX, int SizeY, texture_2d& GrHandle)DXLE_NOEXCEPT_OR_NOTHROW{ return texture_2d::ReCreateGraphFromRectBaseImage(BaseImage, x, y, SizeX, SizeY, GrHandle); }
				//! 基本イメージデータの指定の領域を使って既存のグラフィックに画像データを転送する
			inline int ReCreateGraphFromRectBaseImage(const DxLib::BASEIMAGE *BaseImage, const pointi& p, const sizei& size, texture_2d& GrHandle)DXLE_NOEXCEPT_OR_NOTHROW{ return texture_2d::ReCreateGraphFromRectBaseImage(BaseImage, p.x, p.y, size.width, size.height, GrHandle); }
				//! 画像ファイルからグラフィックへ画像データを転送する
			inline int ReloadGraph(const TCHAR *FileName, texture_2d& GrHandle, bool ReverseFlag = false)DXLE_NOEXCEPT_OR_NOTHROW{ return texture_2d::ReloadGraph(FileName, GrHandle, ReverseFlag); }
				//! 画像ファイルからグラフィックへ画像データを転送する
			inline int ReloadGraph(const tstring& FileName, texture_2d& GrHandle, bool ReverseFlag = false)DXLE_NOEXCEPT_OR_NOTHROW{ return texture_2d::ReloadGraph(FileName.c_str(), GrHandle, ReverseFlag); }
				//! ReloadGraph の画像反転処理追加版
			inline int ReloadReverseGraph(const TCHAR *FileName, texture_2d& GrHandle)DXLE_NOEXCEPT_OR_NOTHROW{ return texture_2d::ReloadReverseGraph(FileName, GrHandle); }
				//! ReloadGraph の画像反転処理追加版
			inline int ReloadReverseGraph(const tstring& FileName, texture_2d& GrHandle)DXLE_NOEXCEPT_OR_NOTHROW{ return texture_2d::ReloadReverseGraph(FileName.c_str(), GrHandle); }




				//実装

			template<typename BuffT, typename OutFunc>
			//! 画像ファイルを分割してグラフィックハンドルを作成する
			inline void texture_2d::LoadDivGraph_impl(OutFunc&& out_func, BuffT* HandleBuf, const TCHAR *FileName, int AllNum, const dxle::sizei& Num, const dxle::sizei& Size, bool NotUse3DFlag)
			{
				DxLib::LoadDivGraph(FileName, AllNum, Num.width, Num.height, Size.width, Size.height, HandleBuf, NotUse3DFlag);
				try{
					std::for_each(HandleBuf, HandleBuf + AllNum, [&out_func, &NotUse3DFlag](int& handle){
						texture_2d temp{ handle, NotUse3DFlag };
						handle = -1;
						out_func(std::move(temp));
					});
					return;
				}
				catch (...){
					std::for_each(HandleBuf, HandleBuf + AllNum, [](int handle){
						DeleteGraph(handle);
					});
					throw;
				}
			}
			template<size_t AllNum, typename Cont, enable_if_t<std::is_same<typename Cont::value_type, texture_2d>::value/* && dxle::ignore_type<decltype(std::declval<Cont>().emplace_back())>::value*/, nullptr_t>>
			//! 画像ファイルを分割してグラフィックハンドルを作成する
			inline Cont texture_2d::LoadDivGraph(const TCHAR *FileName, const dxle::sizei& Num, const dxle::sizei& Size, bool NotUse3DFlag)
			{
				int HandleBuf[AllNum];
				typename std::remove_all_extents<Cont>::type cont;
				LoadDivGraph_impl([&cont](texture_2d&& new_obj){ cont.emplace_back(std::move(new_obj)); }, HandleBuf, FileName, AllNum, Num, Size, NotUse3DFlag);
				return cont;
			}
			template<typename Cont, enable_if_t<std::is_same<typename Cont::value_type, texture_2d>::value/* && dxle::ignore_type<decltype(std::declval<Cont>().emplace_back())>::value*/, nullptr_t>>
			//! 画像ファイルを分割してグラフィックハンドルを作成する
			inline Cont texture_2d::LoadDivGraph(const TCHAR *FileName, int AllNum, const dxle::sizei& Num, const dxle::sizei& Size, bool NotUse3DFlag)
			{
				auto HandleBuf = std::make_unique<int[]>(AllNum);
				typename std::remove_all_extents<Cont>::type cont;
				LoadDivGraph_impl([&cont](texture_2d&& new_obj){ cont.emplace_back(std::move(new_obj)); }, HandleBuf.get(), FileName, AllNum, Num, Size, NotUse3DFlag);
				return cont;
			}
			template<size_t AllNum>
			//! 画像ファイルを分割してグラフィックハンドルを作成する
			inline void texture_2d::LoadDivGraph(std::array<texture_2d, AllNum>& out, const TCHAR *FileName, const dxle::sizei& Num, const dxle::sizei& Size, bool NotUse3DFlag)
			{
				int HandleBuf[AllNum];
				int i = 0;
				LoadDivGraph_impl([&out, &i](texture_2d&& new_obj){ out[i++] = std::move(new_obj); }, HandleBuf, FileName, AllNum, Num, Size, NotUse3DFlag);
				return;
			}
			template<size_t AllNum>
			//! 画像ファイルを分割してグラフィックハンドルを作成する
			inline void texture_2d::LoadDivGraph(texture_2d(&out)[AllNum], const TCHAR *FileName, const dxle::sizei& Num, const dxle::sizei& Size, bool NotUse3DFlag)
			{
				int HandleBuf[AllNum];
				int i = 0;
				LoadDivGraph_impl([&out, &i](texture_2d&& new_obj){ out[i++] = std::move(new_obj); }, HandleBuf, FileName, AllNum, Num, Size, NotUse3DFlag);
				return;
			}
		}
	}
	using namespace graph2d;
}

#endif
