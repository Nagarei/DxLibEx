/*=============================================================================
  Copyright (C) 2015-2016 DxLibEx project
  https://github.com/Nagarei/DxLibEx/

  Distributed under the Boost Software License, Version 1.0.
  (See http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#ifndef DXLE_INC_BASIC_TYPES_point3d3D_HPP_
#define DXLE_INC_BASIC_TYPES_point3d3D_HPP_
#include "dxlibex/config/no_min_max.h"
#include "dxlibex/type_traits/first_enabled.hpp"
#include "dxlibex/type_traits/enable_if.hpp"
#include "dxlibex/type_traits/is_representable.hpp"
#include "dxlibex/basic_types/arithmetic_t.hpp"
#include "dxlibex/basic_types/stdint.hpp"
#include "dxlibex/basic_types/distance_result_type_t.hpp"
#include "dxlibex/algorithm/safe_dist.hpp"
#include "dxlibex/math.hpp"
#include "dxlibex/cstdlib.hpp"
//#include "dxlibex/basic_types.hpp"//DO NOT REMOVE COMMENT-OUT to avoid redefine
#include <iostream>
#include <tuple>
#include <type_traits>
#include <algorithm>
#include <cmath>
#include <limits>
#include "dxlibex/config/defines.h"

namespace dxle {
	/**
	\~japanese	@brief	2次元座標(x, y)　テンプレートクラス。
	\~english	@brief	Template class for 3D points specified by its coordinates `x` and `y`.

	\~japanese	このクラスはstd::tupleと相互変換が可能です。また、内部型の異なるpoint3d_cクラス同士の変換はstatic_castを使用することで可能です(内部でも`static_cast`を使用します)
	上記のメンバーのほかに、以下の演算をサポートします
	\~english	An instance of the claess is interchangeable with std::tuple. There is also a cast operator
	to convert point3d coordinates to the specified type (using static_cast). Commonly, the conversion
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
	typedef point3d_c<int> point3di;
	typedef point3d_c<std::uint8_t> point3du8i;
	typedef point3d_c<std::int8_t> point3d8i;
	typedef point3d_c<double> point3dd;
	typedef point3d_c<float> point3df;
	@endcode
	\~ Example:
	@code
	dxle::point3df a(0.3f, 0.f), b(0.f, 0.4f);
	dxle::point3di pt = (a + b)*10.f;
	std::cout << pt << std::endl;
	@endcode
	*/
	template<typename T, enable_if_t<std::is_arithmetic<T>::value, nullptr_t> = nullptr>
	class point3d_c final
	{
	public:
		typedef T value_type;
		value_type x, y, z;
		DXLE_CONSTEXPR_CLASS point3d_c() DXLE_NOEXCEPT_OR_NOTHROW : x(), y(), z() {}
		DXLE_CONSTEXPR_CLASS point3d_c(value_type x_, value_type y_, value_type z_) DXLE_NOEXCEPT_OR_NOTHROW : x(x_), y(y_), z(z_) {}

		//copy constructor
		DXLE_CONSTEXPR_CLASS point3d_c(const point3d_c<value_type>& o) DXLE_NOEXCEPT_OR_NOTHROW : x(o.x), y(o.y), z(o.z) {}
		//move constructor
		DXLE_CONSTEXPR_CLASS point3d_c(point3d_c<value_type>&& o) DXLE_NOEXCEPT_OR_NOTHROW : x(std::move(o.x)), y(std::move(o.y)), z(std::move(o.z)) {}
		//copy assignment operator
		point3d_c& operator=(const point3d_c<value_type>& r) DXLE_NOEXCEPT_OR_NOTHROW
		{
			this->x = r.x;
			this->y = r.y;
			this->z = r.z;
			return *this;
		}
		//move assignment operator
		point3d_c& operator=(point3d_c<value_type>&& r) DXLE_NOEXCEPT_OR_NOTHROW
		{
			this->x = std::move(r.x);
			this->y = std::move(r.y);
			this->z = std::move(r.z);
			return *this;
		}


		DXLE_CONSTEXPR_CLASS explicit operator bool() const DXLE_NOEXCEPT_OR_NOTHROW {
			return (0 != this->x) || (0 != this->y) || (0 != this->z);
		}
		//!\~english conversion to another data type
		//!\~japanese 内部型の異なるpoint3d_cクラス同士の変換
		template<typename Tp2_> DXLE_CONSTEXPR_CLASS explicit operator point3d_c<Tp2_>() const DXLE_NOEXCEPT_OR_NOTHROW
		{
			return{ static_cast<Tp2_>(this->x), static_cast<Tp2_>(this->y), static_cast<Tp2_>(this->z) };
		}
		//!\~english conversion to std::tuple
		//!\~japanese std::tupleへの変換
		template<typename Tp2_> explicit operator std::tuple<Tp2_, Tp2_, Tp2_>() const DXLE_NOEXCEPT_OR_NOTHROW
		{
			return std::forward_as_tuple(static_cast<Tp2_>(this->x), static_cast<Tp2_>(this->y), static_cast<Tp2_>(this->z));
		}
	};
	//convert from std::tuple

	/**
	@relates point3d_c
	\~japanese	@brief	std::tupleからの変換
	\~english	@brief	conversion from std::tuple
	\~japanese	@param p	std::tupleオブジェクトへのconst-lvalue reference
	\~english	@param p	const-lvalue reference to std::tuple
	\~japanese	@return	point3d_cクラスオブジェクト
	\~english	@return	point3d_c value
	*/
	template<typename T> point3d_c<T> make_point3d_c(const std::tuple<T, T, T>& p) DXLE_NOEXCEPT_OR_NOTHROW
	{
		return { std::get<0>(p),  std::get<1>(p), std::get<2>(p) };
	}
	/**
	@relates point3d_c
	\~japanese	@brief	std::tupleからの変換
	\~english	@brief	conversion from std::tuple
	\~japanese	@param p	std::tupleオブジェクトへのrvalue reference
	\~english	@param p	rvalue reference to std::tuple
	\~japanese	@return	point3d_cクラスオブジェクト
	\~english	@return	point3d_c value
	*/
	template<typename T> point3d_c<T> make_point3d_c(std::tuple<T, T, T>&& p) DXLE_NOEXCEPT_OR_NOTHROW
	{
		return point3d_c<T>(std::move(std::get<0>(p)), std::move(std::get<1>(p)), std::move(std::get<2>(p)));
	}

	//ostream operator
	namespace detail {
		template<typename CharType, typename point3dType>
		void ostream_operator_helper(std::basic_ostream<CharType>& os, const CharType* str, const point3d_c<point3dType>& p)
		{
			using arithmetic_p = arithmetic_t<point3dType>;
			os << static_cast<arithmetic_p>(p.x) << str << static_cast<arithmetic_p>(p.y) << str << static_cast<arithmetic_p>(p.z);
		}
		template<typename CharType, typename point3dType>
		void istream_operator_helper(std::basic_istream<CharType>& is, point3d_c<point3dType>& p)
		{
			arithmetic_t<point3dType> x, y, z;
			is >> x;
			is.ignore((std::numeric_limits<std::streamsize>::max)(), ',');
			is >> y;
			is.ignore((std::numeric_limits<std::streamsize>::max)(), ',');
			is >> z;
			p.x = static_cast<point3dType>(x); p.y = static_cast<point3dType>(y); p.z = static_cast<point3dType>(z);
		}
	}
	/**
	@relates point3d_c
	\~japanese	@brief	出力ストリーム演算子
	\~english	@brief	ostream operator
	\~japanese	@param os	出力ストリームへのlvalue reference
	\~english	@param os	lvalue reference to ostream
	\~japanese	@param p	point3d_cクラスオブジェクトへのconst-lvalue reference
	\~english	@param p	const-lvalue reference to point3d_c
	\~japanese	@return	第一引数に指定した出力ストリームへのlvalue reference
	\~english	@return	lvalue reference to ostream whitch is specified at first argument
	*/
	template<typename T> std::ostream& operator<<(std::ostream& os, const point3d_c<T>& p)
	{
		dxle::detail::ostream_operator_helper<char, T>(os, ", ", p);
		return os;
	}
	/**
	@relates point3d_c
	\~japanese	@brief	出力ストリーム演算子
	\~english	@brief	ostream operator
	\~japanese	@param os	出力ストリームへのlvalue reference
	\~english	@param os	lvalue reference to ostream
	\~japanese	@param p	point3d_cクラスオブジェクトへのconst-lvalue reference
	\~english	@param p	const-lvalue reference to point3d_c
	\~japanese	@return	第一引数に指定した出力ストリームへのlvalue reference
	\~english	@return	lvalue reference to ostream whitch is specified at first argument
	*/
	template<typename T> std::wostream& operator<<(std::wostream& os, const point3d_c<T>& p)
	{
		dxle::detail::ostream_operator_helper<wchar_t, T>(os, L", ", p);
		return os;
	}
	/**
	@relates point3d_c
	\~japanese	@brief	入力ストリーム演算子
	\~english	@brief	istream operator
	\~japanese	@param is	入力ストリームへのlvalue reference
	\~english	@param is	lvalue reference to istream
	\~japanese	@param p	point3d_cクラスオブジェクトへのconst-lvalue reference
	\~english	@param p	const-lvalue reference to point3d_c
	\~japanese	@return	第一引数に指定した入力ストリームへのlvalue reference
	\~english	@return	lvalue reference to istream whitch is specified at first argument
	*/
	template<typename T> std::istream& operator>>(std::istream& is, point3d_c<T>& p)
	{
		dxle::detail::istream_operator_helper<char, T>(is, p);
		return is;
	}
	/**
	@relates point3d_c
	\~japanese	@brief	入力ストリーム演算子
	\~english	@brief	istream operator
	\~japanese	@param is	入力ストリームへのlvalue reference
	\~english	@param is	lvalue reference to istream
	\~japanese	@param p	point3d_cクラスオブジェクトへのconst-lvalue reference
	\~english	@param p	const-lvalue reference to point3d_c
	\~japanese	@return	第一引数に指定した入力ストリームへのlvalue reference
	\~english	@return	lvalue reference to istream whitch is specified at first argument
	*/
	template<typename T> std::wistream& operator>>(std::wistream& is, point3d_c<T>& p)
	{
		detail::istream_operator_helper<wchar_t, T>(is, p);
		return is;
	}

	/**
	@relates point3d_c
	\~japanese	@brief	単項演算子-のオーバーロード
	\~english	@brief	Overload of unary operator -
	\~japanese	@param r	point3d_cクラスオブジェクト
	\~english	@param r	point3d_c value to negate
	\~japanese	@return	符号を逆転させた結果
	\~english	@return	Memberwise opposite of the point3d_c value
	*/
	template <typename T>
	DXLE_CONSTEXPR_CLASS point3d_c<T> operator -(const point3d_c<T>& r) DXLE_NOEXCEPT_OR_NOTHROW
	{
		return { -r.x, -r.y, -r.z };
	}

	/**
	@relates point3d_c
	\~japanese	@brief	単項演算子+のオーバーロード
	\~english	@brief	Overload of unary operator +
	\~japanese	@param r	point3d_cクラスオブジェクトへのconst-lvalue reference
	\~english	@param r	const-lvalue reference to point3d_c value
	\~japanese	@return	第一引数へのconst-lvalue reference
	\~english	@return	const-lvalue reference to first argument
	*/
	template <typename T>
	DXLE_CONSTEXPR_CLASS inline const point3d_c<T>& operator +(const point3d_c<T>& r) DXLE_NOEXCEPT_OR_NOTHROW { return r; }

	/**
	@relates point3d_c
	\~japanese	@brief	単項演算子+のオーバーロード
	\~english	@brief	Overload of unary operator +
	\~japanese	@param r	point3d_cクラスオブジェクトへのrvalue reference
	\~english	@param r	rvalue reference to point3d_c value
	\~japanese	@return	第一引数へのrvalue reference
	\~english	@return	rvalue reference to first argument
	*/
	template <typename T>
	DXLE_CONSTEXPR_CLASS inline point3d_c<T>&& operator +(point3d_c<T>&& r) DXLE_NOEXCEPT_OR_NOTHROW { return std::move(r); }

	/**
	@relates point3d_c
	\~japanese	@brief	二項演算子+=のオーバーロード
	\~english	@brief	Overload of binary operator +=
	\~japanese	@param l	point3d_cクラスオブジェクトへのlvalue reference
	\~english	@param l	lvalue reference to point3d_c value
	\~japanese	@param r	point3d_cクラスオブジェクトへのconst-lvalue reference
	\~english	@param r	const-lvalue reference to point3d_c value
	\~japanese	@return	第一引数へのlvalue reference
	\~english	@return	lvalue reference to first argument
	*/
	template <typename T1, typename T2, enable_if_t<is_representable<T2, T1>::value, nullptr_t> = nullptr>
	point3d_c<T1>& operator +=(point3d_c<T1>& l, const point3d_c<T2>& r) DXLE_NOEXCEPT_OR_NOTHROW
	{
	    l.x += r.x;
	    l.y += r.y;
	    l.z += r.z;
	    return l;
	}

	/**
	@relates point3d_c
	\~japanese	@brief	二項演算子-=のオーバーロード
	\~english	@brief	Overload of binary operator -=
	\~japanese	@param l	point3d_cクラスオブジェクトへのlvalue reference
	\~english	@param l	lvalue reference to point3d_c value
	\~japanese	@param r	point3d_cクラスオブジェクトへのconst-lvalue reference
	\~english	@param r	const-lvalue reference to point3d_c value
	\~japanese	@return	第一引数へのlvalue reference
	\~english	@return	lvalue reference to first argument
	*/
	template <typename T1, typename T2, enable_if_t<is_representable<T2, T1>::value, nullptr_t> = nullptr>
	point3d_c<T1>& operator -=(point3d_c<T1>& l, const point3d_c<T2>& r) DXLE_NOEXCEPT_OR_NOTHROW
	{
	    l.x -= r.x;
	    l.y -= r.y;
	    l.z -= r.z;
	    return l;
	}

	/**
	@relates point3d_c
	\~japanese	@brief	二項演算子+のオーバーロード
	\~english	@brief	Overload of binary operator +
	\~japanese	@param l	point3d_cクラスオブジェクトへのconst-lvalue reference
	\~english	@param l	const-lvalue reference to point3d_c value
	\~japanese	@param r	point3d_cクラスオブジェクトへのconst-lvalue reference
	\~english	@param r	const-lvalue reference to point3d_c value
	\~japanese	@return	2つのpoint3d_cクラスオブジェクトの各メンバー同士の加算結果
	\~english	@return	Memberwise addition of both point3d_c value
	*/
	template <typename T1, typename T2>
	DXLE_CONSTEXPR_CLASS auto operator +(const point3d_c<T1>& l, const point3d_c<T2>& r) DXLE_NOEXCEPT_OR_NOTHROW
		->point3d_c<decltype(std::declval<std::remove_cv_t<T1>>() + std::declval<std::remove_cv_t<T2>>())>
	{
		return { l.x + r.x, l.y + r.y, l.z + r.z };
	}

	/**
	@relates point3d_c
	\~japanese	@brief	二項演算子-のオーバーロード
	\~english	@brief	Overload of binary operator -
	\~japanese	@param l	point3d_cクラスオブジェクトへのconst-lvalue reference
	\~english	@param l	const-lvalue reference to point3d_c value
	\~japanese	@param r	point3d_cクラスオブジェクトへのconst-lvalue reference
	\~english	@param r	const-lvalue reference to point3d_c value
	\~japanese	@return	2つのpoint3d_cクラスオブジェクトの各メンバー同士の減算結果
	\~english	@return	Memberwise subtraction of both point3d_c value
	*/
	template <typename T1, typename T2>
	DXLE_CONSTEXPR_CLASS auto operator -(const point3d_c<T1>& l, const point3d_c<T2>& r) DXLE_NOEXCEPT_OR_NOTHROW
		->point3d_c<decltype(std::declval<std::remove_cv_t<T1>>() - std::declval<std::remove_cv_t<T2>>())>
	{
		return { l.x - r.x, l.y - r.y, l.z - r.z };
	}

	/**
	@relates point3d_c
	\~japanese	@brief	二項演算子*のオーバーロード
	\~english	@brief	Overload of binary operator *
	\~japanese	@param l	point3d_cクラスオブジェクトへのconst-lvalue reference
	\~english	@param l	const-lvalue reference to point3d_c value
	\~japanese	@param r	整数型の値
	\~english	@param r	arithmetic value
	\~japanese	@return	point3d_cクラスオブジェクトの各メンバーに第二引数を乗じた結果
	\~english	@return	Memberwise multiplication by 2nd argument
	*/
	template <typename T1, typename T2, enable_if_t<std::is_arithmetic<T2>::value, nullptr_t> = nullptr>
	DXLE_CONSTEXPR_CLASS auto operator *(const point3d_c<T1>& l, T2 r) DXLE_NOEXCEPT_OR_NOTHROW
		->point3d_c<decltype(std::declval<std::remove_cv_t<T1>>() * std::declval<std::remove_cv_t<T2>>())>
	{
		return { l.x * r, l.y * r, l.z * r };
	}

	/**
	@relates point3d_c
	\~japanese	@brief	二項演算子*のオーバーロード
	\~english	@brief	Overload of binary operator *
	\~japanese	@param l	整数型の値
	\~english	@param l	arithmetic value
	\~japanese	@param r	point3d_cクラスオブジェクトへのconst-lvalue reference
	\~english	@param r	const-lvalue reference to point3d_c value
	\~japanese	@return	point3d_cクラスオブジェクトの各メンバーに第一引数を乗じた結果
	\~english	@return	Memberwise multiplication by 1st argument
	*/
	template <typename T1, typename T2, enable_if_t<std::is_arithmetic<T1>::value, nullptr_t> = nullptr>
	DXLE_CONSTEXPR_CLASS auto operator *(T1 l, const point3d_c<T2>& r) DXLE_NOEXCEPT_OR_NOTHROW
		->point3d_c<decltype(std::declval<std::remove_cv_t<T1>>() * std::declval<std::remove_cv_t<T2>>())>
	{
		return { l + r.x, l + r.y, l + r.z };
	}

	/**
	@relates point3d_c
	\~japanese	@brief	二項演算子*=のオーバーロード
	\~english	@brief	Overload of binary operator *=
	\~japanese	@param l	point3d_cクラスオブジェクトへのconst-lvalue reference
	\~english	@param l	const-lvalue reference to point3d_c value
	\~japanese	@param r	整数型の値
	\~english	@param r	arithmetic value
	\~japanese	@return	第一引数へのlvalue reference
	\~english	@return	lvalue reference to 1st argument
	*/
	template <typename T1, typename T2, enable_if_t<std::is_arithmetic<T2>::value && is_representable<T2, T1>::value, nullptr_t> = nullptr>
	point3d_c<T1>& operator *=(point3d_c<T1>& l, T2 r) DXLE_NOEXCEPT_OR_NOTHROW
	{
	    l.x *= r;
	    l.y *= r;
	    l.z *= r;
	    return l;
	}

	/**
	@relates point3d_c
	\~japanese	@brief	二項演算子/のオーバーロード
	\~english	@brief	Overload of binary operator /
	\~japanese	@param l	point3d_cクラスオブジェクトへのconst-lvalue reference
	\~english	@param l	const-lvalue reference to point3d_c value
	\~japanese	@param r	整数型の値
	\~english	@param r	arithmetic value
	\~japanese	@return	point3d_cクラスオブジェクトの各メンバーを第一引数で割った結果
	\~english	@return	Memberwise multiplication by 1st argument
	*/
	template <typename T1, typename T2, enable_if_t<std::is_arithmetic<T2>::value, nullptr_t> = nullptr>
	DXLE_CONSTEXPR_CLASS auto operator /(const point3d_c<T1>& l, T2 r) DXLE_NOEXCEPT_OR_NOTHROW
		->point3d_c<decltype(std::declval<std::remove_cv_t<T1>>() - std::declval<std::remove_cv_t<T2>>())>
	{
		return { l.x / r, l.y / r, l.z / r};
	}

	/**
	@relates point3d_c
	\~japanese	@brief	二項演算子/=のオーバーロード
	\~english	@brief	Overload of binary operator /=
	\~japanese	@param l	point3d_cクラスオブジェクトへのconst-lvalue reference
	\~english	@param l	const-lvalue reference to point3d_c value
	\~japanese	@param r	整数型の値
	\~english	@param r	arithmetic value
	\~japanese	@return	第一引数へのlvalue reference
	\~english	@return	lvalue reference to 1st argument
	*/
	template <typename T1, typename T2, enable_if_t<std::is_arithmetic<T2>::value && is_representable<T2, T1>::value, nullptr_t> = nullptr>
	point3d_c<T1>& operator /=(point3d_c<T1>& l, T2 r) DXLE_NOEXCEPT_OR_NOTHROW 
	{
	    l.x /= r;
	    l.y /= r;
	    l.z /= r;
	    return l;
	}

	/**
	@relates point3d_c
	\~japanese	@brief	二項演算子!=のオーバーロード。厳密な比較が行われます
	\~english	@brief	Overload of binary operator !=. This operator compares strict difference
	\~japanese	@param l	point3d_cクラスオブジェクトへのconst-lvalue reference
	\~english	@param l	const-lvalue reference to point3d_c value
	\~japanese	@param r	point3d_cクラスオブジェクトへのconst-lvalue reference
	\~english	@param r	const-lvalue reference to point3d_c value
	\~japanese	@return	左辺と右辺が等しくなければtrueを返す
	\~english	@return	true if left operand is not equal to right operand
	*/
	template <typename T, enable_if_t<std::is_arithmetic<T>::value, nullptr_t> = nullptr>
	DXLE_CONSTEXPR_CLASS bool operator !=(const point3d_c<T>& l, const point3d_c<T>& r) DXLE_NOEXCEPT_OR_NOTHROW
	{
		return (l.x != r.x) || (l.y != r.y) || (l.z != r.z);
	}

	/**
	@relates point3d_c
	\~japanese	@brief	二項演算子!=のオーバーロード。厳密な比較が行われます
	\~english	@brief	Overload of binary operator !=. This operator compares strict difference
	\~japanese	@param p	point3d_cクラスオブジェクトへのconst-lvalue reference
	\~english	@param p	const-lvalue reference to point3d_c value
	\~japanese	@return	(0, 0)と等しくなければtrueを返す
	\~english	@return	true if left operand is not equal to (0, 0)
	\~@code
	dxle::point3di p = { 0 , 0 };
	bool re = p != 0;//false
	@endcode
	*/
	template <typename T, enable_if_t<std::is_arithmetic<T>::value, nullptr_t> = nullptr>
	DXLE_CONSTEXPR_CLASS bool operator !=(const point3d_c<T>& p, nullptr_t) DXLE_NOEXCEPT_OR_NOTHROW
	{
		return !static_cast<bool>(p);
	}

	/**
	@relates point3d_c
	\~japanese	@brief	二項演算子!=のオーバーロード。厳密な比較が行われます
	\~english	@brief	Overload of binary operator !=. This operator compares strict difference
	\~japanese	@param p	point3d_cクラスオブジェクトへのconst-lvalue reference
	\~english	@param p	const-lvalue reference to point3d_c value
	\~japanese	@return	(0, 0)と等しくなければtrueを返す
	\~english	@return	true if left operand is not equal to (0, 0)
	\~@code
	dxle::point3di p = { 0 , 0 };
	bool re = 0 != p;//false
	@endcode
	*/
	template <typename T, enable_if_t<std::is_arithmetic<T>::value, nullptr_t> = nullptr>
	DXLE_CONSTEXPR_CLASS bool operator !=(nullptr_t, const point3d_c<T>& p) DXLE_NOEXCEPT_OR_NOTHROW
	{
		return !static_cast<bool>(p);
	}

	/**
	@relates point3d_c
	\~japanese	@brief	二項演算子==のオーバーロード。厳密な比較が行われます
	\~english	@brief	Overload of binary operator ==. This operator compares strict difference
	\~japanese	@param l	point3d_cクラスオブジェクトへのconst-lvalue reference
	\~english	@param l	const-lvalue reference to point3d_c value
	\~japanese	@param r	point3d_cクラスオブジェクトへのconst-lvalue reference
	\~english	@param r	const-lvalue reference to point3d_c value
	\~japanese	@return	左辺と右辺が等しければtrueを返す
	\~english	@return	true if left operand is equal to right operand
	*/
	template <typename T, enable_if_t<std::is_arithmetic<T>::value, nullptr_t> = nullptr>
	DXLE_CONSTEXPR_CLASS bool operator ==(const point3d_c<T>& l, const point3d_c<T>& r) DXLE_NOEXCEPT_OR_NOTHROW { return !(l != r);	}

	/**
	@relates point3d_c
	\~japanese	@brief	二項演算子==のオーバーロード。厳密な比較が行われます
	\~english	@brief	Overload of binary operator ==. This operator compares strict difference
	\~japanese	@param p	point3d_cクラスオブジェクトへのconst-lvalue reference
	\~english	@param p	const-lvalue reference to point3d_c value
	\~japanese	@return	(0, 0)と等しければtrueを返す
	\~english	@return	true if left operand is equal to (0, 0)
	\~@code
	dxle::point3di p = { 0 , 0 };
	bool re = 0 == p;//true
	@endcode
	*/
	template <typename T, enable_if_t<std::is_arithmetic<T>::value, nullptr_t> = nullptr>
	DXLE_CONSTEXPR_CLASS bool operator ==(const point3d_c<T>& p, nullptr_t) DXLE_NOEXCEPT_OR_NOTHROW
	{
		return static_cast<bool>(p);
	}

	/**
	@relates point3d_c
	\~japanese	@brief	二項演算子==のオーバーロード。厳密な比較が行われます
	\~english	@brief	Overload of binary operator ==. This operator compares strict difference
	\~japanese	@param p	point3d_cクラスオブジェクトへのconst-lvalue reference
	\~english	@param p	const-lvalue reference to point3d_c value
	\~japanese	@return	(0, 0)と等しければtrueを返す
	\~english	@return	true if left operand is equal to (0, 0)
	\~@code
	dxle::point3di p = { 0 , 0 };
	bool re = 0 == p;//true
	@endcode
	*/
	template <typename T, enable_if_t<std::is_arithmetic<T>::value, nullptr_t> = nullptr>
	DXLE_CONSTEXPR_CLASS bool operator ==(nullptr_t, const point3d_c<T>& p) DXLE_NOEXCEPT_OR_NOTHROW
	{
		return static_cast<bool>(p);
	}

	/**
	@relates point3d_c
	\~japanese	@brief	point3d_cの絶対値(ベクトルの絶対値ではないのでその場合はdistanceを使ってください)
	\~english	@brief	Absolute value of point3d_c(THIS IS NOT THE ABSOLUTE VALUE OF THE VECTOR! use distance instead.)
	\~japanese	@param o	point3d_cクラスオブジェクト
	\~english	@param o	point3d_c value
	\~japanese	@return	第一引数に指定した入力ストリームへのlvalue reference
	\~english	@return	The absolute value of o.
	\~@code
	const dxle::point3di p1 = { -2, 4 };
	const auto result = dxle::abs(p1);//(2, 4)
	@endcode
	*/
	template<typename T, enable_if_t<std::is_arithmetic<T>::value, nullptr_t> = nullptr>
	DXLE_CONSTEXPR_CLASS point3d_c<T> abs(const point3d_c<T>& o) DXLE_NOEXCEPT_OR_NOTHROW { return{ dxle::abs(o.x), dxle::abs(o.y), dxle::abs(o.z) }; }

	/**
	@relates point3d_c
	\~japanese	@brief	２つのpoint3d_cクラスオブジェクトをベクトルとして内積を計算する
	\~english	@brief	Computes a dot-product of two point3d_c value as vectors.
	\~japanese	@param p1	point3d_cクラスオブジェクト
	\~english	@param p1	point3d_c value
	\~japanese	@param p2	point3d_cクラスオブジェクト
	\~english	@param p2	point3d_c value
	\~japanese	@return	計算結果。戻り値の型は暗黙の型変換で得られるものです。
	\~english	@return	Computed result. return value's type is a result of Implicit conversions.
	*/
	template<typename T1, typename T2, enable_if_t<std::is_arithmetic<T1>::value && std::is_arithmetic<T2>::value, nullptr_t> = nullptr>
	DXLE_CONSTEXPR_CLASS auto dot(const point3d_c<T1>& p1, const point3d_c<T2>& p2) DXLE_NOEXCEPT_OR_NOTHROW
		-> decltype(std::declval<std::remove_cv_t<T1>>() * std::declval<std::remove_cv_t<T2>>())
	{
		return p1.x * p2.x + p1.y * p2.y + p1.z * p2.z;
	}

	/**
	@relates point3d_c
	\~japanese	@brief	２つのpoint3d_cクラスオブジェクトをベクトルとして外積を計算する
	\~english	@brief	Computes a cross-product of two point3d_c value as vectors.
	\~japanese	@param p1	point3d_cクラスオブジェクト
	\~english	@param p1	point3d_c value
	\~japanese	@param p2	point3d_cクラスオブジェクト
	\~english	@param p2	point3d_c value
	\~japanese	@return	計算結果。
	\~english	@return	Computed result.
	*/
	template<typename T1, typename T2, enable_if_t<std::is_arithmetic<T1>::value && std::is_arithmetic<T2>::value, nullptr_t> = nullptr>
	DXLE_CONSTEXPR_CLASS auto cross(const point3d_c<T1>& p1, const point3d_c<T2>& p2) DXLE_NOEXCEPT_OR_NOTHROW
		->point3d_c<decltype(std::declval<std::remove_cv_t<T1>>() * std::declval<std::remove_cv_t<T2>>())>
	{
		//a＝（a1,a2,a3）、 b＝（b1,b2,b3）としたとき、（a2b3-a3b2, a3b1-a1b3, a1b2-a2b1）
		return { p1.y * p2.z - p1.z * p2.y, p1.z * p2.x - p1.x * p2.z, p1.x * p2.y - p1.y * p2.x };
	}

	/**
	@relates point3d_c
	\~japanese	@brief	三平方の定理(std::hypot)にもとづき、２つのpoint3d_cクラスオブジェクトの距離を計算する
	\~english	@brief	Calculate the distance of the two point3d_c class object based on the Pythagorean theorem(std::hypot)
	\~japanese	@param p1	point3d_cクラスオブジェクト
	\~english	@param p1	point3d_c value
	\~japanese	@param p2	point3d_cクラスオブジェクト
	\~english	@param p2	point3d_c value
	\~japanese	@return	計算結果。
	\~english	@return	Computed result.
	*/
	template<typename T1, typename T2> 
	distance_result_type_t<T1, T2> distance(const point3d_c<T1>& p1, const point3d_c<T2>& p2) DXLE_NOEXCEPT_OR_NOTHROW
	{
		return std::hypot(safe_dist(p1.x, p2.x), std::hypot(safe_dist(p1.y, p2.y), safe_dist(p1.z, p2.z)));
	}
	typedef point3d_c<int> point3di;
	typedef point3d_c<std::uint8_t> point3du8i;
	typedef point3d_c<std::int8_t> point3d8i;
	typedef point3d_c<double> point3dd;
	typedef point3d_c<float> point3df;
};
#endif //DXLE_INC_BASIC_TYPES_point3d3D_HPP_
