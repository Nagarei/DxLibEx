/*=============================================================================
  Copyright (C) 2015-2016 DxLibEx project
  https://github.com/Nagarei/DxLibEx/

  Distributed under the Boost Software License, Version 1.0.
  (See http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#ifndef DXLE_INC_TIME_HPP_
#define DXLE_INC_TIME_HPP_

#include "dxlibex/config/no_min_max.h"
#include <vector>
#include <chrono>
#include <type_traits>
#include <utility>
#include <algorithm>
#include <assert.h>
#include <DxLib.h>
#include "dxlibex/config/defines.h"
#include "dxlibex/type_traits/enable_if.hpp"
namespace dxle
{
	namespace time
	{

		class timer {
		public:
			timer();

			void start(void);
			void stop(void);
			bool is_stop(void);
			void reset(std::chrono::milliseconds = std::chrono::milliseconds{ 0 });

			std::chrono::milliseconds get_pass_time();
		private:
			int old_time;
			bool is_stop_;
			std::chrono::milliseconds pass_time;

			void update();
		};

		//! for constructor of timer::counter
		namespace timing_maker
		{
			struct timing_maker_bace
			{
				virtual std::vector<std::chrono::milliseconds> make_timing()const DXLE_REF_QUALIFIERS_LVALUE = 0;
#ifndef DXLE_NO_CXX11_REF_QUALIFIERS
				virtual std::vector<std::chrono::milliseconds> make_timing() &&
				{
					return this->make_timing();
				}
#endif
			};
			class count_per_second : public timing_maker_bace
			{
			public:
				//! \~japanese millisecond ミリ秒間に count 回counterをインクリメントする
				count_per_second(size_t count, std::chrono::milliseconds millisecond = std::chrono::milliseconds(1000))
					: timing(millisecond / count)
				{}
				std::vector<std::chrono::milliseconds> make_timing()const DXLE_REF_QUALIFIERS_LVALUE override
				{
					return std::vector<std::chrono::milliseconds>(1, timing);
				}
			private:
				std::chrono::milliseconds timing;
			};
			class raw_timing : public timing_maker_bace
			{
			public:
				//! \~japanese timing_[i] ミリ秒ごとにcounterをインクリメントする
				raw_timing(std::vector<std::chrono::milliseconds> timing_)
					: timing(std::move(timing_))
				{}
				std::vector<std::chrono::milliseconds> make_timing()const DXLE_REF_QUALIFIERS_LVALUE override
				{
					return timing;
				}
#ifndef DXLE_NO_CXX11_REF_QUALIFIERS
				std::vector<std::chrono::milliseconds> make_timing() && override
				{
					return std::move(timing);
				}
#endif
			private:
				std::vector<std::chrono::milliseconds> timing;
			};

		}

		class counter
		{
		public:
			template<typename timing_maker_T, typename std::enable_if<std::is_base_of<timing_maker::timing_maker_bace, typename std::remove_reference<timing_maker_T>::type>::value, nullptr_t>::type = nullptr>
			counter(timing_maker_T&& timing_maker);

			counter(const counter& other);
			counter(counter&& other);
			counter& operator=(const counter& other);
			counter& operator=(counter&& other);

			void start(void);
			void stop(void);
			bool is_stop(void);
			void reset_pass_time(std::chrono::milliseconds = std::chrono::milliseconds{ 0 });
			std::chrono::milliseconds get_pass_time();

			size_t get();
			void reset_count(size_t = 0);

		private:
			size_t count;
			std::vector<std::chrono::milliseconds> timing;
			size_t next_timing_index;
			std::chrono::milliseconds last_pass_time;
			timer timer_;

			void update();
		};

	}
	using namespace time;
}
#ifdef DXLE_SUPPORT_CXX11_USER_DEFINED_LITERALS
namespace dxle
{
	namespace literals {
		namespace time_literals {

			inline time::timing_maker::count_per_second operator""_cps(unsigned long long int count_per_second)
			{
				return time::timing_maker::count_per_second{static_cast<size_t>(count_per_second)};
			}

		}
		using namespace time_literals;
	}
	using namespace literals;

	namespace time {
		using namespace literals::time_literals;
	}
}
#endif //#ifdef DXLE_SUPPORT_CXX11_USER_DEFINED_LITERALS
namespace dxle
{
	namespace time
	{

	//----------timer----------//

		inline timer::timer()
			: old_time(0)
			, is_stop_(true)
			, pass_time(0)
		{}
		inline void timer::start(void)
		{
			if (is_stop_ == true){
				is_stop_ = false;
				old_time = DxLib::GetNowCount();
			}
		}
		inline void timer::stop(void)
		{
			update();
			is_stop_ = true;
		}
		inline bool timer::is_stop(void)
		{
			return is_stop_;
		}
		inline void timer::reset(std::chrono::milliseconds set_time)
		{
			update();
			pass_time = set_time;
		}
		inline std::chrono::milliseconds timer::get_pass_time(void)
		{
			update();
			return pass_time;
		}
		inline void timer::update()
		{
			if (is_stop_ == false){
				int now_time = DxLib::GetNowCount();
				pass_time += std::chrono::milliseconds((now_time - old_time) & INT_MAX);
				old_time = now_time;
			}
		}

	//----------counter----------//

		template<typename timing_maker_T, typename std::enable_if<std::is_base_of<timing_maker::timing_maker_bace, typename std::remove_reference<timing_maker_T>::type>::value, nullptr_t >::type>
		inline counter::counter(timing_maker_T&& timing_maker)
			: count(0)
			, timing(std::forward<timing_maker_T>(timing_maker).make_timing())
			, next_timing_index(0)
			, last_pass_time(0)
			, timer_()
		{}
		inline counter::counter(const counter& other)
			: count((other.count))
			, timing((other.timing))
			, next_timing_index((other.next_timing_index))
			, last_pass_time(other.last_pass_time)
			, timer_((other.timer_))
		{}
		inline counter& counter::operator=(const counter& other)
		{
			count = (other.count);
			timing = (other.timing);
			next_timing_index = (other.next_timing_index);
			last_pass_time = (other.last_pass_time);
			timer_ = (other.timer_);
		}
		inline counter::counter(counter&& other)
			: count(std::move(other.count))
			, timing(std::move(other.timing))
			, next_timing_index(std::move(other.next_timing_index))
			, last_pass_time(std::move(other.last_pass_time))
			, timer_(std::move(other.timer_))
		{}
		inline counter& counter::operator=(counter&& other)
		{
			count = std::move(other.count);
			timing = std::move(other.timing);
			next_timing_index = std::move(other.next_timing_index);
			last_pass_time = std::move(other.last_pass_time);
			timer_ = std::move(other.timer_);
		}
		inline void counter::start(void)
		{
			timer_.start();
		}
		inline void counter::stop(void)
		{
			timer_.stop();
		}
		inline bool counter::is_stop(void)
		{
			return timer_.is_stop();
		}
		inline void counter::reset_pass_time(std::chrono::milliseconds new_pass_time)
		{
			if (new_pass_time < timer_.get_pass_time()){
				last_pass_time = std::chrono::milliseconds{ 0 };
				count = 0;
			}
			timer_.reset(new_pass_time);
		}
		inline std::chrono::milliseconds counter::get_pass_time()
		{
			return timer_.get_pass_time();
		}

		inline size_t counter::get()
		{
			update();
			return count;
		}
		inline void counter::reset_count(size_t new_count)
		{
			update();
			count = new_count;
		}

		inline void counter::update()
		{
			using std::chrono::milliseconds;
			assert(!timing.empty());
			assert((std::any_of(timing.begin(), timing.end(), [](const milliseconds& x) { return x.count() > 0; })));

			milliseconds now_time = timer_.get_pass_time();
			for (;;)
			{
				milliseconds pass_time{ now_time - last_pass_time };
				if (timing[next_timing_index] <= pass_time) {
					++count;
					last_pass_time += pass_time;
					++next_timing_index;
					next_timing_index %= timing.size();
					continue;
				}
				break;
			}
		}
	}
}

#endif
