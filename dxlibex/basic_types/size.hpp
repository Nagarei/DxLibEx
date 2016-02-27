/*=============================================================================
  Copyright (C) 2015-2016 DxLibEx project
  https://github.com/Nagarei/DxLibEx/

  Distributed under the Boost Software License, Version 1.0.
  (See http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#ifndef DXLE_INC_BASIC_TYPES_SIZE_HPP_
#define DXLE_INC_BASIC_TYPES_SIZE_HPP_
#include "dxlibex/config/no_min_max.h"
#include "dxlibex/type_traits/first_enabled.hpp"
#include "dxlibex/type_traits/enable_if.hpp"
#include "dxlibex/type_traits/is_representable.hpp"
#include "dxlibex/type_traits/is_nothrow.hpp"
#include "dxlibex/basic_types/use_big_type_when_one_byte_t.hpp"
#include "dxlibex/basic_types/stdint.hpp"
#include "dxlibex/basic_types/coordinate_operator_bool_helper.hpp"
#include "dxlibex/math.hpp"
#include "dxlibex/cstdlib.hpp"
//#include "dxlibex/basic_types.hpp"//DO NOT REMOVE COMMENT-OUT to avoid redefine
#include <iostream>
#include <utility>//std::pair
#include <type_traits>
#include <algorithm>
#include <cmath>
#include <limits>
#include "dxlibex/config/defines.h"

namespace dxle {
	template<typename T, enable_if_t<std::is_nothrow_move_constructible<T>::value && std::is_nothrow_move_assignable<T>::value, nullptr_t>>
	class point_c;

	/**
	\~japanese	@brief	2次元の大きさ(width, height)　テンプレートクラス。
	\~english	@brief	Template class for 2D sizes specified by its coordinates `width` and `height`.

	\~japanese	このクラスはstd::pairと相互変換が可能です。また、内部型の異なるsize_cクラス同士の変換はstatic_castを使用することで可能です(内部でも`static_cast`を使用します)
	上記のメンバーのほかに、以下の演算をサポートします
	\~english	An instance of the claess is interchangeable with std::pair. There is also a cast operator
	to convert size coordinates to the specified type (using static_cast). Commonly, the conversion
	uses this operation for each of the coordinates. Besides the class members listed in the
	declaration above, the following operations on sizes are implemented:
	\~
	@code
	pt1 = pt2 + pt3;
	pt1 = pt2 - pt3;
	pt1 = pt2 * a;
	pt1 = a * pt2;
	pt1 = pt2 / a;
	pt1 += pt2;
	pt1 -= pt2;
	pt1 *= a;
	pt1 /= a;
	pt1 == pt2;
	pt1 != pt2;
	@endcode
	\~english	For your convenience, the following type aliases are defined:
	\~japanese	利便性のために、以下の型が定義されています。
	\~
	@code
	typedef size_c<int> sizei;
	typedef size_c<unsigned int> sizeui;
	typedef size_c<double> sized;
	typedef size_c<float> sizef;
	typedef size_c<std::size_t> size;
	@endcode
	\~ Example:
	@code
	dxle::sizef a(0.3f, 0.f), b(0.f, 0.4f);
	dxle::sizei pt = (a + b)*10.f;
	std::cout << pt << std::endl;
	@endcode
	\~english @tparam T
	\~english T is the type of the elements. T must meet the requirements of <a href="http://en.cppreference.com/w/cpp/types/is_move_assignable">NothrowMoveAssignable</a> and <a href="http://en.cppreference.com/w/cpp/types/is_move_constructible">NothrowConstructable</a>.
	\~english T is expected the following conditions to use all features.
	\~english - <a href="http://en.cppreference.com/w/cpp/concept/DefaultConstructible">DefaultConstructible</a>
	\~english - <a href="http://en.cppreference.com/w/cpp/concept/CopyConstructible">CopyConstructible</a>
	\~english - <a href="http://en.cppreference.com/w/cpp/concept/CopyAssignable">CopyAssignable</a>
	\~english - has unary operator + -
	\~english - has binary operator + - * / += -= *= /= == !=
	\~english - has binary operator != to compare with 0
	\~english - able to call functon abs.
	\~japanese @tparam T
	\~japanese 型Tは少なくとも以下の条件を満たしている必要があります。
	\~japanese - 型が例外を投げずにムーブ構築可能であること（<a href="http://cpprefjp.github.io/reference/type_traits/is_nothrow_move_constructible.html">is_nothrow_move_constructible</a><T>::value == trueであること）
	\~japanese - 型が例外を投げずにムーブ代入可能であること（<a href="http://cpprefjp.github.io/reference/type_traits/is_nothrow_move_assignable.html">is_nothrow_move_assignable</a><T>::value == trueであること）
	\~japanese .
	\~japanese また、以下の条件を満たしている事が期待されています。
	\~japanese - 型がデフォルト構築可能であること（<a href="http://cpprefjp.github.io/reference/type_traits/is_default_constructible.html">is_default_constructible</a><T>::value == trueであること）
	\~japanese - 型がコピー構築可能であること（<a href="http://cpprefjp.github.io/reference/type_traits/is_copy_constructible.html">is_copy_constructible</a><T>::value == trueであること）
	\~japanese - 型がコピー代入可能であること（<a href="http://cpprefjp.github.io/reference/type_traits/is_copy_assignable.html">is_copy_assignable</a><T>::value == trueであること）
	\~japanese - 単項 + - 演算子を持つこと
	\~japanese - 二項 + - * / += -= *= /= == != 演算子を持つこと
	\~japanese - 0と!=で比較することが可能であること（  t != 0;  (tはconst T&型の変数)がコンパイル可能であること）
	\~japanese - abs関数がADLなどで見つかること
	\~
	*/
	template<typename T, enable_if_t<std::is_nothrow_move_constructible<T>::value && std::is_nothrow_move_assignable<T>::value, nullptr_t> = nullptr>
	class size_c final
	{
	public:
		typedef T value_type;
		value_type width, height;
		DXLE_CONSTEXPR_CLASS size_c() DXLE_NOEXCEPT_IF((std::is_nothrow_constructible<value_type>::value)) : width(), height() {}
		DXLE_CONSTEXPR_CLASS size_c(const value_type& width_, const value_type& height_) DXLE_NOEXCEPT_IF((std::is_nothrow_copy_constructible<value_type>::value)) : width(width_), height(height_) {}
		DXLE_CONSTEXPR_CLASS size_c(value_type&& width_, value_type&& height_) DXLE_NOEXCEPT_OR_NOTHROW : width(std::move(width_)), height(std::move(height_)) {}

		//!\~english conversion from another data type
		//!\~japanese 内部型の異なるsize_cクラス同士の変換
		template<typename Tp2_> DXLE_CONSTEXPR_CLASS explicit size_c(const size_c<Tp2_>& other) DXLE_NOEXCEPT_IF((dxle::is_nothrow_convertable<Tp2_, value_type>::value))
			: width(static_cast<value_type>(other.width)), height(static_cast<value_type>(other.height)) {}
		//!\~english conversion from another data type
		//!\~japanese 内部型の異なるsize_cクラス同士の変換
		template<typename Tp2_> DXLE_CONSTEXPR_CLASS explicit size_c(size_c<Tp2_>&& other) DXLE_NOEXCEPT_IF((dxle::is_nothrow_convertable<Tp2_&&, value_type>::value))
			: width(static_cast<value_type>(std::move(other.width))), y(static_cast<value_type>(std::move(other.height))) {}
		//copy constructor
		DXLE_CONSTEXPR_CLASS size_c(const size_c<value_type>& o) DXLE_NOEXCEPT_IF((std::is_nothrow_copy_constructible<value_type>::value)) : width(o.width), height(o.height) {}
		//move constructor
		DXLE_CONSTEXPR_CLASS size_c(size_c<value_type>&& o) DXLE_NOEXCEPT_OR_NOTHROW : width(std::move(o.width)), height(std::move(o.height)) {}

		//!\~english conversion from point_c
		//!\~japanese point_cクラスからの変換
		template<typename Tp2_> DXLE_CONSTEXPR_CLASS explicit size_c(const point_c<Tp2_, nullptr>& other) DXLE_NOEXCEPT_IF((dxle::is_nothrow_convertable<Tp2_, value_type>::value))
			: width(static_cast<value_type>(other.x)), height(static_cast<value_type>(other.y)) {}
		//!\~english conversion from point_c
		//!\~japanese point_cクラスからの変換
		template<typename Tp2_> DXLE_CONSTEXPR_CLASS explicit size_c(point_c<Tp2_, nullptr>&& other) DXLE_NOEXCEPT_IF((dxle::is_nothrow_convertable<Tp2_&&, value_type>::value))
			: width(static_cast<value_type>(std::move(other.x))), height(static_cast<value_type>(std::move(other.y))) {}
		//!\~english conversion from point_c
		//!\~japanese point_cクラスからの変換
		DXLE_CONSTEXPR_CLASS size_c(const point_c<value_type, nullptr>& other) DXLE_NOEXCEPT_IF((dxle::is_nothrow_convertable<value_type, value_type>::value))
			: width(other.x), height(other.y) {}
		//!\~english conversion from point_c
		//!\~japanese point_cクラスからの変換
		DXLE_CONSTEXPR_CLASS size_c(point_c<value_type, nullptr>&& other) DXLE_NOEXCEPT_IF((dxle::is_nothrow_convertable<value_type&&, value_type>::value))
			: width(std::move(other.x)), height(std::move(other.y)) {}

		//copy assignment operator
		size_c& operator=(const size_c<value_type>& r) DXLE_NOEXCEPT_IF((std::is_nothrow_copy_assignable<value_type>::value))
		{
			this->width = r.width;
			this->height = r.height;
			return *this;
		}
		//move assignment operator
		size_c& operator=(size_c<value_type>&& r) DXLE_NOEXCEPT_OR_NOTHROW
		{
			this->width = std::move(r.width);
			this->height = std::move(r.height);
			return *this;
		}

		//!operator bool
		//!1. operator bool
		//!2. operator != (nullptr)
		//!3. default constector + operator !=
		DXLE_CONSTEXPR_CLASS explicit operator bool() const DXLE_NOEXCEPT_IF_EXPR((dxle::detail::operator_bool_helper(this->width, this->height))) {
			return dxle::detail::operator_bool_helper(this->width, this->height);
		}
		//!\~english conversion to another data type
		//!\~japanese 内部型の異なるsize_cクラス同士の変換
		template<typename Tp2_> DXLE_CONSTEXPR_CLASS explicit operator size_c<Tp2_>() const DXLE_NOEXCEPT_IF((dxle::is_nothrow_convertable<value_type, Tp2_>::value))
		{
			return{ static_cast<Tp2_>(this->width), static_cast<Tp2_>(this->height) };
		}
		//!\~english conversion to std::pair
		//!\~japanese std::pairへの変換
		template<typename Tp2_> explicit operator std::pair<Tp2_, Tp2_>() const DXLE_NOEXCEPT_IF((dxle::is_nothrow_convertable<value_type, Tp2_>::value))
		{
			return std::pair<Tp2_, Tp2_>(static_cast<Tp2_>(this->width), static_cast<Tp2_>(this->height));
		}
	};
	//convert from std::pair

	/**
	@relates size_c
	\~japanese	@brief	std::pairからの変換
	\~english	@brief	conversion from std::pair
	\~japanese	@param pa	std::pairオブジェクトへのconst-lvalue reference
	\~english	@param pa	const-lvalue reference to std::pair
	\~japanese	@return	size_cクラスオブジェクト
	\~english	@return	size_c value
	*/
	template<typename T> size_c<T> make_size_c(const std::pair<T, T>& pa) DXLE_NOEXCEPT_IF(std::is_nothrow_copy_constructible<T>::value)
	{
		return size_c<T>(pa.first, pa.second);
	}
	/**
	@relates size_c
	\~japanese	@brief	std::pairからの変換
	\~english	@brief	conversion from std::pair
	\~japanese	@param pa	std::pairオブジェクトへのrvalue reference
	\~english	@param pa	rvalue reference to std::pair
	\~japanese	@return	size_cクラスオブジェクト
	\~english	@return	size_c value
	*/
	template<typename T> size_c<T> make_size_c(std::pair<T, T>&& pa) DXLE_NOEXCEPT_OR_NOTHROW
	{
		return size_c<T>(std::move(pa.first), std::move(pa.second));
	}

	//ostream operator
	namespace detail {
		template<typename CharType, typename Size_cType>
		void ostream_operator_helper(std::basic_ostream<CharType>& os, const CharType* str, const size_c<Size_cType>& s)
		{
			using use_big_type_when_one_byte_p = use_big_type_when_one_byte_t<Size_cType>;
			os << static_cast<use_big_type_when_one_byte_p>(s.width) << str << static_cast<use_big_type_when_one_byte_p>(s.height);
		}
		template<typename CharType, typename Size_cType>
		void istream_operator_helper(std::basic_istream<CharType>& is, size_c<Size_cType>& s)
		{
			use_big_type_when_one_byte_t<Size_cType> width, height;
			is >> width;
			is.ignore((std::numeric_limits<std::streamsize>::max)(), ',');
			is >> height;
			s.width = static_cast<Size_cType>(width); s.height = static_cast<Size_cType>(height);
		}
	}
	/**
	@relates size_c
	\~japanese	@brief	出力ストリーム演算子
	\~english	@brief	ostream operator
	\~japanese	@param os	出力ストリームへのlvalue reference
	\~english	@param os	lvalue reference to ostream
	\~japanese	@param s	size_cクラスオブジェクトへのconst-lvalue reference
	\~english	@param s	const-lvalue reference to size_c
	\~japanese	@return	第一引数に指定した出力ストリームへのlvalue reference
	\~english	@return	lvalue reference to ostream whitch is specified at first argument
	*/
	template<typename T> std::ostream& operator<<(std::ostream& os, const size_c<T>& s)
	{
		dxle::detail::ostream_operator_helper<char, T>(os, ", ", s);
		return os;
	}
	/**
	@relates size_c
	\~japanese	@brief	出力ストリーム演算子
	\~english	@brief	ostream operator
	\~japanese	@param os	出力ストリームへのlvalue reference
	\~english	@param os	lvalue reference to ostream
	\~japanese	@param s	size_cクラスオブジェクトへのconst-lvalue reference
	\~english	@param s	const-lvalue reference to size_c
	\~japanese	@return	第一引数に指定した出力ストリームへのlvalue reference
	\~english	@return	lvalue reference to ostream whitch is specified at first argument
	*/
	template<typename T> std::wostream& operator<<(std::wostream& os, const size_c<T>& s)
	{
		dxle::detail::ostream_operator_helper<wchar_t, T>(os, L", ", s);
		return os;
	}
	/**
	@relates size_c
	\~japanese	@brief	入力ストリーム演算子
	\~english	@brief	istream operator
	\~japanese	@param is	入力ストリームへのlvalue reference
	\~english	@param is	lvalue reference to istream
	\~japanese	@param s	size_cクラスオブジェクトへのconst-lvalue reference
	\~english	@param s	const-lvalue reference to size_c
	\~japanese	@return	第一引数に指定した入力ストリームへのlvalue reference
	\~english	@return	lvalue reference to istream whitch is specified at first argument
	*/
	template<typename T> std::istream& operator>>(std::istream& is, size_c<T>& s)
	{
		dxle::detail::istream_operator_helper<char, T>(is, s);
		return is;
	}
	/**
	@relates size_c
	\~japanese	@brief	入力ストリーム演算子
	\~english	@brief	istream operator
	\~japanese	@param is	入力ストリームへのlvalue reference
	\~english	@param is	lvalue reference to istream
	\~japanese	@param s	size_cクラスオブジェクトへのconst-lvalue reference
	\~english	@param s	const-lvalue reference to size_c
	\~japanese	@return	第一引数に指定した入力ストリームへのlvalue reference
	\~english	@return	lvalue reference to istream whitch is specified at first argument
	*/
	template<typename T> std::wistream& operator>>(std::wistream& is, size_c<T>& s)
	{
		dxle::detail::istream_operator_helper<wchar_t, T>(is, s);
		return is;
	}

	/**
	@relates size_c
	\~japanese	@brief	単項演算子-のオーバーロード
	\~english	@brief	Overload of unary operator -
	\~japanese	@param r	size_cクラスオブジェクト
	\~english	@param r	size_c value to negate
	\~japanese	@return	符号を逆転させた結果
	\~english	@return	Memberwise opposite of the size_c value
	*/
	template <typename T>
	DXLE_CONSTEXPR_CLASS size_c<T> operator -(const size_c<T>& r) DXLE_NOEXCEPT_IF_EXPR(-r.width)
	{
		return { -r.width, -r.height };
	}

	/**
	@relates size_c
	\~japanese	@brief	単項演算子+のオーバーロード
	\~english	@brief	Overload of unary operator +
	\~japanese	@param r	size_cクラスオブジェクトへのconst-lvalue reference
	\~english	@param r	const-lvalue reference to size_c value
	\~japanese	@return	第一引数へのconst-lvalue reference
	\~english	@return	const-lvalue reference to first argument
	*/
	template <typename T>
	DXLE_CONSTEXPR_CLASS inline size_c<T> operator +(const size_c<T>& r) DXLE_NOEXCEPT_IF(std::is_nothrow_copy_constructible<T>::value) { return r; }

	/**
	@relates size_c
	\~japanese	@brief	単項演算子+のオーバーロード
	\~english	@brief	Overload of unary operator +
	\~japanese	@param r	size_cクラスオブジェクトへのrvalue reference
	\~english	@param r	rvalue reference to size_c value
	\~japanese	@return	第一引数へのrvalue reference
	\~english	@return	rvalue reference to first argument
	*/
	template <typename T>
	DXLE_CONSTEXPR_CLASS inline size_c<T>&& operator +(size_c<T>&& r) DXLE_NOEXCEPT_OR_NOTHROW { return std::move(r); }

	/**
	@relates size_c
	\~japanese	@brief	二項演算子+=のオーバーロード
	\~english	@brief	Overload of binary operator +=
	\~japanese	@param l	size_cクラスオブジェクトへのlvalue reference
	\~english	@param l	lvalue reference to size_c value
	\~japanese	@param r	size_cクラスオブジェクトへのconst-lvalue reference
	\~english	@param r	const-lvalue reference to size_c value
	\~japanese	@return	第一引数へのlvalue reference
	\~english	@return	lvalue reference to first argument
	*/
	template <typename T1, typename T2, enable_if_t<is_representable<T2, T1>::value, nullptr_t> = nullptr>
	size_c<T1>& operator +=(size_c<T1>& l, const size_c<T2>& r) DXLE_NOEXCEPT_IF_EXPR(l.width += r.width)
	{
	    l.width += r.width;
	    l.height += r.height;
	    return l;
	}

	/**
	@relates size_c
	\~japanese	@brief	二項演算子-=のオーバーロード
	\~english	@brief	Overload of binary operator -=
	\~japanese	@param l	size_cクラスオブジェクトへのlvalue reference
	\~english	@param l	lvalue reference to size_c value
	\~japanese	@param r	size_cクラスオブジェクトへのconst-lvalue reference
	\~english	@param r	const-lvalue reference to size_c value
	\~japanese	@return	第一引数へのlvalue reference
	\~english	@return	lvalue reference to first argument
	*/
	template <typename T1, typename T2, enable_if_t<is_representable<T2, T1>::value, nullptr_t> = nullptr>
	size_c<T1>& operator -=(size_c<T1>& l, const size_c<T2>& r) DXLE_NOEXCEPT_IF_EXPR(l.width -= r.width)
	{
	    l.width -= r.width;
	    l.height -= r.height;
	    return l;
	}

	/**
	@relates size_c
	\~japanese	@brief	二項演算子+のオーバーロード
	\~english	@brief	Overload of binary operator +
	\~japanese	@param l	size_cクラスオブジェクトへのconst-lvalue reference
	\~english	@param l	const-lvalue reference to size_c value
	\~japanese	@param r	size_cクラスオブジェクトへのconst-lvalue reference
	\~english	@param r	const-lvalue reference to size_c value
	\~japanese	@return	2つのsize_cクラスオブジェクトの各メンバー同士の加算結果
	\~english	@return	Memberwise addition of both size_c value
	*/
	template <typename T1, typename T2>
	DXLE_CONSTEXPR_CLASS auto operator +(const size_c<T1>& l, const size_c<T2>& r) DXLE_NOEXCEPT_IF_EXPR(l.width + r.width)
		->size_c<decltype(std::declval<std::remove_cv_t<T1>>() + std::declval<std::remove_cv_t<T2>>())>
	{
		return {l.width + r.width, l.height + r.height};
	}

	/**
	@relates size_c
	\~japanese	@brief	二項演算子-のオーバーロード
	\~english	@brief	Overload of binary operator -
	\~japanese	@param l	size_cクラスオブジェクトへのconst-lvalue reference
	\~english	@param l	const-lvalue reference to size_c value
	\~japanese	@param r	size_cクラスオブジェクトへのconst-lvalue reference
	\~english	@param r	const-lvalue reference to size_c value
	\~japanese	@return	2つのsize_cクラスオブジェクトの各メンバー同士の減算結果
	\~english	@return	Memberwise subtraction of both size_c value
	*/
	template <typename T1, typename T2>
	DXLE_CONSTEXPR_CLASS auto operator -(const size_c<T1>& l, const size_c<T2>& r) DXLE_NOEXCEPT_IF_EXPR(l.width - r.width)
		->size_c<decltype(std::declval<std::remove_cv_t<T1>>() - std::declval<std::remove_cv_t<T2>>())>
	{
		return {l.width - r.width, l.height - r.height};
	}

	/**
	@relates size_c
	\~japanese	@brief	二項演算子*のオーバーロード
	\~english	@brief	Overload of binary operator *
	\~japanese	@param l	size_cクラスオブジェクトへのconst-lvalue reference
	\~english	@param l	const-lvalue reference to size_c value
	\~japanese	@param r	整数型の値
	\~english	@param r	arithmetic value
	\~japanese	@return	size_cクラスオブジェクトの各メンバーに第二引数を乗じた結果
	\~english	@return	Memberwise multiplication by 2nd argument
	*/
	template <typename T1, typename T2>
	DXLE_CONSTEXPR_CLASS auto operator *(const size_c<T1>& l, T2 r) DXLE_NOEXCEPT_IF_EXPR(l.width * r)
		->size_c<decltype(std::declval<std::remove_cv_t<T1>>() * std::declval<std::remove_cv_t<T2>>())>
	{
		return {l.width * r, l.height * r};
	}

	/**
	@relates size_c
	\~japanese	@brief	二項演算子*のオーバーロード
	\~english	@brief	Overload of binary operator *
	\~japanese	@param l	整数型の値
	\~english	@param l	arithmetic value
	\~japanese	@param r	size_cクラスオブジェクトへのconst-lvalue reference
	\~english	@param r	const-lvalue reference to size_c value
	\~japanese	@return	size_cクラスオブジェクトの各メンバーに第一引数を乗じた結果
	\~english	@return	Memberwise multiplication by 1st argument
	*/
	template <typename T1, typename T2>
	DXLE_CONSTEXPR_CLASS auto operator *(T1 l, const size_c<T2>& r) DXLE_NOEXCEPT_IF_EXPR(l * r.width)
		->size_c<decltype(std::declval<std::remove_cv_t<T1>>() * std::declval<std::remove_cv_t<T2>>())>
	{
		return {l * r.width, l * r.height};
	}

	/**
	@relates size_c
	\~japanese	@brief	二項演算子*=のオーバーロード
	\~english	@brief	Overload of binary operator *=
	\~japanese	@param l	size_cクラスオブジェクトへのconst-lvalue reference
	\~english	@param l	const-lvalue reference to size_c value
	\~japanese	@param r	整数型の値
	\~english	@param r	arithmetic value
	\~japanese	@return	第一引数へのlvalue reference
	\~english	@return	lvalue reference to 1st argument
	*/
	template <typename T1, typename T2, enable_if_t<is_representable<T2, T1>::value, nullptr_t> = nullptr>
	size_c<T1>& operator *=(size_c<T1>& l, T2 r) DXLE_NOEXCEPT_IF_EXPR(l.width *= r)
	{
	    l.width *= r;
	    l.height *= r;
	    return l;
	}

	/**
	@relates size_c
	\~japanese	@brief	二項演算子/のオーバーロード
	\~english	@brief	Overload of binary operator /
	\~japanese	@param l	size_cクラスオブジェクトへのconst-lvalue reference
	\~english	@param l	const-lvalue reference to size_c value
	\~japanese	@param r	整数型の値
	\~english	@param r	arithmetic value
	\~japanese	@return	size_cクラスオブジェクトの各メンバーを第一引数で割った結果
	\~english	@return	Memberwise multiplication by 1st argument
	*/
	template <typename T1, typename T2>
	DXLE_CONSTEXPR_CLASS auto operator /(const size_c<T1>& l, T2 r) DXLE_NOEXCEPT_IF_EXPR(l.width / r)
		->size_c<decltype(std::declval<std::remove_cv_t<T1>>() / std::declval<std::remove_cv_t<T2>>())>
	{
		return {l.width / r, l.height / r};
	}

	/**
	@relates size_c
	\~japanese	@brief	二項演算子/=のオーバーロード
	\~english	@brief	Overload of binary operator /=
	\~japanese	@param l	size_cクラスオブジェクトへのconst-lvalue reference
	\~english	@param l	const-lvalue reference to size_c value
	\~japanese	@param r	整数型の値
	\~english	@param r	arithmetic value
	\~japanese	@return	第一引数へのlvalue reference
	\~english	@return	lvalue reference to 1st argument
	*/
	template <typename T1, typename T2, enable_if_t<is_representable<T2, T1>::value, nullptr_t> = nullptr>
	size_c<T1>& operator /=(size_c<T1>& l, T2 r) DXLE_NOEXCEPT_IF_EXPR(l.width /= r)
	{
	    l.width /= r;
	    l.height /= r;
	    return l;
	}

	/**
	@relates size_c
	\~japanese	@brief	二項演算子!=のオーバーロード。厳密な比較が行われます
	\~english	@brief	Overload of binary operator !=. This operator compares strict difference
	\~japanese	@param l	size_cクラスオブジェクトへのconst-lvalue reference
	\~english	@param l	const-lvalue reference to size_c value
	\~japanese	@param r	size_cクラスオブジェクトへのconst-lvalue reference
	\~english	@param r	const-lvalue reference to size_c value
	\~japanese	@return	左辺と右辺が等しくなければtrueを返す
	\~english	@return	true if left operand is not equal to right operand
	*/
	template <typename T>
	DXLE_CONSTEXPR_CLASS bool operator !=(const size_c<T>& l, const size_c<T>& r) DXLE_NOEXCEPT_IF_EXPR(l.width != r.width)
	{
		return (l.width != r.width) || (l.height != r.height);
	}

	/**
	@relates size_c
	\~japanese	@brief	二項演算子==のオーバーロード。厳密な比較が行われます
	\~english	@brief	Overload of binary operator ==. This operator compares strict difference
	\~japanese	@param l	size_cクラスオブジェクトへのconst-lvalue reference
	\~english	@param l	const-lvalue reference to size_c value
	\~japanese	@param r	size_cクラスオブジェクトへのconst-lvalue reference
	\~english	@param r	const-lvalue reference to size_c value
	\~japanese	@return	左辺と右辺が等しければtrueを返す
	\~english	@return	true if left operand is equal to right operand
	*/
	template <typename T>
	DXLE_CONSTEXPR_CLASS bool operator ==(const size_c<T>& l, const size_c<T>& r) DXLE_NOEXCEPT_IF_EXPR(l != r) { return !(l != r);	}

	/**
	@relates size_c
	\~japanese	@brief	二項演算子!=のオーバーロード。厳密な比較が行われます
	\~english	@brief	Overload of binary operator !=. This operator compares strict difference
	\~japanese	@param s	size_cクラスオブジェクトへのconst-lvalue reference
	\~english	@param s	const-lvalue reference to size_c value
	\~japanese	@return	(0, 0)と等しくなければtrueを返す
	\~english	@return	true if left operand is not equal to (0, 0)
	\~@code
	dxle::sizei s = { 0 , 0 };
	bool re = s != 0;//false
	@endcode
	*/
	template <typename T>
	DXLE_CONSTEXPR_CLASS bool operator !=(const size_c<T>& s, std::nullptr_t) DXLE_NOEXCEPT_IF_EXPR(static_cast<bool>(s))
	{
		return static_cast<bool>(s);
	}
	/**
	@relates size_c
	\~japanese	@brief	二項演算子!=のオーバーロード。厳密な比較が行われます
	\~english	@brief	Overload of binary operator !=. This operator compares strict difference
	\~japanese	@param s	size_cクラスオブジェクトへのconst-lvalue reference
	\~english	@param s	const-lvalue reference to size_c value
	\~japanese	@return	(0, 0)と等しくなければtrueを返す
	\~english	@return	true if left operand is not equal to (0, 0)
	\~@code
	dxle::sizei s = { 0 , 0 };
	bool re = 0 != s;//false
	@endcode
	*/
	template <typename T>
	DXLE_CONSTEXPR_CLASS bool operator !=(std::nullptr_t, const size_c<T>& s) DXLE_NOEXCEPT_IF_EXPR(static_cast<bool>(s))
	{
		return static_cast<bool>(s);
	}
	/**
	@relates size_c
	\~japanese	@brief	二項演算子==のオーバーロード。厳密な比較が行われます
	\~english	@brief	Overload of binary operator ==. This operator compares strict difference
	\~japanese	@param p	size_cクラスオブジェクトへのconst-lvalue reference
	\~english	@param p	const-lvalue reference to size_c value
	\~japanese	@return	(0, 0)と等しければtrueを返す
	\~english	@return	true if left operand is equal to (0, 0)
	\~@code
	dxle::sizei s = { 0 , 0 };
	bool re = 0 == s;//true
	@endcode
	*/
	template <typename T>
	DXLE_CONSTEXPR_CLASS bool operator ==(const size_c<T>& s, nullptr_t) DXLE_NOEXCEPT_IF_EXPR(static_cast<bool>(s))
	{
		return !static_cast<bool>(s);
	}
	/**
	@relates size_c
	\~japanese	@brief	二項演算子==のオーバーロード。厳密な比較が行われます
	\~english	@brief	Overload of binary operator ==. This operator compares strict difference
	\~japanese	@param s	size_cクラスオブジェクトへのconst-lvalue reference
	\~english	@param s	const-lvalue reference to size_c value
	\~japanese	@return	(0, 0)と等しければtrueを返す
	\~english	@return	true if left operand is equal to (0, 0)
	\~@code
	dxle::sizei s = { 0 , 0 };
	bool re = 0 == s;//true
	@endcode
	*/
	template <typename T>
	DXLE_CONSTEXPR_CLASS bool operator ==(nullptr_t, const size_c<T>& s) DXLE_NOEXCEPT_IF_EXPR(static_cast<bool>(s))
	{
		return !static_cast<bool>(s);
	}

	/**
	@relates size_c
	\~japanese	@brief	size_cの絶対値(ベクトルの絶対値ではないのでその場合はdistanceを使ってください)
	\~english	@brief	Absolute value of size_c(THIS IS NOT THE ABSOLUTE VALUE OF THE VECTOR! use distance instead.)
	\~japanese	@param o	size_cクラスオブジェクト
	\~english	@param o	size_c value
	\~japanese	@return	第一引数に指定した入力ストリームへのlvalue reference
	\~english	@return	The absolute value of o.
	\~@code
	const dxle::sizei p1 = { -2, 4 };
	const auto result = dxle::abs(p1);//(2, 4)
	@endcode
	*/
	template<typename T>
	DXLE_CONSTEXPR_CLASS size_c<T> abs(const size_c<T>& o) DXLE_NOEXCEPT_IF_EXPR(abs(o.width)) { return { abs(o.width), abs(o.height) }; }

	typedef size_c<int> sizei;
	typedef size_c<unsigned int> sizeui;
	typedef size_c<double> sized;
	typedef size_c<float> sizef;
	typedef size_c<std::size_t> size;
};

namespace std
{
	//hash
#define DXLE_TEMP_make_hash(int_t, bit, bit2)\
	template <> struct hash<dxle::size_c<int_t##bit##_t>> {\
		hash() = default;\
		hash(const hash&) = default;\
		hash(hash&& other) :hash_run(std::move(other.hash_run)) {}\
		~hash() {}\
		hash& operator=(const hash& other) { hash_run = other.hash_run; return *this; }\
		hash& operator=(hash&& other) { hash_run = std::move(other.hash_run); return *this; }\
		size_t operator()(const dxle::size_c<int_t##bit##_t>& key) const { return hash_run((static_cast<int_t##_fast##bit2##_t>(key.width) << bit) | static_cast<int_t##_fast##bit2##_t>(key.height)); }\
		private:\
			std::hash<int_t##_fast##bit2##_t> hash_run;\
	}

	DXLE_TEMP_make_hash(int, 8, 16);
	DXLE_TEMP_make_hash(int, 16, 32);
	DXLE_TEMP_make_hash(int, 32, 64);
	DXLE_TEMP_make_hash(uint, 8, 16);
	DXLE_TEMP_make_hash(uint, 16, 32);
	DXLE_TEMP_make_hash(uint, 32, 64);

#undef DXLE_TEMP_make_hash
}

#endif //DXLE_INC_BASIC_TYPES_SIZE_HPP_
