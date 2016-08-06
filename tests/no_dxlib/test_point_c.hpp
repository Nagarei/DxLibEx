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
IUTEST_TEST(point_c_construct_test, type_int32_t) {
	using type = std::int32_t;
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
	IUTEST_ASSERT(value1.x == 0);
	IUTEST_ASSERT(value1.y == 0);
	const auto value2 = value1;
	IUTEST_ASSERT(value2.x == 0);
	IUTEST_ASSERT(value2.y == 0);
	const auto value3 = std::move(value1);
	IUTEST_ASSERT(value2.x == 0);
	IUTEST_ASSERT(value2.y == 0);
	const dxle::point_c<type> value4 = { 2, -9 };
	IUTEST_ASSERT(value4.x == 2);
	IUTEST_ASSERT(value4.y == -9);
}
IUTEST_TEST(point_c_construct_test, type_int64_t) {
	using type = std::int64_t;
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
	IUTEST_ASSERT(value1.x == 0);
	IUTEST_ASSERT(value1.y == 0);
	const auto value2 = value1;
	IUTEST_ASSERT(value2.x == 0);
	IUTEST_ASSERT(value2.y == 0);
	const auto value3 = std::move(value1);
	IUTEST_ASSERT(value2.x == 0);
	IUTEST_ASSERT(value2.y == 0);
	const dxle::point_c<type> value4 = { 2, -9 };
	IUTEST_ASSERT(value4.x == 2);
	IUTEST_ASSERT(value4.y == -9);
}
IUTEST_TEST(point_c_construct_test, type_uint32_t) {
	using type = std::uint32_t;
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
	IUTEST_ASSERT(value1.x == 0);
	IUTEST_ASSERT(value1.y == 0);
	const auto value2 = value1;
	IUTEST_ASSERT(value2.x == 0);
	IUTEST_ASSERT(value2.y == 0);
	const auto value3 = std::move(value1);
	IUTEST_ASSERT(value2.x == 0);
	IUTEST_ASSERT(value2.y == 0);
	const dxle::point_c<type> value4 = { 2, 13 };
	IUTEST_ASSERT(value4.x == 2);
	IUTEST_ASSERT(value4.y == 13);
}
IUTEST_TEST(point_c_construct_test, type_uint64_t) {
	using type = std::uint64_t;
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
	IUTEST_ASSERT(value1.x == 0);
	IUTEST_ASSERT(value1.y == 0);
	const auto value2 = value1;
	IUTEST_ASSERT(value2.x == 0);
	IUTEST_ASSERT(value2.y == 0);
	const auto value3 = std::move(value1);
	IUTEST_ASSERT(value2.x == 0);
	IUTEST_ASSERT(value2.y == 0);
	const dxle::point_c<type> value4 = { 2, 13 };
	IUTEST_ASSERT(value4.x == 2);
	IUTEST_ASSERT(value4.y == 13);
}
IUTEST_TEST(point_c_construct_test, type_float) {
	using type = float;
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
	IUTEST_ASSERT(value1.x == 0);
	IUTEST_ASSERT(value1.y == 0);
	const auto value2 = value1;
	IUTEST_ASSERT(value2.x == 0);
	IUTEST_ASSERT(value2.y == 0);
	const auto value3 = std::move(value1);
	IUTEST_ASSERT(value2.x == 0);
	IUTEST_ASSERT(value2.y == 0);
	const dxle::point_c<type> value4 = { 2, -9 };
	IUTEST_ASSERT(value4.x == 2);
	IUTEST_ASSERT(value4.y == -9);
}
IUTEST_TEST(point_c_construct_test, type_double) {
	using type = double;
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
	IUTEST_ASSERT(value1.x == 0);
	IUTEST_ASSERT(value1.y == 0);
	const auto value2 = value1;
	IUTEST_ASSERT(value2.x == 0);
	IUTEST_ASSERT(value2.y == 0);
	const auto value3 = std::move(value1);
	IUTEST_ASSERT(value2.x == 0);
	IUTEST_ASSERT(value2.y == 0);
	const dxle::point_c<type> value4 = { 2, 13 };
	IUTEST_ASSERT(value4.x == 2);
	IUTEST_ASSERT(value4.y == 13);
}
IUTEST_TEST(point_c_factory_test, type_int32_t) {
	using type = std::int32_t;
	const std::pair<type, type> pa = { 12, -34 };
	const auto po = dxle::make_point_c(pa);
	static_assert(std::is_same<decltype(po)::value_type, decltype(pa)::first_type>::value, "err");
	IUTEST_ASSERT(po.x == pa.first);
	IUTEST_ASSERT(po.y == pa.second);
	const auto po2 = dxle::make_point_c(std::pair<type, type>(pa));
	IUTEST_ASSERT(po2.x == pa.first);
	IUTEST_ASSERT(po2.y == pa.second);
}
IUTEST_TEST(point_c_factory_test, type_uint32_t) {
	using type = std::uint32_t;
	const std::pair<type, type> pa = { 12, 32 };
	const auto po = dxle::make_point_c(pa);
	static_assert(std::is_same<decltype(po)::value_type, decltype(pa)::first_type>::value, "err");
	IUTEST_ASSERT(po.x == pa.first);
	IUTEST_ASSERT(po.y == pa.second);
	const auto po2 = dxle::make_point_c(std::pair<type, type>(pa));
	IUTEST_ASSERT(po2.x == pa.first);
	IUTEST_ASSERT(po2.y == pa.second);
}
IUTEST_TEST(point_c_factory_test, type_int64_t) {
	using type = std::int64_t;
	const std::pair<type, type> pa = { 12, -34 };
	const auto po = dxle::make_point_c(pa);
	static_assert(std::is_same<decltype(po)::value_type, decltype(pa)::first_type>::value, "err");
	IUTEST_ASSERT(po.x == pa.first);
	IUTEST_ASSERT(po.y == pa.second);
	const auto po2 = dxle::make_point_c(std::pair<type, type>(pa));
	IUTEST_ASSERT(po2.x == pa.first);
	IUTEST_ASSERT(po2.y == pa.second);
}
IUTEST_TEST(point_c_factory_test, type_uint64_t) {
	using type = std::uint64_t;
	const std::pair<type, type> pa = { 12, 32 };
	const auto po = dxle::make_point_c(pa);
	static_assert(std::is_same<decltype(po)::value_type, decltype(pa)::first_type>::value, "err");
	IUTEST_ASSERT(po.x == pa.first);
	IUTEST_ASSERT(po.y == pa.second);
	const auto po2 = dxle::make_point_c(std::pair<type, type>(pa));
	IUTEST_ASSERT(po2.x == pa.first);
	IUTEST_ASSERT(po2.y == pa.second);
}
IUTEST_TEST(point_c_factory_test, type_float) {
	using type = float;
	const std::pair<type, type> pa = { 12.4f, 1.746f };
	const auto po = dxle::make_point_c(pa);
	static_assert(std::is_same<decltype(po)::value_type, decltype(pa)::first_type>::value, "err");
	IUTEST_ASSERT(po.x == pa.first);
	IUTEST_ASSERT(po.y == pa.second);
	const auto po2 = dxle::make_point_c(std::pair<type, type>(pa));
	IUTEST_ASSERT(po2.x == pa.first);
	IUTEST_ASSERT(po2.y == pa.second);
}
IUTEST_TEST(point_c_factory_test, type_double) {
	using type = double;
	const std::pair<type, type> pa = { 12.4, 1.746 };
	const auto po = dxle::make_point_c(pa);
	static_assert(std::is_same<decltype(po)::value_type, decltype(pa)::first_type>::value, "err");
	IUTEST_ASSERT(po.x == pa.first);
	IUTEST_ASSERT(po.y == pa.second);
	const auto po2 = dxle::make_point_c(std::pair<type, type>(pa));
	IUTEST_ASSERT(po2.x == pa.first);
	IUTEST_ASSERT(po2.y == pa.second);
}
IUTEST_TEST(point_c_operaotr_eq_test, type_int32_t) {
	using type = std::int32_t;
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
IUTEST_TEST(point_c_operaotr_eq_test, type_uint32_t) {
	using type = std::uint32_t;
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
IUTEST_TEST(point_c_operaotr_eq_test, type_int64_t) {
	using type = std::int64_t;
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
IUTEST_TEST(point_c_operaotr_eq_test, type_uint64_t) {
	using type = std::uint64_t;
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
IUTEST_TEST(point_c_operaotr_eq_test, type_float) {
	using type = float;
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
IUTEST_TEST(point_c_operaotr_eq_test, type_double) {
	using type = double;
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
IUTEST_TEST(point_c_operator_add_test, type_int32_t) {
	using type = std::int32_t;
	using lim = std::numeric_limits<type>;
	dxle::uniform_normal_distribution<type> dist(lim::min() + 2, lim::max() - 2);
	auto get_rand = [&dist]() { return dist(engine); };
	auto get_rand2 = [](type n1, type n2, int modifyer) {
		const auto minmax = std::minmax(n1, n2);
		return dxle::uniform_normal_distribution<type>(
			(minmax.first > 0) ? lim::min() : lim::min() - minmax.first + modifyer, (minmax.second < 0) ? lim::max() : lim::max() - minmax.second - modifyer
		)(engine);
	};
	for (
#ifndef DXLE_NO_CXX11_ATTRIBUTES
		[[gnu::unused]]
#endif
		auto i : dxle::rep(10)
	) {
		const auto value1_x = get_rand();
		const auto value1_y = get_rand();
		const dxle::point_c<type> value1 = { value1_x, value1_y };
		const auto first_add_dist = get_rand2(value1_x, value1_y, 1);
		auto value2 = value1 + dxle::point_c<type>{first_add_dist, first_add_dist};
		IUTEST_ASSERT(value2.x == value1_x + first_add_dist);
		IUTEST_ASSERT(value2.y == value1_y + first_add_dist);
		const auto second_add_dist = get_rand2(value2.x, value2.y, 0);
		value2 += dxle::point_c<type>{second_add_dist, second_add_dist};
		IUTEST_ASSERT(value2.x == value1_x + first_add_dist + second_add_dist);
		IUTEST_ASSERT(value2.y == value1_y + first_add_dist + second_add_dist);
	}
}
IUTEST_TEST(point_c_operator_add_test, type_uint32_t) {
	using type = std::uint32_t;
	using lim = std::numeric_limits<type>;
	dxle::uniform_normal_distribution<type> dist(lim::min() + 2, lim::max() - 2);
	auto get_rand = [&dist]() { return dist(engine); };
	auto get_rand2 = [](type n1, type n2, int modifyer) {
		const auto minmax = std::minmax(n1, n2);
		return dxle::uniform_normal_distribution<type>(
			(minmax.first > 0) ? lim::min() : lim::min() - minmax.first + modifyer, (minmax.second < 0) ? lim::max() : lim::max() - minmax.second - modifyer
		)(engine);
	};
	for (
#ifndef DXLE_NO_CXX11_ATTRIBUTES
		[[gnu::unused]]
#endif
		auto i : dxle::rep(10)
	) {
		const auto value1_x = get_rand();
		const auto value1_y = get_rand();
		const dxle::point_c<type> value1 = { value1_x, value1_y };
		const auto first_add_dist = get_rand2(value1_x, value1_y, 1);
		auto value2 = value1 + dxle::point_c<type>{first_add_dist, first_add_dist};
		IUTEST_ASSERT(value2.x == value1_x + first_add_dist);
		IUTEST_ASSERT(value2.y == value1_y + first_add_dist);
		const auto second_add_dist = get_rand2(value2.x, value2.y, 0);
		value2 += dxle::point_c<type>{second_add_dist, second_add_dist};
		IUTEST_ASSERT(value2.x == value1_x + first_add_dist + second_add_dist);
		IUTEST_ASSERT(value2.y == value1_y + first_add_dist + second_add_dist);
	}
}
IUTEST_TEST(point_c_operator_add_test, type_int64_t) {
	using type = std::int64_t;
	using lim = std::numeric_limits<type>;
	dxle::uniform_normal_distribution<type> dist(lim::min() + 2, lim::max() - 2);
	auto get_rand = [&dist]() { return dist(engine); };
	auto get_rand2 = [](type n1, type n2, int modifyer) {
		const auto minmax = std::minmax(n1, n2);
		return dxle::uniform_normal_distribution<type>(
			(minmax.first > 0) ? lim::min() : lim::min() - minmax.first + modifyer, (minmax.second < 0) ? lim::max() : lim::max() - minmax.second - modifyer
		)(engine);
	};
	for (
#ifndef DXLE_NO_CXX11_ATTRIBUTES
		[[gnu::unused]]
#endif
	auto i : dxle::rep(10)
		) {
		const auto value1_x = get_rand();
		const auto value1_y = get_rand();
		const dxle::point_c<type> value1 = { value1_x, value1_y };
		const auto first_add_dist = get_rand2(value1_x, value1_y, 1);
		auto value2 = value1 + dxle::point_c<type>{first_add_dist, first_add_dist};
		IUTEST_ASSERT(value2.x == value1_x + first_add_dist);
		IUTEST_ASSERT(value2.y == value1_y + first_add_dist);
		const auto second_add_dist = get_rand2(value2.x, value2.y, 0);
		value2 += dxle::point_c<type>{second_add_dist, second_add_dist};
		IUTEST_ASSERT(value2.x == value1_x + first_add_dist + second_add_dist);
		IUTEST_ASSERT(value2.y == value1_y + first_add_dist + second_add_dist);
	}
}
IUTEST_TEST(point_c_operator_add_test, type_uint64_t) {
	using type = std::uint64_t;
	using lim = std::numeric_limits<type>;
	dxle::uniform_normal_distribution<type> dist(lim::min() + 2, lim::max() - 2);
	auto get_rand = [&dist]() { return dist(engine); };
	auto get_rand2 = [](type n1, type n2, int modifyer) {
		const auto minmax = std::minmax(n1, n2);
		return dxle::uniform_normal_distribution<type>(
			(minmax.first > 0) ? lim::min() : lim::min() - minmax.first + modifyer, (minmax.second < 0) ? lim::max() : lim::max() - minmax.second - modifyer
		)(engine);
	};
	for (
#ifndef DXLE_NO_CXX11_ATTRIBUTES
		[[gnu::unused]]
#endif
	auto i : dxle::rep(10)
		) {
		const auto value1_x = get_rand();
		const auto value1_y = get_rand();
		const dxle::point_c<type> value1 = { value1_x, value1_y };
		const auto first_add_dist = get_rand2(value1_x, value1_y, 1);
		auto value2 = value1 + dxle::point_c<type>{first_add_dist, first_add_dist};
		IUTEST_ASSERT(value2.x == value1_x + first_add_dist);
		IUTEST_ASSERT(value2.y == value1_y + first_add_dist);
		const auto second_add_dist = get_rand2(value2.x, value2.y, 0);
		value2 += dxle::point_c<type>{second_add_dist, second_add_dist};
		IUTEST_ASSERT(value2.x == value1_x + first_add_dist + second_add_dist);
		IUTEST_ASSERT(value2.y == value1_y + first_add_dist + second_add_dist);
	}
}
IUTEST_TEST(point_c_operator_add_test, type_float) {
	using type = float;
	using lim = std::numeric_limits<type>;
	dxle::uniform_normal_distribution<type> dist(lim::min() + 2, lim::max() - 2);
	auto get_rand = [&dist]() { return dist(engine); };
	auto get_rand2 = [](type n1, type n2, int modifyer) {
		const auto minmax = std::minmax(n1, n2);
		return dxle::uniform_normal_distribution<type>(
			(minmax.first > 0) ? lim::min() : lim::min() - minmax.first + modifyer, (minmax.second < 0) ? lim::max() : lim::max() - minmax.second - modifyer
		)(engine);
	};
	for (
#ifndef DXLE_NO_CXX11_ATTRIBUTES
		[[gnu::unused]]
#endif
	auto i : dxle::rep(10)
		) {
		const auto value1_x = get_rand();
		const auto value1_y = get_rand();
		const dxle::point_c<type> value1 = { value1_x, value1_y };
		const auto first_add_dist = get_rand2(value1_x, value1_y, 1);
		auto value2 = value1 + dxle::point_c<type>{first_add_dist, first_add_dist};
		IUTEST_ASSERT(value2.x == value1_x + first_add_dist);
		IUTEST_ASSERT(value2.y == value1_y + first_add_dist);
		const auto second_add_dist = get_rand2(value2.x, value2.y, 0);
		value2 += dxle::point_c<type>{second_add_dist, second_add_dist};
		IUTEST_ASSERT(value2.x == value1_x + first_add_dist + second_add_dist);
		IUTEST_ASSERT(value2.y == value1_y + first_add_dist + second_add_dist);
	}
}
IUTEST_TEST(point_c_operator_add_test, type_double) {
	using type = double;
	using lim = std::numeric_limits<type>;
	dxle::uniform_normal_distribution<type> dist(lim::min() + 2, lim::max() - 2);
	auto get_rand = [&dist]() { return dist(engine); };
	auto get_rand2 = [](type n1, type n2, int modifyer) {
		const auto minmax = std::minmax(n1, n2);
		return dxle::uniform_normal_distribution<type>(
			(minmax.first > 0) ? lim::min() : lim::min() - minmax.first + modifyer, (minmax.second < 0) ? lim::max() : lim::max() - minmax.second - modifyer
		)(engine);
	};
	for (
#ifndef DXLE_NO_CXX11_ATTRIBUTES
		[[gnu::unused]]
#endif
	auto i : dxle::rep(10)
		) {
		const auto value1_x = get_rand();
		const auto value1_y = get_rand();
		const dxle::point_c<type> value1 = { value1_x, value1_y };
		const auto first_add_dist = get_rand2(value1_x, value1_y, 1);
		auto value2 = value1 + dxle::point_c<type>{first_add_dist, first_add_dist};
		IUTEST_ASSERT(value2.x == value1_x + first_add_dist);
		IUTEST_ASSERT(value2.y == value1_y + first_add_dist);
		const auto second_add_dist = get_rand2(value2.x, value2.y, 0);
		value2 += dxle::point_c<type>{second_add_dist, second_add_dist};
		IUTEST_ASSERT(value2.x == value1_x + first_add_dist + second_add_dist);
		IUTEST_ASSERT(value2.y == value1_y + first_add_dist + second_add_dist);
	}
}
