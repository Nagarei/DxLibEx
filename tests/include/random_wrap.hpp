/*=============================================================================
Copyright (C) 2015-2016 DxLibEx project
https://github.com/Nagarei/DxLibEx/

Distributed under the Boost Software License, Version 1.0.
(See http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#ifndef DXLE_TESTS_INCLUDE_RANDOM_WRAP_HPP_
#define DXLE_TESTS_INCLUDE_RANDOM_WRAP_HPP_
#include "random.hpp"
template<typename T>
inline T get_rand_for_add(T n1, T n2, int modifier)
{
	using lim = std::numeric_limits<T>;
	const auto minmax = std::minmax(n1, n2);
	return dxle::uniform_normal_distribution<T>(
		(minmax.first > 0) ? lim::min() : lim::min() - minmax.first + modifier,
		(minmax.second < 0) ? lim::max() : lim::max() - minmax.second - modifier
	)(engine);
}
template<typename T>
T get_rand_for_sub(T n1, T n2, int modifier)
{
	using lim = std::numeric_limits<T>;
	const auto minmax = std::minmax(n1, n2);
	return dxle::uniform_normal_distribution<T>(
		(minmax.first < 0) ? lim::min() : lim::min() + minmax.first + modifier,
		(minmax.second > 0) ? lim::max() : lim::max() + minmax.second - modifier
	)(engine);
}
template<typename T>
T get_rand_for_div1(T min, T max)
{
	const auto minmax = std::minmax(min, max);
	const auto re = dxle::uniform_normal_distribution<T>(minmax.first, minmax.second)(engine);
	return (0 != re) ? re : get_rand_for_div1(min, max);
}
namespace detail {
	template<typename T, bool is_unsigned = std::is_unsigned<T>::value>
	struct get_rand_for_div2_helper {
		T operator()(T n1, T n2)
		{
			const auto min = std::min(n1, n2);
			return get_rand_for_div1<T>(1u, min);
		}
	};
	template<typename T>
	struct get_rand_for_div2_helper<T, false> {
		T operator()(T n1, T n2)
		{
			const auto min = std::min(std::abs(n1), std::abs(n2));
			return get_rand_for_div1<T>(1, min);
		}
	};
}
template<typename T>
T get_rand_for_div2(T n1, T n2) { return detail::get_rand_for_div2_helper<T>()(n1, n2); }
#endif //DXLE_TESTS_INCLUDE_RANDOM_WRAP_HPP_
