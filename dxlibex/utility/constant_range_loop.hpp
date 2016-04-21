/*=============================================================================
Copyright (C) 2015-2016 DxLibEx project
https://github.com/Nagarei/DxLibEx/

Distributed under the Boost Software License, Version 1.0.
(See http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#pragma once
#ifndef DXLE_INC_UTILITY_CONSTANT_RANGE_LOOP_HPP_
#define DXLE_INC_UTILITY_CONSTANT_RANGE_LOOP_HPP_
#include "dxlibex/config/defines.h"
#include <cstddef>
#include "dxlibex/type_traits.hpp"
#include <iterator>
namespace dxle {
	namespace utility {
		template<typename T, enable_if_t<std::is_arithmetic<T>::value, nullptr_t> = nullptr>
		class counter_iterator : std::iterator<std::random_access_iterator_tag, T> {
		private:
			T i;
			typedef std::iterator<std::random_access_iterator_tag, T> my_t;
		public:
			typedef typename my_t::iterator_category iterator_category;
			typedef typename my_t::value_type value_type;
			typedef typename my_t::difference_type difference_type;
			typedef typename my_t::pointer pointer;
			typedef typename my_t::reference reference;

			DXLE_CONSTEXPR counter_iterator() : i() { }
			DXLE_CONSTEXPR counter_iterator(T n) : i(n) { }
			counter_iterator& operator=(const counter_iterator& o) DXLE_NOEXCEPT_OR_NOTHROW {
				i = o.i;
				return *this;
			}
			DXLE_CONSTEXPR bool operator == (const counter_iterator & rhs) const DXLE_NOEXCEPT_OR_NOTHROW { return i == rhs.i; }
			DXLE_CONSTEXPR bool operator != (const counter_iterator & rhs) const DXLE_NOEXCEPT_OR_NOTHROW { return i != rhs.i; }
			DXLE_CONSTEXPR const T & operator * () const DXLE_NOEXCEPT_OR_NOTHROW { return i; }
			T & operator * () DXLE_NOEXCEPT_OR_NOTHROW { return i; }
			DXLE_CONSTEXPR T operator[](difference_type n) const DXLE_NOEXCEPT_OR_NOTHROW { return i + n; }
			DXLE_CONSTEXPR counter_iterator operator+(difference_type n) const DXLE_NOEXCEPT_OR_NOTHROW { return{ i + n }; }
			DXLE_CONSTEXPR counter_iterator operator-(difference_type n) const DXLE_NOEXCEPT_OR_NOTHROW { return{ i - n }; }
			DXLE_CONSTEXPR bool operator<(const counter_iterator& n) const DXLE_NOEXCEPT_OR_NOTHROW { return i < n.i; }
			DXLE_CONSTEXPR bool operator<=(const counter_iterator& n) const DXLE_NOEXCEPT_OR_NOTHROW { return i <= n.i; }
			DXLE_CONSTEXPR bool operator>(const counter_iterator& n) const DXLE_NOEXCEPT_OR_NOTHROW { return i > n.i; }
			DXLE_CONSTEXPR bool operator>=(const counter_iterator& n) const DXLE_NOEXCEPT_OR_NOTHROW { return i >= n.i; }
			counter_iterator & operator += (counter_iterator it) DXLE_NOEXCEPT_OR_NOTHROW {
				i += it.i;
				return *this;
			}
			counter_iterator & operator -= (counter_iterator it) DXLE_NOEXCEPT_OR_NOTHROW {
				i -= it.i;
				return *this;
			}
			counter_iterator & operator ++ () DXLE_NOEXCEPT_OR_NOTHROW {
				++i;
				return *this;
			}
			counter_iterator operator ++ (int) DXLE_NOEXCEPT_OR_NOTHROW {
				auto t = *this;
				++i;
				return t;
			}
			counter_iterator & operator -- () DXLE_NOEXCEPT_OR_NOTHROW {
				--i;
				return *this;
			}
			counter_iterator operator -- (int) DXLE_NOEXCEPT_OR_NOTHROW {
				auto t = *this;
				--i;
				return t;
			}
		};
		template<typename T>
		DXLE_CONSTEXPR counter_iterator<T> operator+(typename counter_iterator<T>::difference_type n, const counter_iterator<T>& it) DXLE_NOEXCEPT_OR_NOTHROW { return it + n; }
		template<typename T>
		DXLE_CONSTEXPR counter_iterator<T> operator-(typename counter_iterator<T>::difference_type n, const counter_iterator<T>& it) DXLE_NOEXCEPT_OR_NOTHROW { return it - n; }
		template<typename T>
		DXLE_CONSTEXPR counter_iterator<T> iterator_next(counter_iterator<T> it) DXLE_NOEXCEPT_OR_NOTHROW { return it + 1; }
		namespace std {
			template<typename T>
			DXLE_CONSTEXPR counter_iterator<T> next(counter_iterator<T> it, typename counter_iterator<T>::difference_type n) DXLE_NOEXCEPT_OR_NOTHROW {
				return it + n;
			}
		}

		template<typename T, enable_if_t<std::is_arithmetic<T>::value, nullptr_t> = nullptr>
		class nth_loop {
		public:
			using value_type = std::remove_cv_t<T>;
			using iterator = counter_iterator<value_type>;
		private:
			value_type begin_, end_;
		public:
			DXLE_CONSTEXPR nth_loop(T n) : begin_(0), end_(n) {}
			DXLE_CONSTEXPR nth_loop(T begin, T end) : begin_(begin), end_(end) {}
			DXLE_CONSTEXPR iterator begin() const DXLE_NOEXCEPT_OR_NOTHROW {
				return{ begin_ };
			}
			DXLE_CONSTEXPR iterator end() const DXLE_NOEXCEPT_OR_NOTHROW {
				return{ end_ };
			}
		};

		template<typename T, enable_if_t<std::is_arithmetic<T>::value, nullptr_t> = nullptr>
		DXLE_CONSTEXPR nth_loop<T> rep(T n) DXLE_NOEXCEPT_OR_NOTHROW {
			return{ n };
		}
		template<typename T, enable_if_t<std::is_arithmetic<T>::value, nullptr_t> = nullptr>
		DXLE_CONSTEXPR nth_loop<T> rep(const T& begin, const T& end) DXLE_NOEXCEPT_OR_NOTHROW {
			return{ begin, end };
		}
	}
	using namespace utility;
}
#endif //DXLE_INC_UTILITY_CONSTANT_RANGE_LOOP_HPP_
