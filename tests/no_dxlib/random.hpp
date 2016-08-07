/*=============================================================================
Copyright (C) 2015-2016 DxLibEx project
https://github.com/Nagarei/DxLibEx/

Distributed under the Boost Software License, Version 1.0.
(See http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#ifndef DXLE_TEST_NO_DXLIB_RANDOM_HPP_
#define DXLE_TEST_NO_DXLIB_RANDOM_HPP_
#pragma once
#if defined(__MINGW32__) && !defined(__clang__)
	//mingw-gcc's std::randome_device is broken.
	//gccをwindowsで使うならstd::random_deviceを使ってはいけない - Qiita
	//http://qiita.com/nanashi/items/f94b78398a6c79d939e1
#	define _CRT_RAND_S
#	include <stdlib.h> //rand_s
#	include <thread>
#endif //defined(__MINGW32__) && !defined(__clang__)
#if defined(_WIN32) || defined(_WIN64)
#	define DXLE_ARC_FOR_WINDWOS 1
#	if !defined(CINTERFACE) && defined(__c2__) &&  __clang_major__ == 3 && __clang_minor__ == 8
//To avoid compile error
//C:\Program Files (x86)\Windows Kits\8.1\Include\um\combaseapi.h(229,21): error : unknown type name 'IUnknown'
//          static_cast<IUnknown*>(*pp);    // make sure everyone derives from IUnknown
#		define CINTERFACE
#	endif
#	include <Windows.h>
#	include <tchar.h>
#elif defined(__linux__)
#	define DXLE_ARC_FOR_LINUX 1
#	include <sys/types.h> 
#	include <unistd.h>
#	include <fstream>
namespace dxle {
	unsigned int get_randome_from_dev_random() {
		std::ifstream file("/dev/random", std::ios::binary);
		if (file.is_open())
		{
			char buf[sizeof(unsigned int)];
			file.read(buf, sizeof(unsigned int));
			file.close();
			return *reinterpret_cast<int*>(buf);
		}
		return 0;
	}
}
#endif
#include <dxlibex/config/defines.h>
#include <random>
#include <memory>
#include <algorithm>//std::generate
#include <ctime>//clock(), time()
#include <functional>//std::ref in gcc
#include <chrono>
#include <cstdint>
#include <cstring>
#include <vector>
#include <dxlibex/type_traits.hpp>
#if !defined(__c2__) || (defined(__clang__) && (__clang_major__ >= 4 ) || __clang_major__ == 3 && __clang_minor__ >= 9)//古いClang with Microsoft CodeGenはasmに対応していない
#	ifndef __INTEL_COMPILER
#		include <immintrin.h>
#		if defined(_WIN32) || defined(_WIN64)
#		include <intrin.h>
#		else
#			include <x86intrin.h>
#		endif //defined(_WIN32) || defined(_WIN64)
#		ifdef __GNUC__
#			include <cpuid.h>
#		endif //__GNUC__
#	endif //__INTEL_COMPILER
namespace dxle {
	using std::uint32_t;
	namespace intrin {
		struct regs_t { uint32_t EAX, EBX, ECX, EDX; };
		union register_str_cvt {
			uint32_t u32[3];
			char str[12];
		};
		regs_t get_cpuid(unsigned int level) {
			regs_t re = { 0 };
			static_assert(sizeof(re) == (sizeof(uint32_t) * 4), "illegal size of struct regs_t ");
#	if defined(__INTEL_COMPILER) || defined(_MSC_VER) && !defined(__clang__)
			__cpuid(reinterpret_cast<int*>(&re), static_cast<int>(level));
#	elif defined(__GNUC__)
			__get_cpuid(level, &re.EAX, &re.EBX, &re.ECX, &re.EDX);
#	endif
			return re;
		}
		bool check_vender(const char* s) {
			const auto id = get_cpuid(0);
			register_str_cvt vender = { { id.EBX, id.EDX, id.ECX } };
			return (0 == std::memcmp(vender.str, s, sizeof(vender.str)));
		}
		bool IsIntelCPU() {
			static const auto is_intel_cpu = check_vender("GenuineIntel");//実行中にCPUは変わらないとする
			return is_intel_cpu;
		}
		bool IsAMDCPU() {
			static const auto is_amd_cpu = check_vender("AuthenticAMD");
			return is_amd_cpu;
		}
		bool IsRDRANDsupport() {
			DXLE_STATIC_CONSTEXPR uint32_t RDRAND_MASK = 1U << 30U;
			if (!IsIntelCPU() && !IsAMDCPU()) return false;
			const auto reg = get_cpuid(1);//If RDRAND is supported, the bit 30 of the ECX register is set after calling CPUID standard function 01H.
			return (RDRAND_MASK == (reg.ECX & RDRAND_MASK));
		}
		bool IsRDSEEDsupport() {
			DXLE_STATIC_CONSTEXPR uint32_t RDSEED_MASK = 1U << 18U;
			if (!IsIntelCPU()) return false;
			const auto reg = get_cpuid(7);//If RDSEED is supported, the bit 18 of the EBX register is set after calling CPUID standard function 07H.
			return (RDSEED_MASK == (reg.EBX & RDSEED_MASK));
		}
	}
}
#else
#	define DXLE_NO_ASM
#endif//!defined(_MSC_VER) || !defined(__clang__)
namespace dxle {
	namespace detail {
		template<typename Pointer>
		struct vector_push_back_helper {
			Pointer value;
		};
		template<typename value_type, typename T, bool T_is_larger>
		struct vector_push_back_operator_impl {
			void operator()(std::vector<value_type>& v, vector_push_back_helper<T> info) {
				if (info.value) v.push_back((value_type)(info.value));
			}
		};
		template<typename value_type>
		struct vector_push_back_operator_impl<value_type, value_type, false> {
			void operator()(std::vector<value_type>& v, vector_push_back_helper<value_type> info) {
				if (info.value) v.push_back(info.value);
			}
		};
		template<typename value_type, typename T>
		struct vector_push_back_operator_impl<value_type, T, true> {
			void operator()(std::vector<value_type>& v, vector_push_back_helper<T> info) {
				DXLE_STATIC_CONSTEXPR size_t size_time = sizeof(T) / sizeof(value_type);
				DXLE_STATIC_CONSTEXPR size_t rshft_num = sizeof(value_type) * CHAR_BIT;
				for (size_t i = 0; i < size_time; ++i) {
					const auto tmp = static_cast<value_type>((std::uintmax_t)(info.value) >> (rshft_num * i));
					if (tmp) v.push_back(tmp);
				}
			}
		};
		template<typename value_type, typename T, std::enable_if_t<std::is_pointer<T>::value || std::is_arithmetic<T>::value, std::nullptr_t> = nullptr>
		void operator| (std::vector<value_type>& v, vector_push_back_helper<T> info) {
			vector_push_back_operator_impl<value_type, T, (sizeof(value_type) < sizeof(T))> ()(v, info);
		}
	}
}
namespace dxle {
	template<typename T>
	dxle::detail::vector_push_back_helper<T> push_back(T pointer) { return{ pointer }; }
	using seed_v_t = std::vector<unsigned int>;
	seed_v_t create_seed_v() {
		const auto begin_time = std::chrono::high_resolution_clock::now();
#if defined(__c2__) && __clang_minor__ < 9
		constexpr std::size_t randome_device_generate_num = 12;//Clnag with Microsoft CodeGen does not support RDRND/RDSEED so that use std::random_device agressively.
#else
		DXLE_STATIC_CONSTEXPR std::size_t randome_device_generate_num = 9;
#endif
		seed_v_t sed_v(randome_device_generate_num);// 初期化用ベクター
#ifndef _CRT_RAND_S
		std::random_device rnd;// ランダムデバイス
		std::generate(sed_v.begin(), sed_v.end(), std::ref(rnd));// ベクタの初期化
#else //_CRT_RAND_S
		std::generate(sed_v.begin(), sed_v.end(), []() {
			unsigned int re;
			if (rand_s(&re)) {
				std::this_thread::sleep_for(std::chrono::milliseconds(300));
				if (auto err = rand_s(&re)) throw std::runtime_error("rand_s failed. error code:" + std::to_string(err));
			}
			return re;
		});// ベクタの初期化
#endif //_CRT_RAND_S
#ifndef DXLE_NO_ASM
		if (intrin::IsRDRANDsupport()) {//RDRAND命令の結果もベクターに追加
			for (unsigned int i = 0; i < 4; i++) {
				unsigned int rdrand_value = 0;
#	if defined(_MSC_VER) || defined(__INTEL_COMPILER)
				_rdrand32_step(&rdrand_value);
#	else//defined(_MSC_VER) || defined(__INTEL_COMPILER)
				__builtin_ia32_rdrand32_step(&rdrand_value);
#	endif//defined(_MSC_VER) || defined(__INTEL_COMPILER)
				if (0 != rdrand_value) {
					sed_v.push_back((rdrand_value < std::numeric_limits<decltype(rdrand_value)>::max() - i) ? rdrand_value + i : rdrand_value);
				}
			}
		}
		if (intrin::IsRDSEEDsupport()) {
			for (unsigned int i = 0; i < 5; i++) {
				unsigned int rdseed_value = 0;
#	if defined(_MSC_VER) || defined(__INTEL_COMPILER)
				_rdseed32_step(&rdseed_value);
#	else//defined(_MSC_VER) || defined(__INTEL_COMPILER)
				__builtin_ia32_rdseed32_step(&rdseed_value);
#	endif//defined(_MSC_VER) || defined(__INTEL_COMPILER)
				if (0 != rdseed_value) {
					sed_v.push_back((rdseed_value < std::numeric_limits<decltype(rdseed_value)>::max() - i) ? rdseed_value + i : rdseed_value);
				}
			}
		}
#endif//!defined(DXLE_NO_ASM)
#ifdef DXLE_ARC_FOR_WINDWOS
		POINT point;
		GetCursorPos(&point);
		sed_v | push_back(point.x);
		sed_v | push_back(point.y);
		sed_v | push_back(GetCurrentProcessId());
#endif //DXLE_ARC_FOR_WINDWOS
#ifdef DXLE_ARC_FOR_LINUX
		sed_v | push_back(get_randome_from_dev_random());
		sed_v | push_back(getppid());
		sed_v | push_back(get_randome_from_dev_random());
#endif
		sed_v | push_back(clock());//clock関数の結果もベクターに追加
		sed_v | push_back(time(nullptr));//time関数の結果もベクターに追加
										 //ヒープ領域のアドレスもベクターに追加
		auto heap = std::make_unique<char>();
		sed_v | push_back(heap.get());
		sed_v | push_back(&heap);
		const auto end_time = std::chrono::high_resolution_clock::now();
		sed_v | push_back((end_time - begin_time).count());
		return sed_v;
	}
	std::mt19937 create_engine() {
		auto sed_v = create_seed_v();
		std::seed_seq seq(sed_v.begin(), sed_v.end());
		return std::mt19937(seq);
	}
	template<typename T>
	using uniform_normal_distribution = dxle::first_enabled_t<
		std::enable_if<std::is_integral<T>::value, std::uniform_int_distribution<T>>,
		std::enable_if<std::is_floating_point<T>::value, std::uniform_real_distribution<T>>
	>;
}
extern std::mt19937 engine;
#endif //DXLE_TEST_NO_DXLIB_RANDOM_HPP_
