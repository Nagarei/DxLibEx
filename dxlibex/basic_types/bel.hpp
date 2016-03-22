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
#include <limits>
namespace dxle {
	typedef std::ratio<1, 10000> myrio;
	namespace sound_units {
		template<typename T_> struct is_bel_c;
		template<typename T, typename Period, enable_if_t<is_ratio<Period>::value, nullptr_t> = nullptr>
		class bel_c;
		template<typename To, typename T, typename Period, enable_if_t<is_bel_c<To>::value, nullptr_t> = nullptr>
		inline DXLE_CONSTEXPR To bel_cast(const bel_c<T, Period>& o);

		/// bel_c_values
		template<typename _Rep> struct bel_c_values{
			static DXLE_CONSTEXPR _Rep zero(){ return _Rep(0); }
			static DXLE_CONSTEXPR _Rep max(){ return std::numeric_limits<_Rep>::max();}
			static DXLE_CONSTEXPR _Rep min(){ return std::numeric_limits<_Rep>::lowest();	}
		};

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
			DXLE_CONSTEXPR bel_c operator+() const { return *this; }
			DXLE_CONSTEXPR bel_c operator-() const { return bel_c(-value_); }
			bel_c& operator++()
			{
			  ++value_;
			  return *this;
			}

			bel_c operator++(int) { return bel_c(value_++); }

			bel_c& operator--()
			{
			  --value_;
			  return *this;
			}

			bel_c operator--(int) { return bel_c(value_--); }

			bel_c& operator+=(const bel_c& o)
			{
			  value_ += o.count();
			  return *this;
			}

			bel_c& operator-=(const bel_c& o)
			{
			  value_ -= o.count();
			  return *this;
			}

			bel_c& operator*=(const T& r)
			{
			  value_ *= r;
			  return *this;
			}

			bel_c& operator/=(const T& r)
			{
			  value_ /= r;
			  return *this;
			}

			// DR 934.
			template<typename T2 = T, std::enable_if_t<!std::is_floating_point<T2>::value, std::nullptr_t> = nullptr>
			bel_c& operator%=(const T& r)
			{
				value_ %= r;
				return *this;
			}

			template<typename T2 = T, std::enable_if_t<!std::is_floating_point<T2>::value, std::nullptr_t> = nullptr>
			bel_c& operator%=(const bel_c& o)
			{
				value_ %= o.count();
				return *this;
			}

			//special values
			static DXLE_CONSTEXPR bel_c zero() { return bel_c(bel_c_values<T>::zero()); }
			static DXLE_CONSTEXPR bel_c min() { return bel_c(bel_c_values<T>::min()); }
			static DXLE_CONSTEXPR bel_c max() { return bel_c(bel_c_values<T>::max()); }
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
		{	// convert bel_c to another bel_c
			typedef std::ratio_divide<Period, typename To::period> CF;

			typedef typename To::value_type ToT;
			typedef std::common_type_t<ToT, T, intmax_t> CR;
			return detail::bel_cast_helper<To, T, Period, CF, ToT, CR, CF::num == 1, CF::den == 1>::cast(o);
		}

		template<typename T1, typename Period1, typename T2, typename Period2>
		DXLE_CONSTEXPR common_type_t<bel_c<T1, Period1>, bel_c<T2, Period2 >>
			operator+(const bel_c<T1, Period1>& l, const bel_c<T2, Period2>& r)
		{
			typedef bel_c<T1, Period1>			bel_c1;
			typedef bel_c<T2, Period2>			bel_c2;
			typedef common_type_t<bel_c1, bel_c2> result_t;
			return result_t(result_t(l).count() + result_t(r).count());
		}

		template<typename T1, typename Period1, typename T2, typename Period2>
		DXLE_CONSTEXPR common_type_t<bel_c<T1, Period1>, bel_c<T2, Period2>>
			operator-(const bel_c<T1, Period1>& l, const bel_c<T2, Period2>& r)
		{
			typedef bel_c<T1, Period1>			bel_c1;
			typedef bel_c<T2, Period2>			bel_c2;
			typedef common_type_t<bel_c1, bel_c2> result_t;
			return result_t(result_t(l).count() - result_t(r).count());
		}

		namespace detail {
			template<typename T2, typename Result>
			struct common_rep_type_impl : enable_if<std::is_convertible<T2, Result>::value, Result> {};
			template<typename T1, typename T2>
			struct common_rep_type : common_rep_type_impl<T2, common_type_t<T1, T2>> {};
			template<typename T1, typename T2>
			using common_rep_type_t = typename common_rep_type<T1, T2>::type;
		}

		template<typename T1, typename _Period, typename T2>
		DXLE_CONSTEXPR bel_c<detail::common_rep_type_t<T1, T2>, _Period>
			operator*(const bel_c<T1, _Period>& b, const T2& r)
		{
			typedef bel_c<common_type_t<T1, T2>, _Period> result_t;
			return result_t(result_t(b).count() * r);
		}

		template<typename T1, typename T2, typename _Period>
		DXLE_CONSTEXPR bel_c<detail::common_rep_type_t<T2, T1>, _Period>
			operator*(const T1& r, const bel_c<T2, _Period>& b)
		{
			return b * r;
		}

		template<typename T1, typename _Period, typename T2>
		DXLE_CONSTEXPR bel_c<detail::common_rep_type_t<T1, enable_if_t<!is_bel_c<T2>::value, T2>>, _Period>
			operator/(const bel_c<T1, _Period>& b, const T2& r)
		{
			typedef bel_c<common_type_t<T1, T2>, _Period> result_t;
			return result_t(result_t(b).count() / r);
		}

		template<typename T1, typename Period1, typename T2, typename Period2>
		DXLE_CONSTEXPR common_type_t<T1, T2>
			operator/(const bel_c<T1, Period1>& l, const bel_c<T2, Period2>& r)
		{
			typedef bel_c<T1, Period1>			bel_c1;
			typedef bel_c<T2, Period2>			bel_c2;
			typedef common_type_t<bel_c1, bel_c2> result_t;
			return result_t(l).count() / result_t(r).count();
		}

		// DR 934.
		template<typename T1, typename _Period, typename T2>
		DXLE_CONSTEXPR bel_c<detail::common_rep_type_t<T1, enable_if_t<!is_bel_c<T2>::value, T2>>, _Period>
			operator%(const bel_c<T1, _Period>& b, const T2& r)
		{
			typedef bel_c<common_type_t<T1, T2>, _Period> result_t;
			return result_t(result_t(b).count() % r);
		}

		template<typename T1, typename Period1, typename T2, typename Period2>
		DXLE_CONSTEXPR common_type_t<bel_c<T1, Period1>, bel_c<T2, Period2>>
			operator%(const bel_c<T1, Period1>& l, const bel_c<T2, Period2>& r)
		{
			typedef bel_c<T1, Period1>			bel_c1;
			typedef bel_c<T2, Period2>			bel_c2;
			typedef common_type_t<bel_c1, bel_c2> result_t;
			return result_t(result_t(l).count() % result_t(r).count());
		}

		// comparisons
		template<typename T1, typename Period1, typename T2, typename Period2>
		DXLE_CONSTEXPR bool operator==(const bel_c<T1, Period1>& l, const bel_c<T2, Period2>& r)
		{
			typedef bel_c<T1, Period1>			bel_c1;
			typedef bel_c<T2, Period2>			bel_c2;
			typedef common_type_t<bel_c1, bel_c2>	__ct;
			return __ct(l).count() == __ct(r).count();
		}

		template<typename T1, typename Period1, typename T2, typename Period2>
		DXLE_CONSTEXPR bool operator<(const bel_c<T1, Period1>& l, const bel_c<T2, Period2>& r)
		{
			typedef bel_c<T1, Period1>			bel_c1;
			typedef bel_c<T2, Period2>			bel_c2;
			typedef common_type_t<bel_c1, bel_c2>	__ct;
			return __ct(l).count() < __ct(r).count();
		}

		template<typename T1, typename Period1, typename T2, typename Period2>
		DXLE_CONSTEXPR bool operator!=(const bel_c<T1, Period1>& l, const bel_c<T2, Period2>& r)
		{
			return !(l == r);
		}

		template<typename T1, typename Period1, typename T2, typename Period2>
		DXLE_CONSTEXPR bool operator<=(const bel_c<T1, Period1>& l, const bel_c<T2, Period2>& r)
		{
			return !(r < l);
		}

		template<typename T1, typename Period1, typename T2, typename Period2>
		DXLE_CONSTEXPR bool operator>(const bel_c<T1, Period1>& l, const bel_c<T2, Period2>& r)
		{
			return r < l;
		}

		template<typename T1, typename Period1, typename T2, typename Period2>
		DXLE_CONSTEXPR bool operator>=(const bel_c<T1, Period1>& l, const bel_c<T2, Period2>& r)
		{
			return !(l < r);
		}

		typedef bel_c<int, myrio> myrio_bel;
		typedef bel_c<int, std::deci> deci_bel;
		typedef bel_c<int, std::ratio<1>> bel;
	}
	using namespace sound_units;
}
#ifdef DXLE_SUPPORT_CXX11_USER_DEFINED_LITERALS
namespace dxle {
	namespace literals {
		namespace sound_units_literals {
			inline DXLE_CONSTEXPR myrio_bel operator "" _myrioB(unsigned long long myrioB) {
				return myrio_bel(static_cast<int>(myrioB));
			}
			inline DXLE_CONSTEXPR deci_bel operator "" _dB(unsigned long long myrioB) {
				return deci_bel(static_cast<int>(myrioB));
			}
		}
		using namespace sound_units_literals;
	}
	using namespace literals;

	namespace sound_units {
		using namespace literals::sound_units_literals;
	}
}
#endif
#endif //DXLE_INC_BASIC_TYPES_BEL_HPP_
