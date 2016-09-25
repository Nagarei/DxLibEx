/*=============================================================================
  Copyright (C) 2015-2016 DxLibEx project
  https://github.com/Nagarei/DxLibEx/

  Distributed under the Boost Software License, Version 1.0.
  (See http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#ifndef DXLE_INC_TEXTURE_TEXTURE2D_HANDLE_MANAGER_HPP_
#define DXLE_INC_TEXTURE_TEXTURE2D_HANDLE_MANAGER_HPP_

#include "dxlibex/config/no_min_max.h"
#include <memory>
#include "dxlibex/config/defines.h"
#include "prototype2d.hpp"
#include "dxlibex/helper.hpp"

//----------2Dグラフィック----------//

namespace dxle
{
//! 2Dグラフィック
namespace graph2d
{
	namespace gr_impl
	{
		//!interface
		class texture2d_handle_manager
		{
		public:
			virtual int get_handle()const = 0;
			virtual ~texture2d_handle_manager()DXLE_NOEXCEPT_OR_NOTHROW{}
		protected:
			texture2d_handle_manager()DXLE_NOEXCEPT_OR_NOTHROW{}
			texture2d_handle_manager(const texture2d_handle_manager&)DXLE_NOEXCEPT_OR_NOTHROW{}
			texture2d_handle_manager& operator=(const texture2d_handle_manager&)DXLE_NOEXCEPT_OR_NOTHROW { return *this; }

			static int GetTextureRawHandle(const texture2d&);
		};

		class simple_graph_handle_manager : public texture2d_handle_manager, public dxle::impl::Unique_HandledObject_Bace<simple_graph_handle_manager>
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
			template<typename... T> static inline std::unique_ptr<texture2d_handle_manager> get_unique(T&&... Args){
				return std::unique_ptr<texture2d_handle_manager>(new simple_graph_handle_manager(std::forward<T>(Args)...));
			}
		};
		class screen_handle_manager : public texture2d_handle_manager, public dxle::impl::Unique_HandledObject_Bace<screen_handle_manager>
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
			template<typename... T> static inline std::unique_ptr<texture2d_handle_manager> get_unique(T&&... Args){
				return std::unique_ptr<texture2d_handle_manager>(new screen_handle_manager(std::forward<T>(Args)...));
			}
		};
		//class lookup_handle_manager : public texture2d_handle_manager
		//{
		//private:
		//	lookup_handle_manager(int& raw_handle) : handle(raw_handle){}
		//	int& handle;
		//	friend divided_texture2d;
		//	template<typename... T> static inline std::unique_ptr<texture2d_handle_manager> get_unique(T&&... Args){
		//		return std::unique_ptr<texture2d_handle_manager>(new lookup_handle_manager(std::forward<T>(Args)...));
		//	}
		//public:
		//	int get_handle()const override{ return handle; }
		//};
	}

}
using namespace graph2d;
}

#endif
