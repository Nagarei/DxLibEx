#ifndef INC_DXLIBEX_BASIC_TYPES_HPP
#define INC_DXLIBEX_BASIC_TYPES_HPP
#pragma once
#include <utility>//std::pair
#include <type_trais.h>

namespace DxLibEx{
	template<typename T, std::enable_if<std::is_arithmetic<T>::value, std::nullptr_t>::type> class point_c
	{
	public:
		typedef T value_type;
		point_c() noexcept;
		point_c(value_type x, value_type y) noexcept;

		//copy constructor
		explicit point_c(const point_c<value_type>& o) noexcept;
		//move constructor
		explicit point_c(point_c<value_type>&& o) noexcept;
		//copy assignment operator
		point_c& operator=(const point_c<value_type>& r) noexcept;
		//move assignment operator
		point_c& operator=(point_c<value_type>&& r) noexcept;

		//convert constructor
		point_c(const std::pair<value_type, value_type>& p) noexcept;
		point_c(std::pair<value_type, value_type>&& p) noexcept;

	};
};
#endif
