﻿/*=============================================================================
  Copyright (C) 2015-2017 DxLibEx project
  https://github.com/Nagarei/DxLibEx/

  Distributed under the Boost Software License, Version 1.0.
  (See http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#ifndef DXLE_INC_TEXTURE_ANIMATION_GRAPH_HPP_
#define DXLE_INC_TEXTURE_ANIMATION_GRAPH_HPP_

#include "dxlibex/config/no_min_max.h"
#include <iterator>
#include <vector>
#include <mutex>
#include <functional>
#include "DxLib.h"
#include "texture2d.hpp"
#include "dxlibex/time.hpp"
#include "dxlibex/thread.hpp"

namespace dxle
{
	//! 2Dグラフィック
	namespace graph2d
	{
		namespace gr_impl
		{
			class animation_handle_manager_bace : public texture2d_handle_manager
			{
			protected:
				mutable time::counter counter;
#ifdef DX_THREAD_SAFE
				mutable std::mutex counter_mtx;
#endif

				animation_handle_manager_bace(time::counter&& counter_)
					: counter(std::move(counter_))
				{
					counter.start();
				}
				friend animation_graph;
				virtual std::unique_ptr<texture2d_handle_manager> clone() = 0;
			};
			template<typename Cont>
			class animation_handle_manager final : public animation_handle_manager_bace
			{
			public:
				int get_handle()const override { DXLE_GET_LOCK(counter_mtx); return texture2d_handle_manager::GetTextureRawHandle(graphs[counter.get() % graphs.size()]); }

			private:
				using this_T = animation_handle_manager<Cont>;
				typename std::remove_reference<Cont>::type graphs;

				animation_handle_manager(time::counter&& counter, Cont&& graphs)
					: animation_handle_manager_bace(std::move(counter))
					, graphs(std::forward<Cont>(graphs))
				{}
				std::unique_ptr<texture2d_handle_manager> clone() override
				{
					std::vector<texture2d> temp_graphs;
					temp_graphs.resize(graphs.size());
					for(size_t i = 0;i < graphs.size(); ++i) {
						temp_graphs[i] = (std::move(*graphs[i].clone()));
					}
					return animation_handle_manager<std::vector<texture2d>>::get_unique(time::counter(this->counter), std::move(temp_graphs));
				}
				template<typename Cont2>
				friend class animation_handle_manager;
				friend animation_graph;
				template<typename... Args> static inline std::unique_ptr<texture2d_handle_manager> get_unique(Args&&... args) {
					return std::unique_ptr<texture2d_handle_manager>(new this_T(std::forward<Args>(args)...));
				}
			};
#ifdef DXLE_TEMP_IMPL_MAKE_ANI_HM
			static_assert(false, "");
#endif
#define DXLE_TEMP_IMPL_MAKE_ANI_HM(template_param, specialization, size)\
			template<template_param>\
			class animation_handle_manager<specialization> final : public animation_handle_manager_bace\
			{\
			public:\
				int get_handle()const override\
				{\
					DXLE_GET_LOCK(counter_mtx);\
					return texture2d_handle_manager::GetTextureRawHandle(graphs.get()[counter.get() % size]);\
				}\
			private:\
				using Cont = specialization;\
				using this_T = animation_handle_manager<Cont>;\
				typename std::remove_reference<Cont>::type graphs;\
				animation_handle_manager(time::counter&& counter, Cont&& graphs)\
					: animation_handle_manager_bace(std::move(counter))\
					, graphs(std::move(graphs))\
				{}\
				std::unique_ptr<texture2d_handle_manager> clone() override\
				{\
					return get_unique(*this);\
				}\
				friend animation_graph;\
				template<typename... Args> static inline std::unique_ptr<texture2d_handle_manager> get_unique(Args&&... args) {\
					return std::unique_ptr<texture2d_handle_manager>(new this_T(std::forward<Args>(args)...));\
				}\
			}
			DXLE_TEMP_IMPL_MAKE_ANI_HM(typename Cont_value_T, std::reference_wrapper<Cont_value_T>, graphs.get().size());
			DXLE_TEMP_IMPL_MAKE_ANI_HM(size_t N, std::reference_wrapper<dxle::texture2d[N]>, N);
			DXLE_TEMP_IMPL_MAKE_ANI_HM(size_t N, std::reference_wrapper<const dxle::texture2d[N]>, N);
			DXLE_TEMP_IMPL_MAKE_ANI_HM(size_t N, std::reference_wrapper<volatile dxle::texture2d[N]>, N);
			DXLE_TEMP_IMPL_MAKE_ANI_HM(size_t N, std::reference_wrapper<const volatile dxle::texture2d[N]>, N);
#undef DXLE_TEMP_IMPL_MAKE_ANI_HM

			template<typename Cont>
			class cont_wrapper_with_size final
			{
			public:
				const texture2d& operator[](size_t i)const{ return graphs[i]; }
				size_t size()const DXLE_NOEXCEPT_OR_NOTHROW{ return size_; }
				cont_wrapper_with_size(Cont&& graphs, size_t size)
					: graphs(std::forward<Cont>(graphs))
					, size_(size)
				{}
			private:
				typename std::remove_reference<Cont>::type graphs;
				size_t size_;
			};
			template<typename Cont>
			class cont_wrapper_with_size<std::reference_wrapper<Cont>>final
			{
			public:
				const texture2d& operator[](size_t i)const{ return (graphs.get())[i]; }
				size_t size()const DXLE_NOEXCEPT_OR_NOTHROW{ return size_; }
				cont_wrapper_with_size(std::reference_wrapper<Cont>&& graphs, size_t size)
					: graphs(std::move(graphs))
					, size_(size)
				{}
			private:
				std::reference_wrapper<Cont> graphs;
				size_t size_;
			};
		}
		class animation_graph final : public texture2d
		{
		public:
			animation_graph()DXLE_NOEXCEPT = default;

			template<typename Cont>
			//! @param graphs アニメーションする画像(dxle::texture2dのコンテナ)
			//! @param graphs 要件：graphs[i]およびgraphs.size()が可能であるorそのようなgraphsへのstd::reference_wrapperであること
			animation_graph(time::counter counter, Cont graphs)
				: texture2d(gr_impl::animation_handle_manager<Cont>::get_unique(std::move(counter), std::move(graphs)))
			{}
			template<size_t N>
			//! @param graphs アニメーションする画像(dxle::texture2dのコンテナ)
			animation_graph(time::counter counter, const texture2d(&graphs)[N])
				: animation_graph(counter, graphs, N)
			{}
			template<typename Cont>
			//! @param graphs アニメーションする画像(dxle::texture2dのコンテナ)
			//! @param graphs 要件：graphs[i]が可能であるorそのようなgraphsへのstd::reference_wrapperであること
			animation_graph(time::counter counter, Cont graphs, size_t size)
				: texture2d(gr_impl::animation_handle_manager<gr_impl::cont_wrapper_with_size<Cont>>::get_unique(std::move(counter), gr_impl::cont_wrapper_with_size<Cont>(std::move(graphs), size)))
			{}

			//! コピー禁止
			animation_graph(const screen& other) = delete;
			//!所有権の譲渡
			animation_graph(animation_graph&& other) DXLE_NOEXCEPT_OR_NOTHROW : texture2d(std::move(other)){}
			//! コピー禁止
			animation_graph& operator=(const animation_graph& other) = delete;
			//! 所有権の譲渡
			animation_graph& operator=(animation_graph&& other) DXLE_NOEXCEPT_OR_NOTHROW{ texture2d::operator=(std::move(other)); return *this; }

			//!画像を複製する
			std::unique_ptr<texture2d> clone()const override{ return cloneAni(); }
			//!画像を複製する
			std::unique_ptr<animation_graph> cloneAni()const;

			void start(void);
			void stop(void);
			bool is_stop(void);
			void reset_pass_time(std::chrono::milliseconds = std::chrono::milliseconds{ 0 });
			std::chrono::milliseconds get_pass_time();

			size_t get_count();
			void reset_count(size_t = 0);

		private:
			static std::vector<texture2d> cast_to_vector(derivative_texture2d&&);
			animation_graph(std::unique_ptr<gr_impl::texture2d_handle_manager>&& handle_manager)
				: texture2d(std::move(handle_manager))
			{}
		};

		inline std::unique_ptr<animation_graph> animation_graph::cloneAni()const
		{
			auto hm_ptr = static_cast<gr_impl::animation_handle_manager_bace*>(handle_manager.get());
			//return std::make_unique<animation_graph>(hm_ptr->clone());
			return std::unique_ptr<animation_graph>(new animation_graph(hm_ptr->clone()));
		}
		inline std::vector<texture2d> animation_graph::cast_to_vector(derivative_texture2d&& div_texture)
		{
			std::vector<texture2d> temp_graphs(div_texture.size());
			for (size_t i = 0; i < div_texture.size(); ++i){
				temp_graphs[i] = std::move(div_texture[i]);
			}
			return temp_graphs;
		}
		inline void animation_graph::start(void)
		{
			static_cast<gr_impl::animation_handle_manager_bace*>(handle_manager.get())->counter.start();
		}
		inline void animation_graph::stop(void)
		{
			static_cast<gr_impl::animation_handle_manager_bace*>(handle_manager.get())->counter.stop();
		}
		inline bool animation_graph::is_stop(void)
		{
			return static_cast<gr_impl::animation_handle_manager_bace*>(handle_manager.get())->counter.is_stop();
		}
		inline void animation_graph::reset_pass_time(std::chrono::milliseconds new_pass_time)
		{
			static_cast<gr_impl::animation_handle_manager_bace*>(handle_manager.get())->counter.reset_pass_time(new_pass_time);
		}
		inline std::chrono::milliseconds animation_graph::get_pass_time()
		{
			return static_cast<gr_impl::animation_handle_manager_bace*>(handle_manager.get())->counter.get_pass_time();
		}
		inline size_t animation_graph::get_count()
		{
			return static_cast<gr_impl::animation_handle_manager_bace*>(handle_manager.get())->counter.get();
		}
		inline void animation_graph::reset_count(size_t new_count)
		{
			static_cast<gr_impl::animation_handle_manager_bace*>(handle_manager.get())->counter.reset_count(new_count);
		}
	}
	using namespace graph2d;
}

#endif
