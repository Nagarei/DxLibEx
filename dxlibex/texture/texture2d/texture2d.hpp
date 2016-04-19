/*=============================================================================
  Copyright (C) 2015-2016 DxLibEx project
  https://github.com/Nagarei/DxLibEx/

  Distributed under the Boost Software License, Version 1.0.
  (See http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#ifndef DXLE_INC_TEXTURE_TEXTURE2D_TEXTURE2D_HPP_
#define DXLE_INC_TEXTURE_TEXTURE2D_TEXTURE2D_HPP_

#include "dxlibex/config/no_min_max.h"
#include "DxLib.h"
#include <cstdint>
#include "../texture2d.h"
#include "dxlibex/basic_types.hpp"

//--------------------実装--------------------//

namespace dxle
{
	//! 2Dグラフィック
	namespace graph2d
	{

		namespace gr_impl
		{
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
			inline int texture2d_handle_manager::GetTextureRawHandle(const texture2d& texture)
			{
				return texture.GetHandle();
			}
		}


	//--------------------texture2d--------------------//

		inline std::unique_ptr<texture2d> texture2d::clone()const
		{
			return std::make_unique<texture2d>(this->DerivationGraph({ 0, 0 }, size()));
		}

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
		inline int texture2d::filter_mono(int16_t Cb, int16_t Cr)
		{
			return DxLib::GraphFilter(GetHandle(), DX_GRAPH_FILTER_MONO, Cb, Cr);
		}
		inline int texture2d::filter_gaussian(uint16_t PixelWidth, int Param)
		{
			return DxLib::GraphFilter(GetHandle(), DX_GRAPH_FILTER_GAUSS, PixelWidth, Param);
		}
		inline int texture2d::filter_down_scale(uint8_t DivNum)
		{
			return DxLib::GraphFilter(GetHandle(), DX_GRAPH_FILTER_DOWN_SCALE, DivNum);
		}
		inline int texture2d::filter_bright_clip(bright_clip_mode clipmode, uint8_t clip_pram, unsigned int fillcolor, uint8_t fillalpha)
		{
			return DxLib::GraphFilter(GetHandle(), DX_GRAPH_FILTER_BRIGHT_CLIP, clipmode, clip_pram, fillcolor, fillalpha);
		}
		inline int texture2d::filter_HSB(bool HueType, int16_t Hue, int Saturation, int16_t Bright)
		{
			return DxLib::GraphFilter(GetHandle(), DX_GRAPH_FILTER_HSB, HueType, Hue, Saturation, Bright);
		}
		inline int texture2d::filter_invert()
		{
			return DxLib::GraphFilter(GetHandle(), DX_GRAPH_FILTER_INVERT);
		}
		inline int texture2d::filter_level(uint8_t min, uint8_t max, int Gamma, uint8_t Aftermin, uint8_t Aftermax)
		{
			return DxLib::GraphFilter(GetHandle(), DX_GRAPH_FILTER_LEVEL, min, max, Gamma, Aftermin, Aftermax);
		}
		inline int texture2d::filter_two_color(uint8_t threshold, unsigned int LowColor, uint8_t LowAlpha, unsigned int HighColor, uint8_t HighAlpha)
		{
			return DxLib::GraphFilter(GetHandle(), DX_GRAPH_FILTER_LEVEL, threshold, LowColor, LowAlpha, HighColor, HighAlpha);
		}


	//--------------------screen--------------------//

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
		inline std::unique_ptr<screen> screen::cloneSc()const
		{
			auto graph_size = GetGraphSize();

			screen new_screen = screen::MakeScreen(graph_size, static_cast<gr_impl::screen_handle_manager&>(*handle_manager).UseAlphaChannel);
			this->BltDrawValidGraph({ 0, 0 }, graph_size, { 0, 0 }, new_screen);
			return std::make_unique<screen>(std::move(new_screen));
		}


	//--------------------static_derivative_texture2d--------------------//

		template<size_t N>
		inline static_derivative_texture2d<N>::static_derivative_texture2d(static_derivative_texture2d&& other)DXLE_NOEXCEPT_OR_NOTHROW
			//: textures(std::move(other.textures))//VS2013はmoveコンストラクタの暗黙の定義に対応していない
		{
			std::move(std::begin(other.textures), std::end(other.textures), std::begin(textures));
		}
			template<size_t N>
		inline static_derivative_texture2d<N>& static_derivative_texture2d<N>::operator=(static_derivative_texture2d&& other)DXLE_NOEXCEPT_OR_NOTHROW
		{
			//textures = std::move(other.textures);//VS2013はmove代入演算子の暗黙の定義に対応していない
			std::move(std::begin(other.textures), std::end(other.textures), std::begin(textures));
		}
		template<size_t N>
		inline static_derivative_texture2d<N>::static_derivative_texture2d(const TCHAR *FileName, const dxle::sizei& Num, const dxle::sizei& Size, bool NotUse3DFlag)
		{
			load(FileName, Num, Size, NotUse3DFlag);
		}
		template<size_t N>
		inline static_derivative_texture2d<N>::static_derivative_texture2d(const tstring& FileName, const dxle::sizei& Num, const dxle::sizei& Size, bool NotUse3DFlag)
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
		inline void static_derivative_texture2d<N>::load(const tstring& FileName, const dxle::sizei& Num, const dxle::sizei& Size, bool NotUse3DFlag = false)
		{
			load(FileName.c_str(), Num, Size, NotUse3DFlag);
		}
		template<size_t N>
		inline void static_derivative_texture2d<N>::delete_all()
		{
			for (auto& i : textures)
			{
				i.delete_this();
			}
		}


	//--------------------derivative_texture2d--------------------//
	
		inline derivative_texture2d::derivative_texture2d(derivative_texture2d&& other)DXLE_NOEXCEPT_IF((std::is_nothrow_move_constructible<cont_type>::value))
			: textures(std::move(other.textures))
		{}
		inline derivative_texture2d& derivative_texture2d::operator=(derivative_texture2d&& other)DXLE_NOEXCEPT_IF((std::is_nothrow_move_assignable<cont_type>::value))
		{
			textures = std::move(other.textures);
		}
		inline derivative_texture2d::derivative_texture2d(const TCHAR *FileName, int AllNum, const dxle::sizei& Num, const dxle::sizei& Size, bool NotUse3DFlag)
		{
			load(FileName, AllNum, Num, Size, NotUse3DFlag);
		}
		inline derivative_texture2d::derivative_texture2d(const tstring& FileName, int AllNum, const dxle::sizei& Num, const dxle::sizei& Size, bool NotUse3DFlag)
		{
			load(FileName, AllNum, Num, Size, NotUse3DFlag);
		}
		inline void derivative_texture2d::load(const TCHAR *FileName, int AllNum, const dxle::sizei& Num, const dxle::sizei& Size, bool NotUse3DFlag)
		{
			auto HandleBuf = std::make_unique<int[]>(AllNum);
			gr_impl::LoadDivGraph_impl([this](texture2d&& new_obj){ this->textures.emplace_back(std::move(new_obj)); }, HandleBuf.get(), FileName, AllNum, Num, Size, NotUse3DFlag);
			return;
		}
		inline void derivative_texture2d::load(const tstring& FileName, int AllNum, const dxle::sizei& Num, const dxle::sizei& Size, bool NotUse3DFlag)
		{
			load(FileName.c_str(), AllNum, Num, Size, NotUse3DFlag);
		}
		inline derivative_texture2d derivative_texture2d::LoadDivGraph(const TCHAR *FileName, int AllNum, const dxle::sizei& Num, const dxle::sizei& Size, bool NotUse3DFlag)
		{
			return derivative_texture2d(FileName, AllNum, Num, Size, NotUse3DFlag);
		}
		inline derivative_texture2d derivative_texture2d::LoadDivGraph(const tstring& FileName, int AllNum, const dxle::sizei& Num, const dxle::sizei& Size, bool NotUse3DFlag)
		{
			return derivative_texture2d(FileName, AllNum, Num, Size, NotUse3DFlag);
		}
		inline void derivative_texture2d::delete_all()
		{
			textures.clear();
		}


	//--------------------生成用関数--------------------//

		inline derivative_texture2d LoadDivGraph(const TCHAR *FileName, int AllNum, const dxle::sizei& Num, const dxle::sizei& Size, bool NotUse3DFlag)
		{
			return derivative_texture2d(FileName, AllNum, Num, Size, NotUse3DFlag);
		}
		inline derivative_texture2d LoadDivGraph(const tstring& FileName, int AllNum, const dxle::sizei& Num, const dxle::sizei& Size, bool NotUse3DFlag)
		{
			return derivative_texture2d(FileName, AllNum, Num, Size, NotUse3DFlag);
		}

	}
	using namespace graph2d;
}
#endif
