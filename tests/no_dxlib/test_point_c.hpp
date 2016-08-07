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
#include <sstream>
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
IUTEST_TYPED_TEST_CASE(point_c_test, ::iutest::Types<std::int32_t, std::uint32_t, std::int64_t, std::uint64_t, float, double>);
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
IUTEST_TYPED_TEST(point_c_test, ostream_operator) {
	using type = TypeParam;
	dxle::uniform_normal_distribution<type> dist;
	auto get_rand = [&dist]() { return dist(engine); };
	for (
#ifndef DXLE_NO_CXX11_ATTRIBUTES
		[[gnu::unused]]
#endif
		auto i : dxle::rep(10)
	) {
		const auto x = get_rand();
		const auto y = get_rand();
		std::stringstream ss1;
		ss1 << x << ", " << y;
		const dxle::point_c<type> value = { x, y };
		std::stringstream ss2;
		ss2 << value;
		IUTEST_ASSERT_EQ(ss1.str(), ss2.str());
	}
}
IUTEST_TYPED_TEST(point_c_test, wostream_operator) {
	using type = TypeParam;
	dxle::uniform_normal_distribution<type> dist;
	auto get_rand = [&dist]() { return dist(engine); };
	for (
#ifndef DXLE_NO_CXX11_ATTRIBUTES
		[[gnu::unused]]
#endif
		auto i : dxle::rep(10)
	) {
		const auto x = get_rand();
		const auto y = get_rand();
		std::wstringstream ss1;
		ss1 << x << L", " << y;
		const dxle::point_c<type> value = { x, y };
		std::wstringstream ss2;
		ss2 << value;
		IUTEST_ASSERT_EQ(ss1.str(), ss2.str());
	}
}
IUTEST_TYPED_TEST(point_c_test, istream_operator) {
	using type = TypeParam;
	dxle::uniform_normal_distribution<type> dist;
	auto get_rand = [&dist]() { return dist(engine); };
	for (
#ifndef DXLE_NO_CXX11_ATTRIBUTES
		[[gnu::unused]]
#endif
		auto i : dxle::rep(10)
	) {
		std::stringstream ss1;
		ss1 << get_rand() << ", " << get_rand();
		std::stringstream ss2; 
		ss2 << ss1.str();
		type v1, v2;
		ss1 >> v1;
		ss1.ignore((std::numeric_limits<std::streamsize>::max)(), ',');
		ss1 >> v2;
		dxle::point_c<type> v;
		ss2 >> v;
		IUTEST_ASSERT(v == dxle::point_c<type>(v1, v2));
	}
}
IUTEST_TYPED_TEST(point_c_test, wistream_operator) {
	using type = TypeParam;
	dxle::uniform_normal_distribution<type> dist;
	auto get_rand = [&dist]() { return dist(engine); };
	for (
#ifndef DXLE_NO_CXX11_ATTRIBUTES
		[[gnu::unused]]
#endif
		auto i : dxle::rep(10)
	) {
		std::wstringstream ss1;
		ss1 << get_rand() << L", " << get_rand();
		std::wstringstream ss2; 
		ss2 << ss1.str();
		type v1, v2;
		ss1 >> v1;
		ss1.ignore((std::numeric_limits<std::streamsize>::max)(), L',');
		ss1 >> v2;
		dxle::point_c<type> v;
		ss2 >> v;
		IUTEST_ASSERT(v == dxle::point_c<type>(v1, v2));
	}
}
IUTEST_TYPED_TEST(point_c_test, unary_operaotr_plus) {
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
		const dxle::point_c<type> value1 = { get_rand(), get_rand() };
		const auto value2 = +value1;
		IUTEST_ASSERT(value1 == value2);
	}
}
namespace detail {
	template<typename T, bool is_unsigned = std::is_unsigned<T>::value>
	struct point_c_test_unary_operaotr_minus_helper {
		using type = T;
		void operator()() {}
	};
	template<typename T>
	struct point_c_test_unary_operaotr_minus_helper<T, false> {
		using type = T;
		void operator()() {
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
				const auto x = get_rand();
				const auto y = get_rand();

				const dxle::point_c<type> value1 = { x, y };
				const auto value2 = -value1;
				IUTEST_ASSERT(value2 == dxle::point_c<type>(-x, -y));
			}
		}
	};
}
IUTEST_TYPED_TEST(point_c_test, unary_operaotr_minus) {
	detail::point_c_test_unary_operaotr_minus_helper<TypeParam>{}();
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
		const auto first_sub_dist = get_rand_for_sub(value1_x, value1_y, 1);
		auto value2 = value1 - dxle::point_c<type>{first_sub_dist, first_sub_dist};
		IUTEST_ASSERT(value2.x == value1_x - first_sub_dist);
		IUTEST_ASSERT(value2.y == value1_y - first_sub_dist);
		const auto second_sub_dist = get_rand_for_sub(value2.x, value2.y, 0);
		value2 -= dxle::point_c<type>{second_sub_dist, second_sub_dist};
		IUTEST_ASSERT(value2.x == value1_x - first_sub_dist - second_sub_dist);
		IUTEST_ASSERT(value2.y == value1_y - first_sub_dist - second_sub_dist);
	}
}
template<typename T, dxle::enable_if_t<std::is_unsigned<T>::value, std::nullptr_t> = nullptr>
T inferior_sqrt(T x) { return static_cast<T>(::std::sqrt(x)); }
template<typename T, dxle::enable_if_t<!std::is_unsigned<T>::value, std::nullptr_t> = nullptr>
T inferior_sqrt(T x) { return (x < 0) ? 0 : static_cast<T>(::std::sqrt(x)); }
std::int64_t inferior_sqrt(const std::int64_t x) {
	return (x < 0) ? 0 : x < static_cast<std::int64_t>(std::numeric_limits<double>::max()) ? static_cast<std::int64_t>(::std::sqrt(x))
		: static_cast<std::int64_t>(inferior_sqrt(static_cast<std::uint64_t>(x)));
}
std::uint64_t inferior_sqrt(const std::uint64_t x)
{
	if(x < static_cast<std::uint64_t>(std::numeric_limits<double>::max())) return static_cast<std::uint64_t>(::std::sqrt(x));
	std::uint64_t s = 1U;
	auto t = x;
	while (s < t) { s <<= 1;  t >>= 1; }
	do {
		t = s;
		s = (x / s + s) >> 1;
	} while (s < t);
	return t;
}
template<typename T, dxle::enable_if_t<!std::is_unsigned<T>::value, std::nullptr_t> = nullptr>
T inferior_sqrt2(T x) { return (x < 0) ? -inferior_sqrt(-x) : inferior_sqrt(x); }
template<typename T, dxle::enable_if_t<std::is_unsigned<T>::value, std::nullptr_t> = nullptr>
T inferior_sqrt2(T x) { return inferior_sqrt(x); }
IUTEST_TYPED_TEST(point_c_test, operator_mul) {
	using type = TypeParam;
	using lim = std::numeric_limits<type>;
	dxle::uniform_normal_distribution<type> dist(inferior_sqrt2(lim::min()), inferior_sqrt(lim::max()));
	auto get_rand = [&dist]() { return dist(engine); };
	for (
#ifndef DXLE_NO_CXX11_ATTRIBUTES
		[[gnu::unused]]
#endif
		auto i : dxle::rep(10)
	) {
		const auto value1_x = get_rand();
		const auto value1_y = get_rand();
		dxle::point_c<type> value1 = { value1_x, value1_y };
		const auto first_mul_dist = get_rand();
		auto value2 = value1 * first_mul_dist;
		IUTEST_ASSERT(value2.x == value1_x * first_mul_dist);
		IUTEST_ASSERT(value2.y == value1_y * first_mul_dist);
		auto value3 = first_mul_dist * value1;
		IUTEST_ASSERT(value2 == value3);
		value1 *= first_mul_dist;
		IUTEST_ASSERT(value3 == value1);
	}
}
