/*=============================================================================
  Copyright (C) 2015-2017 DxLibEx project
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
#include <limits>
#include <sstream>
#if defined(_MSC_VER) && !defined(__c2__)
#	pragma warning(disable: 4189)
#endif
template<typename T>
struct basic_types_size : public ::iutest::Test{};
IUTEST_TYPED_TEST_CASE(basic_types_size, ::iutest::Types<std::int32_t, std::uint32_t, std::int64_t, std::uint64_t, float, double>);
IUTEST_TYPED_TEST(basic_types_size, construct) {
	using type = TypeParam;
	dxle::uniform_normal_distribution<type> dist;
	auto get_rand = [&dist]() { return dist(engine); };
	for (DXLE_UNUSED auto i : dxle::rep(10)) {
		const auto value_width = get_rand();
		const auto value_height = get_rand();
		const dxle::size_c<type> value = { value_width, value_height };
		IUTEST_ASSERT(value.width == value_width);
		IUTEST_ASSERT(value.height == value_height);
	}
	const dxle::size_c<type> value1 = {};
	const auto value2 = value1;
	const auto value3 = std::move(value1);
	IUTEST_ASSERT(is_l_zero(value1.width, value1.height, value2.width, value2.height, value3.width, value3.height));
	const dxle::size_c<type> value4 = { 2, 12 };
	IUTEST_ASSERT(value4.width == static_cast<type>(2));
	IUTEST_ASSERT(value4.height == static_cast<type>(12));
}
IUTEST_TYPED_TEST(basic_types_size, factory) {
	using type = TypeParam;
	const std::pair<type, type> pa = { static_cast<type>(12), static_cast<type>(23) };
	const auto po = dxle::make_size_c(pa);
	static_assert(std::is_same<typename decltype(po)::value_type, typename decltype(pa)::first_type>::value, "err");
	IUTEST_ASSERT(po.width == pa.first);
	IUTEST_ASSERT(po.height == pa.second);
	const auto po2 = dxle::make_size_c(std::pair<type, type>(pa));
	IUTEST_ASSERT(po2.width == pa.first);
	IUTEST_ASSERT(po2.height == pa.second);
}
IUTEST_TYPED_TEST(basic_types_size, operaotr_eq) {
	using type = TypeParam;
	const bool eq1 = 0 == dxle::size_c<type>{} && dxle::size_c<type>{} == 0;
	IUTEST_ASSERT(eq1);
	dxle::uniform_normal_distribution<type> dist;
	auto get_rand = [&dist]() { return dist(engine); };
	for (DXLE_UNUSED auto i : dxle::rep(10)) {
		const auto value_width = get_rand();
		const auto value_height = get_rand();
		const dxle::size_c<type> value = { value_width, value_height };
		IUTEST_ASSERT(value == dxle::size_c<type>(value_width, value_height));
	}
}
IUTEST_TYPED_TEST(basic_types_size, ostream_operator) {
	using type = TypeParam;
	dxle::uniform_normal_distribution<type> dist;
	auto get_rand = [&dist]() { return dist(engine); };
	for (DXLE_UNUSED auto i : dxle::rep(10)) {
		const auto x = get_rand();
		const auto y = get_rand();
		std::stringstream ss1;
		ss1 << x << ", " << y;
		const dxle::size_c<type> value = { x, y };
		std::stringstream ss2;
		ss2 << value;
		IUTEST_ASSERT_EQ(ss1.str(), ss2.str());
	}
}
IUTEST_TYPED_TEST(basic_types_size, wostream_operator) {
	using type = TypeParam;
	dxle::uniform_normal_distribution<type> dist;
	auto get_rand = [&dist]() { return dist(engine); };
	for (DXLE_UNUSED auto i : dxle::rep(10)) {
		const auto x = get_rand();
		const auto y = get_rand();
		std::wstringstream ss1;
		ss1 << x << L", " << y;
		const dxle::size_c<type> value = { x, y };
		std::wstringstream ss2;
		ss2 << value;
		IUTEST_ASSERT_EQ(ss1.str(), ss2.str());
	}
}
IUTEST_TYPED_TEST(basic_types_size, istream_operator) {
	using type = TypeParam;
	dxle::uniform_normal_distribution<type> dist;
	auto get_rand = [&dist]() { return dist(engine); };
	for (DXLE_UNUSED auto i : dxle::rep(10)) {
		std::stringstream ss1;
		ss1 << get_rand() << ", " << get_rand();
		std::stringstream ss2;
		ss2 << ss1.str();
		type v1, v2;
		ss1 >> v1;
		ss1.ignore((std::numeric_limits<std::streamsize>::max)(), ',');
		ss1 >> v2;
		dxle::size_c<type> v;
		ss2 >> v;
		IUTEST_ASSERT(v == dxle::size_c<type>(v1, v2));
	}
}
IUTEST_TYPED_TEST(basic_types_size, wistream_operator) {
	using type = TypeParam;
	dxle::uniform_normal_distribution<type> dist;
	auto get_rand = [&dist]() { return dist(engine); };
	for (DXLE_UNUSED auto i : dxle::rep(10)) {
		std::wstringstream ss1;
		ss1 << get_rand() << L", " << get_rand();
		std::wstringstream ss2;
		ss2 << ss1.str();
		type v1, v2;
		ss1 >> v1;
		ss1.ignore((std::numeric_limits<std::streamsize>::max)(), L',');
		ss1 >> v2;
		dxle::size_c<type> v;
		ss2 >> v;
		IUTEST_ASSERT(v == dxle::size_c<type>(v1, v2));
	}
}
IUTEST_TYPED_TEST(basic_types_size, unary_operaotr_plus) {
	using type = TypeParam;
	const bool eq1 = 0 == dxle::size_c<type>{} && dxle::size_c<type>{} == 0;
	IUTEST_ASSERT(eq1);
	dxle::uniform_normal_distribution<type> dist;
	auto get_rand = [&dist]() { return dist(engine); };
	for (DXLE_UNUSED auto i : dxle::rep(10)) {
		const dxle::size_c<type> value1 = { get_rand(), get_rand() };
		const auto value2 = +value1;
		IUTEST_ASSERT(value1 == value2);
	}
}
namespace detail {
	template<typename T, bool is_unsigned = std::is_unsigned<T>::value>
	struct basic_types_size_unary_operaotr_minus_helper {
		using type = T;
		void operator()() {}
	};
	template<typename T>
	struct basic_types_size_unary_operaotr_minus_helper<T, false> {
		using type = T;
		void operator()() {
			const bool eq1 = 0 == dxle::size_c<type>{} && dxle::size_c<type>{} == 0;
			IUTEST_ASSERT(eq1);
			dxle::uniform_normal_distribution<type> dist;
			auto get_rand = [&dist]() { return dist(engine); };
			for (DXLE_UNUSED auto i : dxle::rep(10)) {
				const auto x = get_rand();
				const auto y = get_rand();

				const dxle::size_c<type> value1 = { x, y };
				const auto value2 = -value1;
				IUTEST_ASSERT(value2 == dxle::size_c<type>(-x, -y));
			}
		}
	};
}
IUTEST_TYPED_TEST(basic_types_size, unary_operaotr_minus) {
	detail::basic_types_size_unary_operaotr_minus_helper<TypeParam>{}();
}
IUTEST_TYPED_TEST(basic_types_size, operator_add) {
	using type = TypeParam;
	using lim = std::numeric_limits<type>;
	dxle::uniform_normal_distribution<type> dist(lim::min() + 2, lim::max() - 2);
	auto get_rand = [&dist]() { return dist(engine); };
	for (DXLE_UNUSED auto i : dxle::rep(10)) {
		const auto value1_width = get_rand();
		const auto value1_height = get_rand();
		const dxle::size_c<type> value1 = { value1_width, value1_height };
		const auto first_add_dist = get_rand_for_add(value1_width, value1_height, 1);
		auto value2 = value1 + dxle::size_c<type>{first_add_dist, first_add_dist};
		IUTEST_ASSERT(value2.width == value1_width + first_add_dist);
		IUTEST_ASSERT(value2.height == value1_height + first_add_dist);
		const auto second_add_dist = get_rand_for_add(value2.width, value2.height, 0);
		value2 += dxle::size_c<type>{second_add_dist, second_add_dist};
		IUTEST_ASSERT(value2.width == value1_width + first_add_dist + second_add_dist);
		IUTEST_ASSERT(value2.height == value1_height + first_add_dist + second_add_dist);
	}
}
IUTEST_TYPED_TEST(basic_types_size, operator_sub) {
	using type = TypeParam;
	using lim = std::numeric_limits<type>;
	dxle::uniform_normal_distribution<type> dist(lim::min() + 2, lim::max() - 2);
	auto get_rand = [&dist]() { return dist(engine); };
	for (DXLE_UNUSED auto i : dxle::rep(10)) {
		const auto value1_width = get_rand();
		const auto value1_height = get_rand();
		const dxle::size_c<type> value1 = { value1_width, value1_height };
		const auto first_sub_dist = get_rand_for_sub(value1_width, value1_height, 1);
		auto value2 = value1 - dxle::size_c<type>{first_sub_dist, first_sub_dist};
		IUTEST_ASSERT(value2.width == value1_width - first_sub_dist);
		IUTEST_ASSERT(value2.height == value1_height - first_sub_dist);
		const auto second_sub_dist = get_rand_for_sub(value2.width, value2.height, 0);
		value2 -= dxle::size_c<type>{second_sub_dist, second_sub_dist};
		IUTEST_ASSERT(value2.width == value1_width - first_sub_dist - second_sub_dist);
		IUTEST_ASSERT(value2.height == value1_height - first_sub_dist - second_sub_dist);
	}
}
IUTEST_TYPED_TEST(basic_types_size, operator_mul) {
	using type = TypeParam;
	using lim = std::numeric_limits<type>;
	dxle::uniform_normal_distribution<type> dist(inferior_sqrt2(lim::min()), inferior_sqrt(lim::max()));
	auto get_rand = [&dist]() { return dist(engine); };
	for (DXLE_UNUSED auto i : dxle::rep(10)) {
		const auto value1_width = get_rand();
		const auto value1_height = get_rand();
		dxle::size_c<type> value1 = { value1_width, value1_height };
		const auto first_mul_num = get_rand();
		auto value2 = value1 * first_mul_num;
		IUTEST_ASSERT(value2.width == value1_width * first_mul_num);
		IUTEST_ASSERT(value2.height == value1_height * first_mul_num);
		auto value3 = first_mul_num * value1;
		IUTEST_ASSERT(value2 == value3);
		value1 *= first_mul_num;
		IUTEST_ASSERT(value3 == value1);
	}
}
IUTEST_TYPED_TEST(basic_types_size, operator_div) {
	using type = TypeParam;
	using lim = std::numeric_limits<type>;
	for (DXLE_UNUSED auto i : dxle::rep(10)) {
		const auto value1_width = get_rand_for_div1(lim::min(), lim::max());
		const auto value1_height = get_rand_for_div1(lim::min(), lim::max());
		dxle::size_c<type> value1 = { value1_width, value1_height };
		const auto first_div_num = get_rand_for_div2(value1_width, value1_height);
		auto value2 = value1 / first_div_num;
		IUTEST_ASSERT(value2.width == value1_width / first_div_num);
		IUTEST_ASSERT(value2.height == value1_height / first_div_num);
		value1 /= first_div_num;
		IUTEST_ASSERT(value2 == value1);
	}
}
namespace detail {
	template<typename T, bool is_unsigned = std::is_unsigned<T>::value>
	struct basic_types_size_abs_helper {
		using type = T;
		using lim = std::numeric_limits<type>;
		type operator()() { return lim::max(); }
	};
	template<typename T>
	struct basic_types_size_abs_helper<T, false> {
		using type = T;
		using lim = std::numeric_limits<type>;
		type operator()() { return -1; }
	};
}
IUTEST_TYPED_TEST(basic_types_size, abs) {
	using type = TypeParam;
	using lim = std::numeric_limits<type>;
	dxle::uniform_normal_distribution<type> dist(lim::lowest(), detail::basic_types_size_abs_helper<type>()());
	auto get_rand = [&dist]() { return dist(engine); };
	for (DXLE_UNUSED auto i : dxle::rep(10)) {
		const auto value1_width = get_rand();
		const auto value1_height = get_rand();
		dxle::size_c<type> value1 = { value1_width, value1_height };
		IUTEST_ASSERT(dxle::size_c<type>(dxle::abs(value1_width), dxle::abs(value1_height)) == dxle::abs(value1));
	}
}
