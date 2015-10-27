#ifndef INC_DXLIBEX_BASIC_TYPES_HPP
#define INC_DXLIBEX_BASIC_TYPES_HPP
//#pragma once
#include <utility>//std::pair
#include <type_traits>
#include "DxLibEx_Defines.h"

namespace DxLibEx{
	template<typename T, typename = typename std::enable_if<std::is_arithmetic<T>::value>::type> class point_c
	{
	public:
		typedef T value_type;
		point_c() DXLIBEX_NOEXCEPT;
		point_c(value_type x, value_type y) DXLIBEX_NOEXCEPT;

		//copy constructor
		explicit point_c(const point_c<value_type>& o) DXLIBEX_NOEXCEPT;
		//move constructor
		explicit point_c(point_c<value_type>&& o) DXLIBEX_NOEXCEPT;
		//copy assignment operator
		point_c& operator=(const point_c<value_type>& r) DXLIBEX_NOEXCEPT;
		//move assignment operator
		point_c& operator=(point_c<value_type>&& r) DXLIBEX_NOEXCEPT;

		//convert constructor
		point_c(const std::pair<value_type, value_type>& p) DXLIBEX_NOEXCEPT;
		point_c(std::pair<value_type, value_type>&& p) DXLIBEX_NOEXCEPT;
	};
};
#endif
