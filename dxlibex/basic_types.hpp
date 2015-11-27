#ifndef DXLE_INC_BASIC_TYPES_HPP_
#define DXLE_INC_BASIC_TYPES_HPP_
//#pragma once
#include <iostream>//ostream
#include <utility>//std::pair
#include <type_traits>
#include <cstdint>
#include <cmath>
#include <limits>
#include "config/defines.h"

namespace dxle {
	/** 
	@brief	\~japanese 2次元座標(x, y)　テンプレートクラス。
			\~english Template class for 2D points specified by its coordinates `x` and `y`.

	\~english	An instance of the claess is interchangeable with std::pair. There is also a cast operator
				to convert point coordinates to the specified type (using static_cast). Commonly, the conversion
				uses this operation for each of the coordinates. Besides the class members listed in the
				declaration above, the following operations on points are implemented:
	\~japanese	このクラスはstd::pairと相互変換が可能です。また、内部型の異なるpoint_cクラス同士の変換は`static_cast`を使用することで可能です(内部でも`static_cast`を使用します)
				上記のメンバーのほかに、以下の演算をサポートします
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
	template<typename T, typename std::enable_if<std::is_arithmetic<T>::value, std::nullptr_t>::type = nullptr>
	class point_c
	{
	public:
		typedef T value_type;
		value_type x, y;
		point_c() DXLE_NOEXCEPT_OR_NOTHROW : x(), y() {}
		//point_c(std::nullptr_t) DXLE_NOEXCEPT_OR_NOTHROW : x(), y() {}
		explicit point_c(value_type x_) DXLE_NOEXCEPT_OR_NOTHROW : x(x_), y() {}
		point_c(value_type x_, value_type y_) DXLE_NOEXCEPT_OR_NOTHROW : x(x_), y(y_) {}

		//copy constructor
		point_c(const point_c<value_type>& o) DXLE_NOEXCEPT_OR_NOTHROW : x(o.x), y(o.y) {}
		//move constructor
		point_c(point_c<value_type>&& o) DXLE_NOEXCEPT_OR_NOTHROW : x(std::move(o.x)), y(std::move(o.y)) {}
		//copy assignment operator
		point_c& operator=(const point_c<value_type>& r) DXLE_NOEXCEPT_OR_NOTHROW {
			this->x = r.x;
			this->y = r.y;
			return *this;
		}
		//move assignment operator
		point_c& operator=(point_c<value_type>&& r) DXLE_NOEXCEPT_OR_NOTHROW {
			this->x = std::move(r.x);
			this->y = std::move(r.y);
			return *this;
		}


		explicit operator bool() const DXLE_NOEXCEPT_OR_NOTHROW {
			return (0 != this->x) || (0 != this->y);
		}
		//!\~english conversion to another data type
		//!\~japanese 内部型の異なるpoint_cクラス同士の変換
		template<typename _Tp2> explicit operator point_c<_Tp2>() const DXLE_NOEXCEPT_OR_NOTHROW {
			return point_c<_Tp2>(static_cast<_Tp2>(this->x), static_cast<_Tp2>(this->y));
		}
		//!\~english conversion to std::pair
		//!\~japanese std::pairへの変換
		template<typename _Tp2> explicit operator std::pair<_Tp2, _Tp2>() const DXLE_NOEXCEPT_OR_NOTHROW {
			return std::pair<_Tp2, _Tp2>(static_cast<_Tp2>(this->x), static_cast<_Tp2>(this->y));
		}
	};
	//convert from std::pair

	//!\~english conversion from std::pair
	//!\~japanese std::pairからの変換
	template<typename T> point_c<T> make_point_c(const std::pair<T, T>& p) DXLE_NOEXCEPT_OR_NOTHROW {
		return point_c<T>(p.first, p.second);
	}
	//!\~english conversion from std::pair
	//!\~japanese std::pairからの変換
	template<typename T> point_c<T> make_point_c(std::pair<T, T>&& p) DXLE_NOEXCEPT_OR_NOTHROW {
		return point_c<T>(std::move(p.first), std::move(p.second));
	}

	//ostream operator
	namespace detail {
		template<typename T, bool is_arithmetical = std::is_arithmetic<T>::value, size_t s = sizeof(T), bool is_s = std::is_signed<T>::value> struct ToArithmetic;
		template<typename T, size_t s, bool is_s> struct ToArithmetic<T, true, s, is_s> {
			typedef T type;
		};
		template<typename T> struct ToArithmetic<T, true, 1, false> {
			typedef unsigned int type;
		};
		template<typename T> struct ToArithmetic<T, true, 1, true> {
			typedef int type;
		};
		template<typename CharType, typename PointType>
		struct ostream_operator_helper {
			void operator()(std::basic_ostream<CharType>& os, const CharType* str, const point_c<PointType>& p) {
				typedef typename ToArithmetic<PointType>::type Arithmetic;
				os << static_cast<Arithmetic>(p.x) << str << static_cast<Arithmetic>(p.y);
			}
		};
		template<typename CharType, typename PointType>
		struct istream_operator_helper {
			void operator()(std::basic_istream<CharType>& is, point_c<PointType>& p) {
				typedef typename ToArithmetic<PointType>::type Arithmetic;
				Arithmetic x, y;
				is >> x;
				is.ignore((std::numeric_limits<std::streamsize>::max)(), ',');
				is >> y;
				p.x = static_cast<PointType>(x); p.y = static_cast<PointType>(y);
			}
		};
	}
	template<typename T> std::ostream& operator<<(std::ostream& os, const point_c<T>& p) {
		detail::ostream_operator_helper<char, T>()(os, ", ", p);
		return os;
	}
	template<typename T> std::wostream& operator<<(std::wostream& os, const point_c<T>& p) {
		detail::ostream_operator_helper<wchar_t, T>()(os, L", ", p);
		return os;
	}
	template<typename T> std::istream& operator>>(std::istream& is, point_c<T>& p) {
		detail::istream_operator_helper<char, T>()(is, p);
		return is;
	}
	template<typename T> std::wistream& operator>>(std::wistream& is, point_c<T>& p) {
		detail::istream_operator_helper<wchar_t, T>()(is, p);
		return is;
	}

	template <typename T>
	bool operator ==(const point_c<T>& p, std::nullptr_t) DXLE_NOEXCEPT_OR_NOTHROW {
		return static_cast<bool>(p);
	}
	template <typename T>
	bool operator ==(std::nullptr_t, const point_c<T>& p) DXLE_NOEXCEPT_OR_NOTHROW {
		return static_cast<bool>(p);
	}
	////////////////////////////////////////////////////////////
	/// \relates point_c
	/// \brief Overload of unary operator -
	///
	/// \param r Vector to negate
	///
	/// \return Memberwise opposite of the vector
	///
	////////////////////////////////////////////////////////////
	template <typename T>
	point_c<T> operator -(const point_c<T>& r) DXLE_NOEXCEPT_OR_NOTHROW {
		return point_c<T>(-r.x, -r.y);
	}

	template <typename T>
	inline const point_c<T>& operator +(const point_c<T>& r) DXLE_NOEXCEPT_OR_NOTHROW { return r; }
	template <typename T>
	inline point_c<T>&& operator +(point_c<T>&& r) DXLE_NOEXCEPT_OR_NOTHROW { return r; }

	////////////////////////////////////////////////////////////
	/// \relates point_c
	/// \brief Overload of binary operator +=
	///
	/// This operator performs a memberwise addition of both vectors,
	/// and assigns the result to \a l.
	///
	/// \param l  L operand (a vector)
	/// \param r Right operand (a vector)
	///
	/// \return Reference to \a l
	///
	////////////////////////////////////////////////////////////
	template <typename T>
	point_c<T>& operator +=(point_c<T>& l, const point_c<T>& r){
	    l.x += r.x;
	    l.y += r.y;
	    return l;
	}

	////////////////////////////////////////////////////////////
	/// \relates point_c
	/// \brief Overload of binary operator -=
	///
	/// This operator performs a memberwise subtraction of both vectors,
	/// and assigns the result to \a l.
	///
	/// \param l  L operand (a vector)
	/// \param r Right operand (a vector)
	///
	/// \return Reference to \a l
	///
	////////////////////////////////////////////////////////////
	template <typename T>
	point_c<T>& operator -=(point_c<T>& l, const point_c<T>& r){
	    l.x -= r.x;
	    l.y -= r.y;
	    return l;
	}

	////////////////////////////////////////////////////////////
	/// \relates point_c
	/// \brief Overload of binary operator +
	///
	/// \param l  L operand (a vector)
	/// \param r Right operand (a vector)
	///
	/// \return Memberwise addition of both vectors
	///
	////////////////////////////////////////////////////////////
	template <typename T>
	point_c<T> operator +(const point_c<T>& l, const point_c<T>& r){
		return point_c<T>(l.x + r.x, l.y + r.y);
	}

	////////////////////////////////////////////////////////////
	/// \relates point_c
	/// \brief Overload of binary operator -
	///
	/// \param l  L operand (a vector)
	/// \param r Right operand (a vector)
	///
	/// \return Memberwise subtraction of both vectors
	///
	////////////////////////////////////////////////////////////
	template <typename T>
	point_c<T> operator -(const point_c<T>& l, const point_c<T>& r){
		return point_c<T>(l.x - r.x, l.y - r.y);
	}

	////////////////////////////////////////////////////////////
	/// \relates point_c
	/// \brief Overload of binary operator *
	///
	/// \param l  L operand (a vector)
	/// \param r Right operand (a scalar value)
	///
	/// \return Memberwise multiplication by \a r
	///
	////////////////////////////////////////////////////////////
	template <typename T>
	point_c<T> operator *(const point_c<T>& l, T r){
		return point_c<T>(l.x * r, l.y * r);
	}

	////////////////////////////////////////////////////////////
	/// \relates point_c
	/// \brief Overload of binary operator *
	///
	/// \param l  L operand (a scalar value)
	/// \param r Right operand (a vector)
	///
	/// \return Memberwise multiplication by \a l
	///
	////////////////////////////////////////////////////////////
	template <typename T>
	point_c<T> operator *(T l, const point_c<T>& r){
		return point_c<T>(l + r.x, l + r.y);
	}

	////////////////////////////////////////////////////////////
	/// \relates point_c
	/// \brief Overload of binary operator *=
	///
	/// This operator performs a memberwise multiplication by \a r,
	/// and assigns the result to \a l.
	///
	/// \param l  L operand (a vector)
	/// \param r Right operand (a scalar value)
	///
	/// \return Reference to \a l
	///
	////////////////////////////////////////////////////////////
	template <typename T>
	point_c<T>& operator *=(point_c<T>& l, T r){
	    l.x *= r;
	    l.y *= r;
	    return l;
	}

	////////////////////////////////////////////////////////////
	/// \relates point_c
	/// \brief Overload of binary operator /
	///
	/// \param l  L operand (a vector)
	/// \param r Right operand (a scalar value)
	///
	/// \return Memberwise division by \a r
	///
	////////////////////////////////////////////////////////////
	template <typename T>
	point_c<T> operator /(const point_c<T>& l, T r){
		return point_c<T>(l.x * r, l.y * r);
	}

	////////////////////////////////////////////////////////////
	/// \relates point_c
	/// \brief Overload of binary operator /=
	///
	/// This operator performs a memberwise division by \a r,
	/// and assigns the result to \a l.
	///
	/// \param l  L operand (a vector)
	/// \param r Right operand (a scalar value)
	///
	/// \return Reference to \a l
	///
	////////////////////////////////////////////////////////////
	template <typename T>
	point_c<T>& operator /=(point_c<T>& l, T r){
	    l.x /= r;
	    l.y /= r;
	    return l;
	}

	////////////////////////////////////////////////////////////
	/// \relates point_c
	/// \brief Overload of binary operator !=
	///
	/// This operator compares strict difference between two vectors.
	///
	/// \param l  L operand (a vector)
	/// \param r Right operand (a vector)
	///
	/// \return True if \a l is not equal to \a r
	///
	////////////////////////////////////////////////////////////
	template <typename T>
	bool operator !=(const point_c<T>& l, const point_c<T>& r){
		return (l.x != r.x) || (l.y != r.y);
	}

	////////////////////////////////////////////////////////////
	/// \relates point_c
	/// \brief Overload of binary operator ==
	///
	/// This operator compares strict equality between two vectors.
	///
	/// \param l  L operand (a vector)
	/// \param r Right operand (a vector)
	///
	/// \return True if \a l is equal to \a r
	///
	////////////////////////////////////////////////////////////
	template <typename T>
	bool operator ==(const point_c<T>& l, const point_c<T>& r){
		return !(l != r);
	}

	namespace detail{
		template<typename T, bool is_signed = std::is_signed<T>::value>struct abs_helper{
			point_c<T> operator() (const point_c<T>& o){ return point_c<T>(std::abs(o.x), std::abs(o.y)); }
		};
		template<typename T> struct abs_helper<T, false> {
			point_c<T> operator() (const point_c<T>& o){ return o; }
		};
	}
	template<typename T> point_c<T> abs(const point_c<T>& o) { return detail::abs_helper<T>()(o); }

	typedef point_c<int> pointi;
	typedef point_c<uint8_t> pointu8i;
	typedef point_c<int8_t> point8i;
	typedef point_c<double> pointd;
	typedef point_c<float> pointf;
};
#endif
