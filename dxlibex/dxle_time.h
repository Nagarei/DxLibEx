/*=============================================================================
  Copyright (C) 2015-2016 DxLibEx project
  https://github.com/Nagarei/DxLibEx/

  Distributed under the Boost Software License, Version 1.0.
  (See http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#ifndef DXLE_INC_DXLE_TIME_H_
#define DXLE_INC_DXLE_TIME_H_

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

		class timer{
		public:
			timer();

			void start(void);
			void stop(void);
			void reset(std::chrono::milliseconds = std::chrono::milliseconds{ 0 });

			std::chrono::milliseconds get_pass_time();
		private:
			int old_time;
			bool is_stop;
			std::chrono::milliseconds pass_time;

			void update();
		};

		namespace timing_maker
		{
			struct timing_maker_bace
			{
				virtual std::vector<std::chrono::milliseconds> make_timing()const = 0;
			};
			class count_per_second : public timing_maker_bace
			{
			public:
				count_per_second(size_t count, std::chrono::milliseconds second = std::chrono::milliseconds(1000))
					: timing(second / count)
				{}
				std::vector<std::chrono::milliseconds> make_timing()const override
				{
					return std::vector<std::chrono::milliseconds>(1, timing);
				}
			private:
				std::chrono::milliseconds timing;
			};
			class raw_timing : public timing_maker_bace
			{
			public:
				raw_timing(std::vector<std::chrono::milliseconds> timing_)
					: timing(std::move(timing_))
				{}
				std::vector<std::chrono::milliseconds> make_timing()const override
				{
					return timing;
				}
#if 0
				std::vector<std::chrono::milliseconds> make_timing() &&
				{
					//return std::move(timing);
					std::vector temp(std::move(timing));
					return temp;
				}
#endif
			private:
				std::vector<std::chrono::milliseconds> timing;
			};

		}

		class counter
		{
		public:
			template<typename timing_maker_T, enable_if_t<std::is_base_of<timing_maker::timing_maker_bace, typename std::remove_reference<timing_maker_T>::type>::value, nullptr_t> = nullptr>
			explicit counter(timing_maker_T&& timing_maker);

			counter(const counter& other);
			counter(counter&& other);
			counter& operator=(const counter& other);
			counter& operator=(counter&& other);

			void start(void);
			void stop(void);
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



	//----------timer----------//

		inline timer::timer()
			: old_time(0)
			, is_stop(true)
			, pass_time(0)
		{}
		inline void timer::start(void)
		{
			if (is_stop == true){
				is_stop = false;
				old_time = DxLib::GetNowCount();
			}
		}
		inline void timer::stop(void)
		{
			update();
			is_stop = true;
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
			if (is_stop == false){
				int now_time = DxLib::GetNowCount();
				pass_time += std::chrono::milliseconds((now_time - old_time) & INT_MAX);
				old_time = now_time;
			}
		}

	//----------counter----------//

		template<typename timing_maker_T, enable_if_t<std::is_base_of<timing_maker::timing_maker_bace, typename std::remove_reference<timing_maker_T>::type>::value, nullptr_t >>
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
