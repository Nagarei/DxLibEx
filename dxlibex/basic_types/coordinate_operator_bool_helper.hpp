#ifndef DXLE_INC_BASIC_TYPES_COORDINATE_OPERATOR_BOOL_HELPER_HPP_
#define DXLE_INC_BASIC_TYPES_COORDINATE_OPERATOR_BOOL_HELPER_HPP_
#include "dxlibex/type_traits/is_castable.hpp"
#include "dxlibex/type_traits/is_well_format.hpp"
#include "dxlibex/type_traits/enable_if.hpp"
#include "dxlibex/config/defines.h"
namespace dxle {
	namespace detail {
		//operator bool
		//1. operator bool
		//2. operator != (nullptr)
		//3. default constector + operator !=

		template <
			typename T,
			bool has_operator_bool = !std::is_scalar<T>::value && is_castable<T, bool>::value,
			bool has_operator_notequal_to_zero = has_operator_notequal_to_zero<const T>::value,
			bool is_compareable_with_default_ctor = std::is_default_constructible<T>::value && has_operator_notequal_to_this<T>::value
		>
		struct operator_bool_helper_impl;
		//1. operator bool
		template<typename T, bool has_operator_notequal_to_zero, bool is_compareable_with_default_ctor>
		struct operator_bool_helper_impl<T, true, has_operator_notequal_to_zero, is_compareable_with_default_ctor> {
			DXLE_CONSTEXPR bool operator()(const T& first, const T& second) const DXLE_NOEXCEPT_IF_EXPR(static_cast<bool>(first)) {
				return static_cast<bool>(first) || static_cast<bool>(second);
			}
			DXLE_CONSTEXPR bool operator()(const T& first, const T& second, const T& third) const DXLE_NOEXCEPT_IF_EXPR(static_cast<bool>(first)) {
				return static_cast<bool>(first) || static_cast<bool>(second) || static_cast<bool>(third);
			}
		};
		//2. operator != (nullptr)
		template<typename T, bool is_compareable_with_default_ctor>
		struct operator_bool_helper_impl<T, false, true, is_compareable_with_default_ctor> {
			DXLE_CONSTEXPR bool operator()(const T& first, const T& second) const DXLE_NOEXCEPT_IF_EXPR(first != 0) {
				return first != 0 || second != 0;
			}
			DXLE_CONSTEXPR bool operator()(const T& first, const T& second, const T& third) const DXLE_NOEXCEPT_IF_EXPR(first != 0) {
				return first != 0 || second != 0 || third != 0;
			}
		};
		//3. default constector + operator !=
		template<typename T>
		struct operator_bool_helper_impl<T, false, false, true> {
			DXLE_CONSTEXPR bool operator()(const T& first, const T& second) const DXLE_NOEXCEPT_IF_EXPR(first != T{}) {
				return first != T{} || second != T{};
			}
			DXLE_CONSTEXPR bool operator()(const T& first, const T& second, const T& third) const DXLE_NOEXCEPT_IF_EXPR(first != T{}) {
				return first != T{} || second != T{} || third != T{};
			}
		};

		template<typename T, enable_if_t<std::is_arithmetic<T>::value, nullptr_t> = nullptr>
		DXLE_CONSTEXPR inline bool operator_bool_helper(const T& first, const T& second)
			DXLE_NOEXCEPT_IF_EXPR(dxle::detail::operator_bool_helper_impl<T>()(first, second))
		{
			return dxle::detail::operator_bool_helper_impl<T>()(first, second);
		}
		template<typename T, enable_if_t<std::is_arithmetic<T>::value, nullptr_t> = nullptr>
		DXLE_CONSTEXPR inline bool operator_bool_helper(const T& first, const T& second, const T& third)
			DXLE_NOEXCEPT_IF_EXPR(dxle::detail::operator_bool_helper_impl<T>()(first, second, third))
		{
			return dxle::detail::operator_bool_helper_impl<T>()(first, second, third);
		}
	}
}
#endif //DXLE_INC_BASIC_TYPES_COORDINATE_OPERATOR_BOOL_HELPER_HPP_
