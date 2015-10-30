#ifndef INC_DXLIBEX_BASIC_TYPES_HPP
#define INC_DXLIBEX_BASIC_TYPES_HPP
//#pragma once
#include <utility>//std::pair
#include <type_traits>
#include "DxLibEx_Defines.h"

namespace DxLibEx {
	template<typename T, typename std::enable_if<std::is_arithmetic<T>::value, std::nullptr_t>::type = nullptr> class point_c
	{
	public:
		typedef T value_type;
		value_type x, y;
		point_c() DXLIBEX_NOEXCEPT : x(), y() {}
		point_c(std::nullptr_t) DXLIBEX_NOEXCEPT : x(), y() {}//for compare with 0. rf.)http://faithandbrave.hateblo.jp/entry/20081222/1229936810
		point_c(value_type x_, value_type y_) DXLIBEX_NOEXCEPT : x(x_), y(y_) {}

		//copy constructor
		point_c(const point_c<value_type>& o) DXLIBEX_NOEXCEPT : x(o.x), y(o.y) {}
		//move constructor
		point_c(point_c<value_type>&& o) DXLIBEX_NOEXCEPT : x(std::move(o.x)), y(std::move(o.y)) {}
		//copy assignment operator
		point_c& operator=(const point_c<value_type>& r) DXLIBEX_NOEXCEPT {
			this->x = r.x;
			this->y = r.y;
			return *this;
		}
		//move assignment operator
		point_c& operator=(point_c<value_type>&& r) DXLIBEX_NOEXCEPT {
			this->x = std::move(r.x);
			this->y = std::move(r.y);
			return *this;
		}

		//convert constructor
		point_c(const std::pair<value_type, value_type>& p) DXLIBEX_NOEXCEPT : x(p.first), y(p.second) {}
		point_c(std::pair<value_type, value_type>&& p) DXLIBEX_NOEXCEPT : x(std::move(p.first)), y(std::move(p.second)) {}
		explicit operator bool() const DXLIBEX_NOEXCEPT {
			return (0 != this->x) || (0 != this->y);
		}
	};
	template <typename T>
	bool operator ==(const point_c<T>& p, std::nullptr_t) DXLIBEX_NOEXCEPT {
		return static_cast<bool>(p);
	}
	template <typename T>
	bool operator ==(std::nullptr_t, const point_c<T>& p) DXLIBEX_NOEXCEPT {
		return static_cast<bool>(p);
	}
	////////////////////////////////////////////////////////////
	/// \relates point_c
	/// \brief Overload of unary operator -
	///
	/// \param right Vector to negate
	///
	/// \return Memberwise opposite of the vector
	///
	////////////////////////////////////////////////////////////
	template <typename T>
	point_c<T> operator -(const point_c<T>& r) DXLIBEX_NOEXCEPT {
		return point_c<T>(-r.x, -r.y);
	}

	template <typename T>
	inline const point_c<T>& operator +(const point_c<T>& r) DXLIBEX_NOEXCEPT { return r; }
	template <typename T>
	inline point_c<T>&& operator +(point_c<T>&& r) DXLIBEX_NOEXCEPT { return r; }

	////////////////////////////////////////////////////////////
	/// \relates point_c
	/// \brief Overload of binary operator +=
	///
	/// This operator performs a memberwise addition of both vectors,
	/// and assigns the result to \a left.
	///
	/// \param left  Left operand (a vector)
	/// \param right Right operand (a vector)
	///
	/// \return Reference to \a left
	///
	////////////////////////////////////////////////////////////
	template <typename T>
	point_c<T>& operator +=(point_c<T>& left, const point_c<T>& right);

	////////////////////////////////////////////////////////////
	/// \relates point_c
	/// \brief Overload of binary operator -=
	///
	/// This operator performs a memberwise subtraction of both vectors,
	/// and assigns the result to \a left.
	///
	/// \param left  Left operand (a vector)
	/// \param right Right operand (a vector)
	///
	/// \return Reference to \a left
	///
	////////////////////////////////////////////////////////////
	template <typename T>
	point_c<T>& operator -=(point_c<T>& left, const point_c<T>& right);

	////////////////////////////////////////////////////////////
	/// \relates point_c
	/// \brief Overload of binary operator +
	///
	/// \param left  Left operand (a vector)
	/// \param right Right operand (a vector)
	///
	/// \return Memberwise addition of both vectors
	///
	////////////////////////////////////////////////////////////
	template <typename T>
	point_c<T> operator +(const point_c<T>& left, const point_c<T>& right);

	////////////////////////////////////////////////////////////
	/// \relates point_c
	/// \brief Overload of binary operator -
	///
	/// \param left  Left operand (a vector)
	/// \param right Right operand (a vector)
	///
	/// \return Memberwise subtraction of both vectors
	///
	////////////////////////////////////////////////////////////
	template <typename T>
	point_c<T> operator -(const point_c<T>& left, const point_c<T>& right);

	////////////////////////////////////////////////////////////
	/// \relates point_c
	/// \brief Overload of binary operator *
	///
	/// \param left  Left operand (a vector)
	/// \param right Right operand (a scalar value)
	///
	/// \return Memberwise multiplication by \a right
	///
	////////////////////////////////////////////////////////////
	template <typename T>
	point_c<T> operator *(const point_c<T>& left, T right);

	////////////////////////////////////////////////////////////
	/// \relates point_c
	/// \brief Overload of binary operator *
	///
	/// \param left  Left operand (a scalar value)
	/// \param right Right operand (a vector)
	///
	/// \return Memberwise multiplication by \a left
	///
	////////////////////////////////////////////////////////////
	template <typename T>
	point_c<T> operator *(T left, const point_c<T>& right);

	////////////////////////////////////////////////////////////
	/// \relates point_c
	/// \brief Overload of binary operator *=
	///
	/// This operator performs a memberwise multiplication by \a right,
	/// and assigns the result to \a left.
	///
	/// \param left  Left operand (a vector)
	/// \param right Right operand (a scalar value)
	///
	/// \return Reference to \a left
	///
	////////////////////////////////////////////////////////////
	template <typename T>
	point_c<T>& operator *=(point_c<T>& left, T right);

	////////////////////////////////////////////////////////////
	/// \relates point_c
	/// \brief Overload of binary operator /
	///
	/// \param left  Left operand (a vector)
	/// \param right Right operand (a scalar value)
	///
	/// \return Memberwise division by \a right
	///
	////////////////////////////////////////////////////////////
	template <typename T>
	point_c<T> operator /(const point_c<T>& left, T right);

	////////////////////////////////////////////////////////////
	/// \relates point_c
	/// \brief Overload of binary operator /=
	///
	/// This operator performs a memberwise division by \a right,
	/// and assigns the result to \a left.
	///
	/// \param left  Left operand (a vector)
	/// \param right Right operand (a scalar value)
	///
	/// \return Reference to \a left
	///
	////////////////////////////////////////////////////////////
	template <typename T>
	point_c<T>& operator /=(point_c<T>& left, T right);

	////////////////////////////////////////////////////////////
	/// \relates point_c
	/// \brief Overload of binary operator ==
	///
	/// This operator compares strict equality between two vectors.
	///
	/// \param left  Left operand (a vector)
	/// \param right Right operand (a vector)
	///
	/// \return True if \a left is equal to \a right
	///
	////////////////////////////////////////////////////////////
	template <typename T>
	bool operator ==(const point_c<T>& left, const point_c<T>& right);

	////////////////////////////////////////////////////////////
	/// \relates point_c
	/// \brief Overload of binary operator !=
	///
	/// This operator compares strict difference between two vectors.
	///
	/// \param left  Left operand (a vector)
	/// \param right Right operand (a vector)
	///
	/// \return True if \a left is not equal to \a right
	///
	////////////////////////////////////////////////////////////
	template <typename T>
	bool operator !=(const point_c<T>& left, const point_c<T>& right);

};
#endif
