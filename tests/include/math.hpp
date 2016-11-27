/*=============================================================================
Copyright (C) 2015-2016 DxLibEx project
https://github.com/Nagarei/DxLibEx/

Distributed under the Boost Software License, Version 1.0.
(See http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#ifndef DXLE_TESTS_INCLUDE_MATH_HPP_
#define DXLE_TESTS_INCLUDE_MATH_HPP_
namespace detail {
	template<typename T, bool is_unsigned = std::is_unsigned<T>::value>
	struct inferior_sqrt_helper {
		T operator()(T x) { return static_cast<T>(::std::sqrt(x)); }
	};
	template<typename T>
	struct inferior_sqrt_helper<T, false> {
		T operator()(T x) { return (x < 0) ? 0 : static_cast<T>(::std::sqrt(x)); }
	};
}
template<typename T>
T inferior_sqrt(T x) { return detail::inferior_sqrt_helper<T>()(x); }
inline std::uint64_t inferior_sqrt(const std::uint64_t x)
{
	if (x < static_cast<std::uint64_t>(std::numeric_limits<double>::max())) return static_cast<std::uint64_t>(::std::sqrt(x));
	std::uint64_t s = 1U;
	auto t = x;
	while (s < t) { s <<= 1;  t >>= 1; }
	do {
		t = s;
		s = (x / s + s) >> 1;
	} while (s < t);
	return t;
}
inline std::int64_t inferior_sqrt(const std::int64_t x)
{
	return (x < 0) ? 0 : x < static_cast<std::int64_t>(std::numeric_limits<double>::max()) ? static_cast<std::int64_t>(::std::sqrt(x))
		: static_cast<std::int64_t>(inferior_sqrt(static_cast<std::uint64_t>(x)));
}
namespace detail {
	template<typename T, bool is_unsigned = std::is_unsigned<T>::value>
	struct inferior_sqrt2_helper {
		T operator()(T x) { return inferior_sqrt(x); }
	};
	template<typename T>
	struct inferior_sqrt2_helper<T, false> {
		T operator()(T x) { return (x < 0) ? -inferior_sqrt(-x) : inferior_sqrt(x); }
	};
}
template<typename T>
T inferior_sqrt2(T x) { return detail::inferior_sqrt2_helper<T>()(x); }
#endif //DXLE_TESTS_INCLUDE_MATH_HPP_
