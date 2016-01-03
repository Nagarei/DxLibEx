/*=============================================================================
  Copyright (C) 2015-2016 DxLibEx project
  https://github.com/Nagarei/DxLibEx/

  Distributed under the Boost Software License, Version 1.0.
  (See http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#ifndef DXLE_INC_BASIC_TYPES_HPP_
#define DXLE_INC_BASIC_TYPES_HPP_
//#pragma once
#include "dxlibex/config/no_min_max.h"
#include "dxlibex/type_traits/first_enabled.hpp"
#include "dxlibex/type_traits/enable_if.hpp"
#include <iostream>//ostream
#include <utility>//std::pair
#include <type_traits>
#include <algorithm>
#include <cstdint>
#include <cmath>
#include <limits>
#include "dxlibex/config/defines.h"

namespace dxle {
	/** 
	\~japanese	@brief	2次元座標(x, y)　テンプレートクラス。
	\~english	@brief	Template class for 2D points specified by its coordinates `x` and `y`.

	\~japanese	このクラスはstd::pairと相互変換が可能です。また、内部型の異なるpoint_cクラス同士の変換はstatic_castを使用することで可能です(内部でも`static_cast`を使用します)
				上記のメンバーのほかに、以下の演算をサポートします
	\~english	An instance of the claess is interchangeable with std::pair. There is also a cast operator
				to convert point coordinates to the specified type (using static_cast). Commonly, the conversion
				uses this operation for each of the coordinates. Besides the class members listed in the
				declaration above, the following operations on points are implemented:
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
		typedef point_c<int> pointi;
		typedef point_c<uint8_t> pointu8i;
		typedef point_c<int8_t> point8i;
		typedef point_c<double> pointd;
		typedef point_c<float> pointf;
	@endcode
	\~ Example:
	@code
	    dxle::pointf a(0.3f, 0.f), b(0.f, 0.4f);
	    dxle::pointi pt = (a + b)*10.f;
	    std::cout << pt << std::endl;
	@endcode
	*/
	template<typename T, enable_if_t<std::is_arithmetic<T>::value, std::nullptr_t> = nullptr>
	class point_c final
	{
	public:
		typedef T value_type;
		value_type x, y;
		point_c() DXLE_NOEXCEPT_OR_NOTHROW : x(), y() {}
		point_c(value_type x_, value_type y_) DXLE_NOEXCEPT_OR_NOTHROW : x(x_), y(y_) {}

		//copy constructor
		point_c(const point_c<value_type>& o) DXLE_NOEXCEPT_OR_NOTHROW : x(o.x), y(o.y) {}
		//move constructor
		point_c(point_c<value_type>&& o) DXLE_NOEXCEPT_OR_NOTHROW : x(std::move(o.x)), y(std::move(o.y)) {}
		//copy assignment operator
		point_c& operator=(const point_c<value_type>& r) DXLE_NOEXCEPT_OR_NOTHROW
		{
			this->x = r.x;
			this->y = r.y;
			return *this;
		}
		//move assignment operator
		point_c& operator=(point_c<value_type>&& r) DXLE_NOEXCEPT_OR_NOTHROW
		{
			this->x = std::move(r.x);
			this->y = std::move(r.y);
			return *this;
		}


		explicit operator bool() const DXLE_NOEXCEPT_OR_NOTHROW {
			return (0 != this->x) || (0 != this->y);
		}
		//!\~english conversion to another data type
		//!\~japanese 内部型の異なるpoint_cクラス同士の変換
		template<typename _Tp2> explicit operator point_c<_Tp2>() const DXLE_NOEXCEPT_OR_NOTHROW
		{
			return point_c<_Tp2>(static_cast<_Tp2>(this->x), static_cast<_Tp2>(this->y));
		}
		//!\~english conversion to std::pair
		//!\~japanese std::pairへの変換
		template<typename _Tp2> explicit operator std::pair<_Tp2, _Tp2>() const DXLE_NOEXCEPT_OR_NOTHROW
		{
			return std::pair<_Tp2, _Tp2>(static_cast<_Tp2>(this->x), static_cast<_Tp2>(this->y));
		}
	};
	//convert from std::pair

	/**
	@relates point_c
	\~japanese	@brief	std::pairからの変換
	\~english	@brief	conversion from std::pair
	\~japanese	@param p	std::pairオブジェクトへのconst-lvalue reference
	\~english	@param p	const-lvalue reference to std::pair
	\~japanese	@return	point_cクラスオブジェクト
	\~english	@return	point_c value
	*/
	template<typename T> point_c<T> make_point_c(const std::pair<T, T>& p) DXLE_NOEXCEPT_OR_NOTHROW
	{
		return point_c<T>(p.first, p.second);
	}
	/**
	@relates point_c
	\~japanese	@brief	std::pairからの変換
	\~english	@brief	conversion from std::pair
	\~japanese	@param p	std::pairオブジェクトへのrvalue reference
	\~english	@param p	rvalue reference to std::pair
	\~japanese	@return	point_cクラスオブジェクト
	\~english	@return	point_c value
	*/
	template<typename T> point_c<T> make_point_c(std::pair<T, T>&& p) DXLE_NOEXCEPT_OR_NOTHROW
	{
		return point_c<T>(std::move(p.first), std::move(p.second));
	}

	//ostream operator
	namespace detail {
		//! for int8_t/uint8_t
		template<typename T, enable_if_t<std::is_arithmetic<T>::value, std::nullptr_t> = nullptr>
		using arithmetic_t = first_enabled_t <
			enable_if<1 != sizeof(T), T>,
			enable_if<std::is_signed<T>::value, int>,
			unsigned int
		>;
		template<typename CharType, typename PointType>
		struct ostream_operator_helper {
			void operator()(std::basic_ostream<CharType>& os, const CharType* str, const point_c<PointType>& p)
			{
				using arithmetic_p = arithmetic_t<PointType>;
				os << static_cast<arithmetic_p>(p.x) << str << static_cast<arithmetic_p>(p.y);
			}
		};
		template<typename CharType, typename PointType>
		struct istream_operator_helper {
			void operator()(std::basic_istream<CharType>& is, point_c<PointType>& p)
			{
				arithmetic_t<PointType> x, y;
				is >> x;
				is.ignore((std::numeric_limits<std::streamsize>::max)(), ',');
				is >> y;
				p.x = static_cast<PointType>(x); p.y = static_cast<PointType>(y);
			}
		};
	}
	/**
	@relates point_c
	\~japanese	@brief	出力ストリーム演算子
	\~english	@brief	ostream operator
	\~japanese	@param os	出力ストリームへのlvalue reference
	\~english	@param os	lvalue reference to ostream
	\~japanese	@param p	point_cクラスオブジェクトへのconst-lvalue reference
	\~english	@param p	const-lvalue reference to point_c
	\~japanese	@return	第一引数に指定した出力ストリームへのlvalue reference
	\~english	@return	lvalue reference to ostream whitch is specified at first argument
	*/
	template<typename T> std::ostream& operator<<(std::ostream& os, const point_c<T>& p)
	{
		detail::ostream_operator_helper<char, T>()(os, ", ", p);
		return os;
	}
	/**
	@relates point_c
	\~japanese	@brief	出力ストリーム演算子
	\~english	@brief	ostream operator
	\~japanese	@param os	出力ストリームへのlvalue reference
	\~english	@param os	lvalue reference to ostream
	\~japanese	@param p	point_cクラスオブジェクトへのconst-lvalue reference
	\~english	@param p	const-lvalue reference to point_c
	\~japanese	@return	第一引数に指定した出力ストリームへのlvalue reference
	\~english	@return	lvalue reference to ostream whitch is specified at first argument
	*/
	template<typename T> std::wostream& operator<<(std::wostream& os, const point_c<T>& p)
	{
		detail::ostream_operator_helper<wchar_t, T>()(os, L", ", p);
		return os;
	}
	/**
	@relates point_c
	\~japanese	@brief	入力ストリーム演算子
	\~english	@brief	istream operator
	\~japanese	@param is	入力ストリームへのlvalue reference
	\~english	@param is	lvalue reference to istream
	\~japanese	@param p	point_cクラスオブジェクトへのconst-lvalue reference
	\~english	@param p	const-lvalue reference to point_c
	\~japanese	@return	第一引数に指定した入力ストリームへのlvalue reference
	\~english	@return	lvalue reference to istream whitch is specified at first argument
	*/
	template<typename T> std::istream& operator>>(std::istream& is, point_c<T>& p)
	{
		detail::istream_operator_helper<char, T>()(is, p);
		return is;
	}
	/**
	@relates point_c
	\~japanese	@brief	入力ストリーム演算子
	\~english	@brief	istream operator
	\~japanese	@param is	入力ストリームへのlvalue reference
	\~english	@param is	lvalue reference to istream
	\~japanese	@param p	point_cクラスオブジェクトへのconst-lvalue reference
	\~english	@param p	const-lvalue reference to point_c
	\~japanese	@return	第一引数に指定した入力ストリームへのlvalue reference
	\~english	@return	lvalue reference to istream whitch is specified at first argument
	*/
	template<typename T> std::wistream& operator>>(std::wistream& is, point_c<T>& p)
	{
		detail::istream_operator_helper<wchar_t, T>()(is, p);
		return is;
	}

	/**
	@relates point_c
	\~japanese	@brief	単項演算子-のオーバーロード
	\~english	@brief	Overload of unary operator -
	\~japanese	@param r	point_cクラスオブジェクト
	\~english	@param r	point_c value to negate
	\~japanese	@return	符号を逆転させた結果
	\~english	@return	Memberwise opposite of the point_c value
	*/
	template <typename T>
	point_c<T> operator -(const point_c<T>& r) DXLE_NOEXCEPT_OR_NOTHROW
	{
		return { -r.x, -r.y };
	}

	/**
	@relates point_c
	\~japanese	@brief	単項演算子+のオーバーロード
	\~english	@brief	Overload of unary operator +
	\~japanese	@param r	point_cクラスオブジェクトへのconst-lvalue reference
	\~english	@param r	const-lvalue reference to point_c value
	\~japanese	@return	第一引数へのconst-lvalue reference
	\~english	@return	const-lvalue reference to first argument
	*/
	template <typename T>
	inline const point_c<T>& operator +(const point_c<T>& r) DXLE_NOEXCEPT_OR_NOTHROW { return r; }

	/**
	@relates point_c
	\~japanese	@brief	単項演算子+のオーバーロード
	\~english	@brief	Overload of unary operator +
	\~japanese	@param r	point_cクラスオブジェクトへのrvalue reference
	\~english	@param r	rvalue reference to point_c value
	\~japanese	@return	第一引数へのrvalue reference
	\~english	@return	rvalue reference to first argument
	*/
	template <typename T>
	inline point_c<T>&& operator +(point_c<T>&& r) DXLE_NOEXCEPT_OR_NOTHROW { return std::move(r); }

	/**
	@relates point_c
	\~japanese	@brief	二項演算子+=のオーバーロード
	\~english	@brief	Overload of binary operator +=
	\~japanese	@param l	point_cクラスオブジェクトへのlvalue reference
	\~english	@param l	lvalue reference to point_c value
	\~japanese	@param r	point_cクラスオブジェクトへのconst-lvalue reference
	\~english	@param r	const-lvalue reference to point_c value
	\~japanese	@return	第一引数へのlvalue reference
	\~english	@return	lvalue reference to first argument
	*/
	template <typename T>
	point_c<T>& operator +=(point_c<T>& l, const point_c<T>& r) DXLE_NOEXCEPT_OR_NOTHROW
	{
	    l.x += r.x;
	    l.y += r.y;
	    return l;
	}

	/**
	@relates point_c
	\~japanese	@brief	二項演算子-=のオーバーロード
	\~english	@brief	Overload of binary operator -=
	\~japanese	@param l	point_cクラスオブジェクトへのlvalue reference
	\~english	@param l	lvalue reference to point_c value
	\~japanese	@param r	point_cクラスオブジェクトへのconst-lvalue reference
	\~english	@param r	const-lvalue reference to point_c value
	\~japanese	@return	第一引数へのlvalue reference
	\~english	@return	lvalue reference to first argument
	*/
	template <typename T>
	point_c<T>& operator -=(point_c<T>& l, const point_c<T>& r) DXLE_NOEXCEPT_OR_NOTHROW
	{
	    l.x -= r.x;
	    l.y -= r.y;
	    return l;
	}

	/**
	@relates point_c
	\~japanese	@brief	二項演算子+のオーバーロード
	\~english	@brief	Overload of binary operator +
	\~japanese	@param l	point_cクラスオブジェクトへのconst-lvalue reference
	\~english	@param l	const-lvalue reference to point_c value
	\~japanese	@param r	point_cクラスオブジェクトへのconst-lvalue reference
	\~english	@param r	const-lvalue reference to point_c value
	\~japanese	@return	2つのpoint_cクラスオブジェクトの各メンバー同士の加算結果
	\~english	@return	Memberwise addition of both point_c value
	*/
	template <typename T>
	point_c<T> operator +(const point_c<T>& l, const point_c<T>& r) DXLE_NOEXCEPT_OR_NOTHROW
	{
		return {l.x + r.x, l.y + r.y};
	}

	/**
	@relates point_c
	\~japanese	@brief	二項演算子-のオーバーロード
	\~english	@brief	Overload of binary operator -
	\~japanese	@param l	point_cクラスオブジェクトへのconst-lvalue reference
	\~english	@param l	const-lvalue reference to point_c value
	\~japanese	@param r	point_cクラスオブジェクトへのconst-lvalue reference
	\~english	@param r	const-lvalue reference to point_c value
	\~japanese	@return	2つのpoint_cクラスオブジェクトの各メンバー同士の減算結果
	\~english	@return	Memberwise subtraction of both point_c value
	*/
	template <typename T>
	point_c<T> operator -(const point_c<T>& l, const point_c<T>& r) DXLE_NOEXCEPT_OR_NOTHROW
	{
		return {l.x - r.x, l.y - r.y};
	}

	/**
	@relates point_c
	\~japanese	@brief	二項演算子*のオーバーロード
	\~english	@brief	Overload of binary operator *
	\~japanese	@param l	point_cクラスオブジェクトへのconst-lvalue reference
	\~english	@param l	const-lvalue reference to point_c value
	\~japanese	@param r	整数型の値
	\~english	@param r	arithmetic value
	\~japanese	@return	point_cクラスオブジェクトの各メンバーに第二引数を乗じた結果
	\~english	@return	Memberwise multiplication by 2nd argument
	*/
	template <typename T1, typename T2, enable_if_t<std::is_arithmetic<T1>::value && std::is_arithmetic<T2>::value, std::nullptr_t> = nullptr>
	auto operator *(const point_c<T1>& l, T2 r) DXLE_NOEXCEPT_OR_NOTHROW -> point_c<decltype(l.x * r)>
	{
		return {l.x * r, l.y * r};
	}

	/**
	@relates point_c
	\~japanese	@brief	二項演算子*のオーバーロード
	\~english	@brief	Overload of binary operator *
	\~japanese	@param l	整数型の値
	\~english	@param l	arithmetic value
	\~japanese	@param r	point_cクラスオブジェクトへのconst-lvalue reference
	\~english	@param r	const-lvalue reference to point_c value
	\~japanese	@return	point_cクラスオブジェクトの各メンバーに第一引数を乗じた結果
	\~english	@return	Memberwise multiplication by 1st argument
	*/
	template <typename T1, typename T2, enable_if_t<std::is_arithmetic<T1>::value && std::is_arithmetic<T2>::value, std::nullptr_t> = nullptr>
	auto operator *(T1 l, const point_c<T2>& r) DXLE_NOEXCEPT_OR_NOTHROW -> point_c<decltype(l * r.x)>
	{
		return {l + r.x, l + r.y};
	}

	/**
	@relates point_c
	\~japanese	@brief	二項演算子*=のオーバーロード
	\~english	@brief	Overload of binary operator *=
	\~japanese	@param l	point_cクラスオブジェクトへのconst-lvalue reference
	\~english	@param l	const-lvalue reference to point_c value
	\~japanese	@param r	整数型の値
	\~english	@param r	arithmetic value
	\~japanese	@return	第一引数へのlvalue reference
	\~english	@return	lvalue reference to 1st argument
	*/
	template <typename T, enable_if_t<std::is_arithmetic<T>::value, std::nullptr_t> = nullptr>
	point_c<T>& operator *=(point_c<T>& l, T r) DXLE_NOEXCEPT_OR_NOTHROW
	{
	    l.x *= r;
	    l.y *= r;
	    return l;
	}

	/**
	@relates point_c
	\~japanese	@brief	二項演算子/のオーバーロード
	\~english	@brief	Overload of binary operator /
	\~japanese	@param l	point_cクラスオブジェクトへのconst-lvalue reference
	\~english	@param l	const-lvalue reference to point_c value
	\~japanese	@param r	整数型の値
	\~english	@param r	arithmetic value
	\~japanese	@return	point_cクラスオブジェクトの各メンバーを第一引数で割った結果
	\~english	@return	Memberwise multiplication by 1st argument
	*/
	template <typename T1, typename T2, enable_if_t<std::is_arithmetic<T1>::value && std::is_arithmetic<T2>::value, std::nullptr_t> = nullptr>
	auto operator /(const point_c<T1>& l, T2 r) DXLE_NOEXCEPT_OR_NOTHROW -> point_c<decltype(l.x / r)>
	{
		return {l.x / r, l.y / r};
	}

	/**
	@relates point_c
	\~japanese	@brief	二項演算子/=のオーバーロード
	\~english	@brief	Overload of binary operator /=
	\~japanese	@param l	point_cクラスオブジェクトへのconst-lvalue reference
	\~english	@param l	const-lvalue reference to point_c value
	\~japanese	@param r	整数型の値
	\~english	@param r	arithmetic value
	\~japanese	@return	第一引数へのlvalue reference
	\~english	@return	lvalue reference to 1st argument
	*/
	template <typename T, enable_if_t<std::is_arithmetic<T>::value, std::nullptr_t> = nullptr>
	point_c<T>& operator /=(point_c<T>& l, T r) DXLE_NOEXCEPT_OR_NOTHROW 
	{
	    l.x /= r;
	    l.y /= r;
	    return l;
	}

	/**
	@relates point_c
	\~japanese	@brief	二項演算子!=のオーバーロード。厳密な比較が行われます
	\~english	@brief	Overload of binary operator !=. This operator compares strict difference
	\~japanese	@param l	point_cクラスオブジェクトへのconst-lvalue reference
	\~english	@param l	const-lvalue reference to point_c value
	\~japanese	@param r	point_cクラスオブジェクトへのconst-lvalue reference
	\~english	@param r	const-lvalue reference to point_c value
	\~japanese	@return	左辺と右辺が等しくなければtrueを返す
	\~english	@return	true if left operand is not equal to right operand
	*/
	template <typename T, enable_if_t<std::is_arithmetic<T>::value, std::nullptr_t> = nullptr>
	bool operator !=(const point_c<T>& l, const point_c<T>& r) DXLE_NOEXCEPT_OR_NOTHROW
	{
		return (l.x != r.x) || (l.y != r.y);
	}

	/**
	@relates point_c
	\~japanese	@brief	二項演算子!=のオーバーロード。厳密な比較が行われます
	\~english	@brief	Overload of binary operator !=. This operator compares strict difference
	\~japanese	@param p	point_cクラスオブジェクトへのconst-lvalue reference
	\~english	@param p	const-lvalue reference to point_c value
	\~japanese	@return	(0, 0)と等しくなければtrueを返す
	\~english	@return	true if left operand is not equal to (0, 0)
	\~@code
	dxle::pointi p = { 0 , 0 };
	bool re = p != 0;//false
	@endcode
	*/
	template <typename T, enable_if_t<std::is_arithmetic<T>::value, std::nullptr_t> = nullptr>
	bool operator !=(const point_c<T>& p, std::nullptr_t) DXLE_NOEXCEPT_OR_NOTHROW
	{
		return !static_cast<bool>(p);
	}

	/**
	@relates point_c
	\~japanese	@brief	二項演算子!=のオーバーロード。厳密な比較が行われます
	\~english	@brief	Overload of binary operator !=. This operator compares strict difference
	\~japanese	@param p	point_cクラスオブジェクトへのconst-lvalue reference
	\~english	@param p	const-lvalue reference to point_c value
	\~japanese	@return	(0, 0)と等しくなければtrueを返す
	\~english	@return	true if left operand is not equal to (0, 0)
	\~@code
	dxle::pointi p = { 0 , 0 };
	bool re = 0 != p;//false
	@endcode
	*/
	template <typename T, enable_if_t<std::is_arithmetic<T>::value, std::nullptr_t> = nullptr>
	bool operator !=(std::nullptr_t, const point_c<T>& p) DXLE_NOEXCEPT_OR_NOTHROW
	{
		return !static_cast<bool>(p);
	}

	/**
	@relates point_c
	\~japanese	@brief	二項演算子==のオーバーロード。厳密な比較が行われます
	\~english	@brief	Overload of binary operator ==. This operator compares strict difference
	\~japanese	@param l	point_cクラスオブジェクトへのconst-lvalue reference
	\~english	@param l	const-lvalue reference to point_c value
	\~japanese	@param r	point_cクラスオブジェクトへのconst-lvalue reference
	\~english	@param r	const-lvalue reference to point_c value
	\~japanese	@return	左辺と右辺が等しければtrueを返す
	\~english	@return	true if left operand is equal to right operand
	*/
	template <typename T, enable_if_t<std::is_arithmetic<T>::value, std::nullptr_t> = nullptr>
	bool operator ==(const point_c<T>& l, const point_c<T>& r) DXLE_NOEXCEPT_OR_NOTHROW { return !(l != r);	}

	/**
	@relates point_c
	\~japanese	@brief	二項演算子==のオーバーロード。厳密な比較が行われます
	\~english	@brief	Overload of binary operator ==. This operator compares strict difference
	\~japanese	@param p	point_cクラスオブジェクトへのconst-lvalue reference
	\~english	@param p	const-lvalue reference to point_c value
	\~japanese	@return	(0, 0)と等しければtrueを返す
	\~english	@return	true if left operand is equal to (0, 0)
	\~@code
	dxle::pointi p = { 0 , 0 };
	bool re = 0 == p;//true
	@endcode
	*/
	template <typename T, enable_if_t<std::is_arithmetic<T>::value, std::nullptr_t> = nullptr>
	bool operator ==(const point_c<T>& p, std::nullptr_t) DXLE_NOEXCEPT_OR_NOTHROW
	{
		return static_cast<bool>(p);
	}

	/**
	@relates point_c
	\~japanese	@brief	二項演算子==のオーバーロード。厳密な比較が行われます
	\~english	@brief	Overload of binary operator ==. This operator compares strict difference
	\~japanese	@param p	point_cクラスオブジェクトへのconst-lvalue reference
	\~english	@param p	const-lvalue reference to point_c value
	\~japanese	@return	(0, 0)と等しければtrueを返す
	\~english	@return	true if left operand is equal to (0, 0)
	\~@code
	dxle::pointi p = { 0 , 0 };
	bool re = 0 == p;//true
	@endcode
	*/
	template <typename T, enable_if_t<std::is_arithmetic<T>::value, std::nullptr_t> = nullptr>
	bool operator ==(std::nullptr_t, const point_c<T>& p) DXLE_NOEXCEPT_OR_NOTHROW
	{
		return static_cast<bool>(p);
	}

	namespace detail{
		template<typename T, bool is_signed = std::is_signed<T>::value> struct abs_helper {
			point_c<T> operator() (const point_c<T>& o) DXLE_NOEXCEPT_OR_NOTHROW { return point_c<T>(std::abs(o.x), std::abs(o.y)); }
		};
		template<typename T> struct abs_helper<T, false> {
			point_c<T> operator() (const point_c<T>& o) DXLE_NOEXCEPT_OR_NOTHROW { return o; }
		};
	}

	/**
	@relates point_c
	\~japanese	@brief	point_cの絶対値(ベクトルの絶対値ではないのでその場合はdistanceを使ってください)
	\~english	@brief	Absolute value of point_c(THIS IS NOT THE ABSOLUTE VALUE OF THE VECTOR! use distance instead.)
	\~japanese	@param o	point_cクラスオブジェクト
	\~english	@param o	point_c value
	\~japanese	@return	第一引数に指定した入力ストリームへのlvalue reference
	\~english	@return	The absolute value of o.
	\~@code
	const dxle::pointi p1 = { -2, 4 };
	const auto result = dxle::abs(p1);//(2, 4)
	@endcode
	*/
	template<typename T, enable_if_t<std::is_arithmetic<T>::value, std::nullptr_t> = nullptr>
	point_c<T> abs(const point_c<T>& o) DXLE_NOEXCEPT_OR_NOTHROW { return detail::abs_helper<T>()(o); }

	/**
	@relates point_c
	\~japanese	@brief	２つのpoint_cクラスオブジェクトをベクトルとして内積を計算する
	\~english	@brief	Computes a dot-product of two point_c value as vectors.
	\~japanese	@param p1	point_cクラスオブジェクト
	\~english	@param p1	point_c value
	\~japanese	@param p2	point_cクラスオブジェクト
	\~english	@param p2	point_c value
	\~japanese	@return	計算結果。戻り値の型は暗黙の型変換で得られるものです。
	\~english	@return	Computed result. return value's type is a result of Implicit conversions.
	*/
	template<typename T1, typename T2, enable_if_t<std::is_arithmetic<T1>::value && std::is_arithmetic<T2>::value, std::nullptr_t> = nullptr>
	auto dot(const point_c<T1>& p1, const point_c<T2>& p2) DXLE_NOEXCEPT_OR_NOTHROW -> decltype(p1.x * p2.x)
	{
		return p1.x * p2.x + p1.y * p2.y;
	}

	/**
	@relates point_c
	\~japanese	@brief	２つのpoint_cクラスオブジェクトをベクトルとして外積を計算する
	\~english	@brief	Computes a cross-product of two point_c value as vectors.
	\~japanese	@param p1	point_cクラスオブジェクト
	\~english	@param p1	point_c value
	\~japanese	@param p2	point_cクラスオブジェクト
	\~english	@param p2	point_c value
	\~japanese	@return	計算結果。
	\~english	@return	Computed result.
	*/
	template<typename T1, typename T2, enable_if_t<std::is_arithmetic<T1>::value && std::is_arithmetic<T2>::value, std::nullptr_t> = nullptr>
	double cross(const point_c<T1>& p1, const point_c<T2>& p2) DXLE_NOEXCEPT_OR_NOTHROW
	{
		return static_cast<double>(p1.x) * p2.y + static_cast<double>(p1.y) * p2.x;
	}
	namespace detail {
		template<typename T1, typename T2, enable_if_t<std::is_arithmetic<T1>::value && std::is_arithmetic<T2>::value, std::nullptr_t> = nullptr>
		auto safe_dist(T1 n1, T2 n2) DXLE_NOEXCEPT_OR_NOTHROW -> decltype(n1 - n2)
		{
			return (n1 < n2) ? n2 - n1 : n1 - n2;
		}
	}

	/**
	@relates point_c
	\~japanese	@brief	三平方の定理(std::hypot)にもとづき、２つのpoint_cクラスオブジェクトの距離を計算する
	\~english	@brief	Calculate the distance of the two point_c class object based on the Pythagorean theorem(std::hypot)
	\~japanese	@param p1	point_cクラスオブジェクト
	\~english	@param p1	point_c value
	\~japanese	@param p2	point_cクラスオブジェクト
	\~english	@param p2	point_c value
	\~japanese	@return	計算結果。
	\~english	@return	Computed result.
	*/
	template<typename T1, typename T2> auto distance(const point_c<T1>& p1, const point_c<T2>& p2) DXLE_NOEXCEPT_OR_NOTHROW -> decltype(p1.x + p2.x)
	{
		return std::hypot(detail::safe_dist(p1.x, p2.x), detail::safe_dist(p1.y, p2.y));
	}
	typedef point_c<int> pointi;
	typedef point_c<uint8_t> pointu8i;
	typedef point_c<int8_t> point8i;
	typedef point_c<double> pointd;
	typedef point_c<float> pointf;
};
#endif
