/*=============================================================================
Copyright (C) 2015-2016 DxLibEx project
https://github.com/Nagarei/DxLibEx/

Distributed under the Boost Software License, Version 1.0.
(See http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#ifndef DXLE_INC_BASIC_TYPES_BEL_HPP_
#define DXLE_INC_BASIC_TYPES_BEL_HPP_
#include <limits>
#include <ratio>
#include <utility>
#include "dxlibex/config/defines.h"
#include "dxlibex/type_traits/is_ratio.hpp"
#include "dxlibex/type_traits/enable_if.hpp"
#include <type_traits>
namespace dxle {
	typedef std::ratio<1, 10000> myrio;
	namespace sound_units {
		template<typename T_> struct is_bel_c;
		template<typename T, typename Period, enable_if_t<is_ratio<Period>::value, nullptr_t> = nullptr>
		class bel_c;
		template<typename To, typename T, typename Period, enable_if_t<is_bel_c<To>::value, nullptr_t> = nullptr>
		inline DXLE_CONSTEXPR To bel_cast(const bel_c<T, Period>& o);
		template<typename T, typename Period, enable_if_t<is_ratio<Period>::value, nullptr_t>>
		class bel_c {
		public:
			typedef T value_type;
			typedef Period period;
		private:
			value_type value_;
		public:
			DXLE_CONSTEXPR bel_c() = default;

			// NB: Make constexpr implicit. This cannot be explicitly constexpr, as any UDT that is not a literal type with a constexpr copy constructor will be ill-formed.
			//cf.)
			//- https://gcc.gnu.org/ml/libstdc++/2012-10/msg00120.html
			//- https://twitter.com/bolero_MURAKAMI/status/709308206640537600
			//- https://twitter.com/bolero_MURAKAMI/status/709308973850058752
			bel_c(const bel_c&) = default;

			template<typename T2, enable_if_t<std::is_convertible<T2, T>::value && (std::is_floating_point<T>::value || !std::is_floating_point<T2>::value), nullptr_t> = nullptr>
			DXLE_CONSTEXPR explicit bel_c(const T2& o) : value_(static_cast<T>(o)) {}

			template<typename T2, typename Period2, enable_if_t<
				std::is_floating_point<value_type>::value || (std::ratio_divide<Period2, period>::den == 1 && !std::is_floating_point<T2>::value), 
			nullptr_t> = nullptr>
			DXLE_CONSTEXPR bel_c(const bel_c<T2, Period2>& o) : value_(bel_cast<bel_c>(o).count()) {}
			~bel_c() = default;
			bel_c& operator=(const bel_c&) = default;

			//observer
			DXLE_CONSTEXPR value_type count() const { return value_; }
			DXLE_CONSTEXPR value_type get() const { return value_; }
			DXLE_CONSTEXPR value_type operator*() const { return value_; }
		};
		template<typename T_> struct is_bel_c : std::false_type {};
		template<typename T, typename Period> struct is_bel_c<bel_c<T, Period>> : std::true_type{};

		namespace detail {
			template<typename To, typename T, typename Period, typename CF, typename ToT, typename CR, bool num_is_one, bool den_is_one>
			struct bel_cast_helper {
				static DXLE_CONSTEXPR To cast(const bel_c<T, Period>& o) {
					return { static_cast<ToT>(static_cast<CR>(o.count()) * static_cast<CR>(CF::num) / static_cast<CR>(CF::den)) };
				}
			};
			template<typename To, typename T, typename Period, typename CF, typename ToT, typename CR>
			struct bel_cast_helper<To, T, Period, CF, ToT, CR, true, true> {
				static DXLE_CONSTEXPR To cast(const bel_c<T, Period>& o) {
					return To{ static_cast<ToT>(static_cast<ToT>(o.count())) };
				}
			};
			template<typename To, typename T, typename Period, typename CF, typename ToT, typename CR>
			struct bel_cast_helper<To, T, Period, CF, ToT, CR, false, true> {
				static DXLE_CONSTEXPR To cast(const bel_c<T, Period>& o) {
					return To{ static_cast<ToT>(static_cast<CR>(o.count()) * static_cast<CR>(CF::num)) };
				}
			};
			template<typename To, typename T, typename Period, typename CF, typename ToT, typename CR>
			struct bel_cast_helper<To, T, Period, CF, ToT, CR, true, false> {
				static DXLE_CONSTEXPR To cast(const bel_c<T, Period>& o) {
					return To{ static_cast<ToT>(static_cast<ToT>(static_cast<CR>(o.count()) / static_cast<CR>(CF::den))) };
				}
			};
		}
		template<typename To, typename T, typename Period, enable_if_t<is_bel_c<To>::value, nullptr_t>>
		inline DXLE_CONSTEXPR To bel_cast(const bel_c<T, Period>& o)
		{	// convert duration to another duration
			typedef std::ratio_divide<Period, typename To::period> CF;

			typedef typename To::value_type ToT;
			typedef std::common_type_t<ToT, T, intmax_t> CR;
			return detail::bel_cast_helper<To, T, Period, CF, ToT, CR, CF::num == 1, CF::den == 1>::cast(o);
				//return (CF::num == 1 && CF::den == 1
				//? To(static_cast<ToT>(_Dur.count()))
				//: CF::num != 1 && CF::den == 1
				//? To(static_cast<ToT>(
				//	static_cast<CR>(
				//		_Dur.count()) * static_cast<CR>(CF::num)))
				//: CF::num == 1 && CF::den != 1
				//? To(static_cast<ToT>(
				//	static_cast<CR>(_Dur.count())
				//	/ static_cast<CR>(CF::den)))
				//: To(static_cast<ToT>(
				//	static_cast<CR>(_Dur.count()) * static_cast<CR>(CF::num)
				//	/ static_cast<CR>(CF::den))));
		}


		typedef bel_c<int, myrio> myrio_bel;
		typedef bel_c<int, std::deci> deci_bel;
		typedef bel_c<int, std::ratio<1>> bel;
	}
	using namespace sound_units;
}
#ifdef DXLE_SUPPORT_CXX11_USER_DEFINED_LITERALS
#if !defined(_MSC_VER) || defined(__clang__)
namespace dxle { namespace sound_units { namespace sound_units_literals {
#else
#	define myrio_bel dxle::myrio_bel
#	define deci_bel dxle::deci_bel
#endif
	inline DXLE_CONSTEXPR myrio_bel operator "" _myrioB(unsigned long long myrioB) {
		return myrio_bel(static_cast<int>(myrioB));
	}
	inline DXLE_CONSTEXPR deci_bel operator "" _dB(unsigned long long myrioB) {
		return deci_bel(static_cast<int>(myrioB));
	}
#if !defined(_MSC_VER) || defined(__clang__)
} } }
using namespace dxle::sound_units::sound_units_literals;
#else
#	undef myrio_bel
#	undef deci_bel
#endif
#endif
#endif //DXLE_INC_BASIC_TYPES_BEL_HPP_
