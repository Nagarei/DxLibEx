/*=============================================================================
  Copyright (C) 2015-2016 DxLibEx project
  https://github.com/Nagarei/DxLibEx/

  Distributed under the Boost Software License, Version 1.0.
  (See http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#include "../../3rd_party/iutest/include/iutest.hpp"
#include <random>
#include <dxlibex/basic_types.hpp>
#include <dxlibex/utility/constant_range_loop.hpp>
#include "random.hpp"
#include <cstdint>
#include <cmath>
#include <limits>
template<typename T>
DXLE_CONSTEXPR bool is_l_zero(T&& val) {
	return static_cast<T>(0) == val;
}
template<typename T, typename... Rest>
DXLE_CONSTEXPR bool is_l_zero(T&& val, Rest&&... rest) {
	return static_cast<T>(0) == val && is_l_zero(std::forward<Rest>(rest)...);
}
template<typename T>
struct point_c_test : public ::iutest::Test{};
using test_types = ::iutest::Types<std::int32_t, std::uint32_t, std::int64_t, std::uint64_t, float, double>;
IUTEST_TYPED_TEST_CASE(point_c_test, test_types);
IUTEST_TYPED_TEST(point_c_test, construct) {
	using type = TypeParam;
	dxle::uniform_normal_distribution<type> dist;
	auto get_rand = [&dist]() { return dist(engine); };
	for (
#ifndef DXLE_NO_CXX11_ATTRIBUTES
		[[gnu::unused]]
#endif
		auto i : dxle::rep(10)
	) {
		const auto value_x = get_rand();
		const auto value_y = get_rand();
		const dxle::point_c<type> value = { value_x, value_y };
		IUTEST_ASSERT(value.x == value_x);
		IUTEST_ASSERT(value.y == value_y);
	}
	const dxle::point_c<type> value1 = {};
	const auto value2 = value1;
	const auto value3 = std::move(value1);
	IUTEST_ASSERT(is_l_zero(value1.x, value1.y, value2.x, value2.y, value3.x, value3.y));
	const dxle::point_c<type> value4 = { 2, 12 };
	IUTEST_ASSERT(value4.x == static_cast<type>(2));
	IUTEST_ASSERT(value4.y == static_cast<type>(12));
}
IUTEST_TYPED_TEST(point_c_test, factory) {
	using type = TypeParam;
	const std::pair<type, type> pa = { static_cast<type>(12), static_cast<type>(23) };
	const auto po = dxle::make_point_c(pa);
	static_assert(std::is_same<typename decltype(po)::value_type, typename decltype(pa)::first_type>::value, "err");
	IUTEST_ASSERT(po.x == pa.first);
	IUTEST_ASSERT(po.y == pa.second);
	const auto po2 = dxle::make_point_c(std::pair<type, type>(pa));
	IUTEST_ASSERT(po2.x == pa.first);
	IUTEST_ASSERT(po2.y == pa.second);
}
IUTEST_TYPED_TEST(point_c_test, operaotr_eq) {
	using type = TypeParam;
	const bool eq1 = 0 == dxle::point_c<type>{} && dxle::point_c<type>{} == 0;
	IUTEST_ASSERT(eq1);
	dxle::uniform_normal_distribution<type> dist;
	auto get_rand = [&dist]() { return dist(engine); };
	for (
#ifndef DXLE_NO_CXX11_ATTRIBUTES
		[[gnu::unused]]
#endif
		auto i : dxle::rep(10)
	) {
		const auto value_x = get_rand();
		const auto value_y = get_rand();
		const dxle::point_c<type> value = { value_x, value_y };
		IUTEST_ASSERT(value == dxle::point_c<type>(value_x, value_y));
	}
}
template<typename T>
T get_rand_for_add(T n1, T n2, int modifier) {
	using lim = std::numeric_limits<T>;
	const auto minmax = std::minmax(n1, n2);
	return dxle::uniform_normal_distribution<T>(
		(minmax.first > 0) ? lim::min() : lim::min() - minmax.first + modifier,
		(minmax.second < 0) ? lim::max() : lim::max() - minmax.second - modifier
	)(engine);
}
IUTEST_TYPED_TEST(point_c_test, operator_add) {
	using type = TypeParam;
	using lim = std::numeric_limits<type>;
	dxle::uniform_normal_distribution<type> dist(lim::min() + 2, lim::max() - 2);
	auto get_rand = [&dist]() { return dist(engine); };
	for (
#ifndef DXLE_NO_CXX11_ATTRIBUTES
		[[gnu::unused]]
#endif
		auto i : dxle::rep(10)
	) {
		const auto value1_x = get_rand();
		const auto value1_y = get_rand();
		const dxle::point_c<type> value1 = { value1_x, value1_y };
		const auto first_add_dist = get_rand_for_add(value1_x, value1_y, 1);
		auto value2 = value1 + dxle::point_c<type>{first_add_dist, first_add_dist};
		IUTEST_ASSERT(value2.x == value1_x + first_add_dist);
		IUTEST_ASSERT(value2.y == value1_y + first_add_dist);
		const auto second_add_dist = get_rand_for_add(value2.x, value2.y, 0);
		value2 += dxle::point_c<type>{second_add_dist, second_add_dist};
		IUTEST_ASSERT(value2.x == value1_x + first_add_dist + second_add_dist);
		IUTEST_ASSERT(value2.y == value1_y + first_add_dist + second_add_dist);
	}
}
template<typename T>
T get_rand_for_sub(T n1, T n2, int modifier) {
	using lim = std::numeric_limits<T>;
	const auto minmax = std::minmax(n1, n2);
	return dxle::uniform_normal_distribution<T>(
		(minmax.first < 0) ? lim::min() : lim::min() + minmax.first + modifier,
		(minmax.second > 0) ? lim::max() : lim::max() + minmax.second - modifier
	)(engine);
}
IUTEST_TYPED_TEST(point_c_test, operator_sub) {
	using type = TypeParam;
	using lim = std::numeric_limits<type>;
	dxle::uniform_normal_distribution<type> dist(lim::min() + 2, lim::max() - 2);
	auto get_rand = [&dist]() { return dist(engine); };
	for (
#ifndef DXLE_NO_CXX11_ATTRIBUTES
		[[gnu::unused]]
#endif
		auto i : dxle::rep(10)
	) {
		const auto value1_x = get_rand();
		const auto value1_y = get_rand();
		const dxle::point_c<type> value1 = { value1_x, value1_y };
		const auto first_add_dist = get_rand_for_sub(value1_x, value1_y, 1);
		auto value2 = value1 - dxle::point_c<type>{first_add_dist, first_add_dist};
		IUTEST_ASSERT(value2.x == value1_x - first_add_dist);
		IUTEST_ASSERT(value2.y == value1_y - first_add_dist);
		const auto second_add_dist = get_rand_for_sub(value2.x, value2.y, 0);
		value2 -= dxle::point_c<type>{second_add_dist, second_add_dist};
		IUTEST_ASSERT(value2.x == value1_x - first_add_dist - second_add_dist);
		IUTEST_ASSERT(value2.y == value1_y - first_add_dist - second_add_dist);
	}
}
