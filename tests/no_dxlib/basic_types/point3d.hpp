/*=============================================================================
  Copyright (C) 2015-2016 DxLibEx project
  https://github.com/Nagarei/DxLibEx/

  Distributed under the Boost Software License, Version 1.0.
  (See http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#include <3rd_party/iutest/include/iutest.hpp>
#include <random>
#include <dxlibex/basic_types.hpp>
#include <dxlibex/utility/constant_range_loop.hpp>
#include <tests/include/random.hpp>
#include <tests/include/random_wrap.hpp>
#include <tests/include/math.hpp>
#include <tests/include/utility.hpp>
#include <cstdint>
#include <cmath>
#include <array>
#include <limits>
#include <sstream>
#if defined(_MSC_VER) && !defined(__c2__)
#	pragma warning(disable: 4189)
#endif
template<typename T>
struct basic_types_point3d : public ::iutest::Test{};
IUTEST_TYPED_TEST_CASE(basic_types_point3d, ::iutest::Types<std::int32_t, std::uint32_t, std::int64_t, std::uint64_t, float, double>);
IUTEST_TYPED_TEST(basic_types_point3d, construct) {
	using type = TypeParam;
	dxle::uniform_normal_distribution<type> dist;
	auto get_rand = [&dist]() { return dist(engine); };
	for (DXLE_UNUSED auto i : dxle::rep(10)) {
		const auto value_x = get_rand();
		const auto value_y = get_rand();
		const auto value_z = get_rand();
		const dxle::point3d_c<type> value = { value_x, value_y, value_z };
		IUTEST_ASSERT(value.x == value_x);
		IUTEST_ASSERT(value.y == value_y);
		IUTEST_ASSERT(value.z == value_z);
	}
	const dxle::point3d_c<type> value1 = {};
	const auto value2 = value1;
	const auto value3 = std::move(value1);
	IUTEST_ASSERT(is_l_zero(value1.x, value1.y, value1.z, value2.x, value2.y, value2.z, value3.x, value3.y, value3.z));
	const dxle::point3d_c<type> value4 = { 2, 12, 22 };
	IUTEST_ASSERT(value4.x == static_cast<type>(2));
	IUTEST_ASSERT(value4.y == static_cast<type>(12));
	IUTEST_ASSERT(value4.z == static_cast<type>(22));
}
IUTEST_TYPED_TEST(basic_types_point3d, factory) {
	using type = TypeParam;
	const std::tuple<type, type, type> pa = { static_cast<type>(12), static_cast<type>(23), static_cast<type>(34) };
	const auto po = dxle::make_point3d_c(pa);
	static_assert(std::is_same<typename decltype(po)::value_type, type>::value, "err");
	IUTEST_ASSERT(po.x == std::get<0>(pa));
	IUTEST_ASSERT(po.y == std::get<1>(pa));
	IUTEST_ASSERT(po.z == std::get<2>(pa));
	const auto po2 = dxle::make_point3d_c(std::tuple<type, type, type>(pa));
	IUTEST_ASSERT(po2.x == std::get<0>(pa));
	IUTEST_ASSERT(po2.y == std::get<1>(pa));
	IUTEST_ASSERT(po2.z == std::get<2>(pa));
}
IUTEST_TYPED_TEST(basic_types_point3d, operaotr_eq) {
	using type = TypeParam;
	const bool eq1 = 0 == dxle::point3d_c<type>{};
	const bool eq2 = dxle::point3d_c<type>{} == 0;
	IUTEST_ASSERT(eq1);
	IUTEST_ASSERT(eq2);
	dxle::uniform_normal_distribution<type> dist;
	auto get_rand = [&dist]() { return dist(engine); };
	for (DXLE_UNUSED auto i : dxle::rep(10)) {
		const auto value_x = get_rand();
		const auto value_y = get_rand();
		const auto value_z = get_rand();
		const dxle::point3d_c<type> value = { value_x, value_y, value_z };
		IUTEST_ASSERT(value == dxle::point3d_c<type>(value_x, value_y, value_z));
	}
}
IUTEST_TYPED_TEST(basic_types_point3d, ostream_operator) {
	using type = TypeParam;
	dxle::uniform_normal_distribution<type> dist;
	auto get_rand = [&dist]() { return dist(engine); };
	for (DXLE_UNUSED auto i : dxle::rep(10)) {
		const auto x = get_rand();
		const auto y = get_rand();
		const auto z = get_rand();
		std::stringstream ss1;
		ss1 << x << ", " << y << ", " << z;
		const dxle::point3d_c<type> value = { x, y, z };
		std::stringstream ss2;
		ss2 << value;
		IUTEST_ASSERT_EQ(ss1.str(), ss2.str());
	}
}
IUTEST_TYPED_TEST(basic_types_point3d, wostream_operator) {
	using type = TypeParam;
	dxle::uniform_normal_distribution<type> dist;
	auto get_rand = [&dist]() { return dist(engine); };
	for (DXLE_UNUSED auto i : dxle::rep(10)) {
		const auto x = get_rand();
		const auto y = get_rand();
		const auto z = get_rand();
		std::wstringstream ss1;
		ss1 << x << L", " << y << L", " << z;
		const dxle::point3d_c<type> value = { x, y, z };
		std::wstringstream ss2;
		ss2 << value;
		IUTEST_ASSERT_EQ(ss1.str(), ss2.str());
	}
}
IUTEST_TYPED_TEST(basic_types_point3d, istream_operator) {
	using type = TypeParam;
	dxle::uniform_normal_distribution<type> dist;
	auto get_rand = [&dist]() { return dist(engine); };
	for (DXLE_UNUSED auto i : dxle::rep(10)) {
		std::stringstream ss1;
		ss1 << get_rand() << ", " << get_rand() << ", " << get_rand();
		std::stringstream ss2;
		ss2 << ss1.str();
		type v1, v2, v3;
		ss1 >> v1;
		ss1.ignore((std::numeric_limits<std::streamsize>::max)(), ',');
		ss1 >> v2;
		ss1.ignore((std::numeric_limits<std::streamsize>::max)(), ',');
		ss1 >> v3;
		dxle::point3d_c<type> v;
		ss2 >> v;
		IUTEST_ASSERT(v == dxle::point3d_c<type>(v1, v2, v3));
	}
}
IUTEST_TYPED_TEST(basic_types_point3d, wistream_operator) {
	using type = TypeParam;
	dxle::uniform_normal_distribution<type> dist;
	auto get_rand = [&dist]() { return dist(engine); };
	for (DXLE_UNUSED auto i : dxle::rep(10)) {
		std::wstringstream ss1;
		ss1 << get_rand() << L", " << get_rand() << L", " << get_rand();
		std::wstringstream ss2;
		ss2 << ss1.str();
		type v1, v2, v3;
		ss1 >> v1;
		ss1.ignore((std::numeric_limits<std::streamsize>::max)(), L',');
		ss1 >> v2;
		ss1.ignore((std::numeric_limits<std::streamsize>::max)(), L',');
		ss1 >> v3;
		dxle::point3d_c<type> v;
		ss2 >> v;
		IUTEST_ASSERT(v == dxle::point3d_c<type>(v1, v2, v3));
	}
}
IUTEST_TYPED_TEST(basic_types_point3d, unary_operaotr_plus) {
	using type = TypeParam;
	const bool eq1 = 0 == dxle::point3d_c<type>{} && dxle::point3d_c<type>{} == 0;
	IUTEST_ASSERT(eq1);
	dxle::uniform_normal_distribution<type> dist;
	auto get_rand = [&dist]() { return dist(engine); };
	for (DXLE_UNUSED auto i : dxle::rep(10)) {
		const dxle::point3d_c<type> value1 = { get_rand(), get_rand(), get_rand() };
		const auto value2 = +value1;
		IUTEST_ASSERT(value1 == value2);
	}
}
namespace detail {
	template<typename T, bool is_unsigned = std::is_unsigned<T>::value>
	struct basic_types_point3d_unary_operaotr_minus_helper {
		using type = T;
		void operator()() {}
	};
	template<typename T>
	struct basic_types_point3d_unary_operaotr_minus_helper<T, false> {
		using type = T;
		void operator()() {
			const bool eq1 = 0 == dxle::point3d_c<type>{} && dxle::point3d_c<type>{} == 0;
			IUTEST_ASSERT(eq1);
			dxle::uniform_normal_distribution<type> dist;
			auto get_rand = [&dist]() { return dist(engine); };
			for (DXLE_UNUSED auto i : dxle::rep(10)) {
				const auto x = get_rand();
				const auto y = get_rand();
				const auto z = get_rand();

				const dxle::point3d_c<type> value1 = { x, y, z };
				const auto value2 = -value1;
				IUTEST_ASSERT(value2 == dxle::point3d_c<type>(-x, -y, -z));
			}
		}
	};
}
IUTEST_TYPED_TEST(basic_types_point3d, unary_operaotr_minus) {
	detail::basic_types_point3d_unary_operaotr_minus_helper<TypeParam>{}();
}
IUTEST_TYPED_TEST(basic_types_point3d, operator_add) {
	using type = TypeParam;
	using lim = std::numeric_limits<type>;
	dxle::uniform_normal_distribution<type> dist(lim::min() + 2, lim::max() - 2);
	auto get_rand = [&dist]() { return dist(engine); };
	for (DXLE_UNUSED auto i : dxle::rep(10)) {
		const auto value1_x = get_rand();
		const auto value1_y = get_rand();
		const auto value1_z = get_rand();
		const dxle::point3d_c<type> value1 = { value1_x, value1_y, value1_z };
		const auto first_add_dist = get_rand_for_add(value1_x, value1_y, value1_z, 1);
		auto value2 = value1 + dxle::point3d_c<type>{first_add_dist, first_add_dist, first_add_dist };
		IUTEST_ASSERT(value2.x == value1_x + first_add_dist);
		IUTEST_ASSERT(value2.y == value1_y + first_add_dist);
		IUTEST_ASSERT(value2.z == value1_z + first_add_dist);
		const auto second_add_dist = get_rand_for_add(value2.x, value2.y, value2.z, 0);
		value2 += dxle::point3d_c<type>{second_add_dist, second_add_dist, second_add_dist};
		IUTEST_ASSERT(value2.x == value1_x + first_add_dist + second_add_dist);
		IUTEST_ASSERT(value2.y == value1_y + first_add_dist + second_add_dist);
		IUTEST_ASSERT(value2.z == value1_z + first_add_dist + second_add_dist);
	}
}
IUTEST_TYPED_TEST(basic_types_point3d, operator_sub) {
	using type = TypeParam;
	using lim = std::numeric_limits<type>;
	dxle::uniform_normal_distribution<type> dist(lim::min() + 2, lim::max() - 2);
	auto get_rand = [&dist]() { return dist(engine); };
	for (DXLE_UNUSED auto i : dxle::rep(10)) {
		const auto value1_x = get_rand();
		const auto value1_y = get_rand();
		const auto value1_z = get_rand();
		const dxle::point3d_c<type> value1 = { value1_x, value1_y, value1_z };
		const auto first_add_dist = get_rand_for_sub(value1_x, value1_y, value1_z, 1);
		auto value2 = value1 - dxle::point3d_c<type>{first_add_dist, first_add_dist, first_add_dist };
		IUTEST_ASSERT(value2.x == value1_x - first_add_dist);
		IUTEST_ASSERT(value2.y == value1_y - first_add_dist);
		IUTEST_ASSERT(value2.z == value1_z - first_add_dist);
		const auto second_add_dist = get_rand_for_sub(value2.x, value2.y, value2.z, 0);
		value2 -= dxle::point3d_c<type>{second_add_dist, second_add_dist, second_add_dist};
		IUTEST_ASSERT(value2.x == value1_x - first_add_dist - second_add_dist);
		IUTEST_ASSERT(value2.y == value1_y - first_add_dist - second_add_dist);
		IUTEST_ASSERT(value2.z == value1_z - first_add_dist - second_add_dist);
	}
}
IUTEST_TYPED_TEST(basic_types_point3d, operator_mul) {
	using type = TypeParam;
	using lim = std::numeric_limits<type>;
	dxle::uniform_normal_distribution<type> dist(inferior_sqrt2(lim::min()), inferior_sqrt(lim::max()));
	auto get_rand = [&dist]() { return dist(engine); };
	for (DXLE_UNUSED auto i : dxle::rep(10)) {
		const auto value1_x = get_rand();
		const auto value1_y = get_rand();
		const auto value1_z = get_rand();
		dxle::point3d_c<type> value1 = { value1_x, value1_y, value1_z };
		const auto first_mul_num = get_rand();
		auto value2 = value1 * first_mul_num;
		IUTEST_ASSERT(value2.x == value1_x * first_mul_num);
		IUTEST_ASSERT(value2.y == value1_y * first_mul_num);
		IUTEST_ASSERT(value2.z == value1_z * first_mul_num);
		auto value3 = first_mul_num * value1;
		IUTEST_ASSERT(value2 == value3);
		value1 *= first_mul_num;
		IUTEST_ASSERT(value3 == value1);
	}
}
IUTEST_TYPED_TEST(basic_types_point3d, operator_div) {
	using type = TypeParam;
	using lim = std::numeric_limits<type>;
	for (DXLE_UNUSED auto i : dxle::rep(10)) {
		const auto value1_x = get_rand_for_div1(lim::min(), lim::max());
		const auto value1_y = get_rand_for_div1(lim::min(), lim::max());
		const auto value1_z = get_rand_for_div1(lim::min(), lim::max());
		dxle::point3d_c<type> value1 = { value1_x, value1_y, value1_z };
		const auto first_div_num = get_rand_for_div2(value1_x, value1_y, value1_z);
		auto value2 = value1 / first_div_num;
		IUTEST_ASSERT(value2.x == value1_x / first_div_num);
		IUTEST_ASSERT(value2.y == value1_y / first_div_num);
		IUTEST_ASSERT(value2.z == value1_z / first_div_num);
		value1 /= first_div_num;
		IUTEST_ASSERT(value2 == value1);
	}
}
namespace detail {
	template<typename T, bool is_unsigned = std::is_unsigned<T>::value>
	struct basic_types_point3d_abs_helper {
		using type = T;
		using lim = std::numeric_limits<type>;
		type operator()() { return lim::max(); }
	};
	template<typename T>
	struct basic_types_point3d_abs_helper<T, false> {
		using type = T;
		using lim = std::numeric_limits<type>;
		type operator()() { return -1; }
	};
}
IUTEST_TYPED_TEST(basic_types_point3d, abs) {
	using type = TypeParam;
	using lim = std::numeric_limits<type>;
	dxle::uniform_normal_distribution<type> dist(lim::lowest(), detail::basic_types_point3d_abs_helper<type>()());
	auto get_rand = [&dist]() { return dist(engine); };
	for (DXLE_UNUSED auto i : dxle::rep(10)) {
		const auto value1_x = get_rand();
		const auto value1_y = get_rand();
		const auto value1_z = get_rand();
		dxle::point3d_c<type> value1 = { value1_x, value1_y, value1_z };
		IUTEST_ASSERT(dxle::point3d_c<type>(dxle::abs(value1_x), dxle::abs(value1_y), dxle::abs(value1_z)) == dxle::abs(value1));
	}
}
namespace detail {
	template<typename T, bool is_unsigned = std::is_unsigned<T>::value>
	struct basic_types_point3d_dot_helper {
		using type = T;
		void operator()() {}
	};
	template<typename T>
	struct basic_types_point3d_dot_helper<T, false> {
		using type = T;
		void operator()()
		{
			const dxle::point3d_c<type> a = { -3, 1, 2 };
			const dxle::point3d_c<type> b = { -2, 3, 1 };
			const dxle::point3d_c<type> c = { -1, 2, 3 };
			IUTEST_ASSERT(type(3) == (dxle::dot(b - a, c- a)));
		}
	};
}
IUTEST_TYPED_TEST(basic_types_point3d, dot_product) {
	using type = TypeParam;
	using lim = std::numeric_limits<type>;
	dxle::uniform_normal_distribution<type> dist(inferior_sqrt2(lim::min()) / 2, inferior_sqrt(lim::max()) / 2);
	auto get_rand = [&dist]() { return dist(engine); };
	for (DXLE_UNUSED auto i : dxle::rep(10)) {
		const dxle::point3d_c<type> value1 = { get_rand(), get_rand(), get_rand() };
		const dxle::point3d_c<type> value2 = { get_rand(), get_rand(), get_rand() };
		const auto re1 = dxle::dot(value1, value2);
		const auto re2 = value1.x * value2.x + value1.y * value2.y + value1.z * value2.z;
		IUTEST_ASSERT(re1 == re2);
	}
	detail::basic_types_point3d_dot_helper<type>{}();
	const dxle::point3d_c<type> ex = { 1, 0, 0 };
	const dxle::point3d_c<type> ey = { 0, 1, 0 };
	const dxle::point3d_c<type> ez = { 0, 0, 1 };
	IUTEST_ASSERT_EQ(type(0), dxle::dot(ex, ey));
	IUTEST_ASSERT_EQ(type(0), dxle::dot(ey, ez));
	IUTEST_ASSERT_EQ(type(0), dxle::dot(ez, ex));
	IUTEST_ASSERT_EQ(type(1), dxle::dot(ex, ex));
	IUTEST_ASSERT_EQ(type(1), dxle::dot(ey, ey));
	IUTEST_ASSERT_EQ(type(1), dxle::dot(ez, ez));
}
namespace detail {
	template<typename T, bool is_unsigned = std::is_unsigned<T>::value>
	struct basic_types_point3d_cross_helper {
		using type = T;
		void operator()() {}
	};
	template<typename T>
	struct basic_types_point3d_cross_helper<T, false> {
		using type = T;
		void operator()()
		{
			static const std::array<dxle::point3d_c<type>, 3> check_list[] = {
				{{ { 1,  3, 2 }, { -1,  1,  2 }, {  4,  -4,   4 } }},
				{{ { 1,  1, 2 }, {  3,  6,  9 }, { -3,  -3,   3 } }},
				{{ { 6,  3, 3 }, { -7, -3, 10 }, { 39, -81,   3 } }},
				{{ { 0, -1, 2 }, { -1,  0,  5 }, { -5,  -2,  -1 } }}
			};
			for (auto&& i : check_list) {
				IUTEST_ASSERT(i[2] == (dxle::cross(i[0], i[1])));
			}
		}
	};
}
IUTEST_TYPED_TEST(basic_types_point3d, cross_product) {
	using type = TypeParam;
	detail::basic_types_point3d_cross_helper<type>{}();
	const dxle::point3d_c<type> ex = { 1, 0, 0 };
	const dxle::point3d_c<type> ey = { 0, 1, 0 };
	const dxle::point3d_c<type> ez = { 0, 0, 1 };
	IUTEST_ASSERT_EQ(ez, dxle::cross(ex, ey));
	IUTEST_ASSERT_EQ(ex, dxle::cross(ey, ez));
	IUTEST_ASSERT_EQ(ey, dxle::cross(ez, ex));
	IUTEST_ASSERT((0 == dxle::cross(ex, ex)));
	IUTEST_ASSERT((0 == dxle::cross(ey, ey)));
	IUTEST_ASSERT((0 == dxle::cross(ez, ez)));
}
IUTEST_TYPED_TEST(basic_types_point3d, distance) {
	using type = TypeParam;
	const dxle::point3d_c<type> zero = {};
	const dxle::point3d_c<type> a = { 12, 15, 16 };
	IUTEST_ASSERT(type(25) == dxle::distance(zero, a));
	static const dxle::point3d_c<type> b[] = { { 3, 4, 0 }, { 0, 3, 4 }, { 4, 0, 3 } };
	for (auto&& i : b) {
		IUTEST_ASSERT(type(5) == dxle::distance(zero, i));
	}
	const dxle::point3d_c<type> ex = { 1, 0, 0 };
	IUTEST_ASSERT(type(1) == dxle::distance(zero, ex));
	const dxle::point3d_c<type> ey = { 0, 1, 0 };
	IUTEST_ASSERT(type(1) == dxle::distance(zero, ey));
	const dxle::point3d_c<type> ez = { 0, 0, 1 };
	IUTEST_ASSERT(type(1) == dxle::distance(zero, ez));
}
