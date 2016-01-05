#ifndef DXLE_INC_BASIC_TYPES_ARTHMETIC_T_HPP_
#define DXLE_INC_BASIC_TYPES_ARTHMETIC_T_HPP_
#include "dxlibex/type_traits/first_enabled.hpp"
#include "dxlibex/type_traits/enable_if.hpp"
namespace dxle {
	//! for int8_t/uint8_t
	template<typename T, enable_if_t<std::is_arithmetic<T>::value, std::nullptr_t> = nullptr>
	using arithmetic_t = first_enabled_t <
		enable_if<1 != sizeof(T), T>,
		enable_if<std::is_signed<T>::value, int>,
		unsigned int
	>;
}
#endif //DXLE_INC_BASIC_TYPES_ARTHMETIC_T_HPP_
