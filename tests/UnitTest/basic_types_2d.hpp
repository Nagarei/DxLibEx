/*=============================================================================
  Copyright (C) 2015-2016 DxLibEx project
  https://github.com/Nagarei/DxLibEx/

  Distributed under the Boost Software License, Version 1.0.
  (See http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#include <random>
#include <dxlibex/basic_types.hpp>
#define BOOST_TEST_MODULE basic_types_2d
#define BOOST_TEST_NO_LIB
#include "boost/test/included/unit_test.hpp"
#include <cmath>


BOOST_AUTO_TEST_CASE(opertor_plus_minus_test)
{

	auto seed = std::random_device{}();
	std::mt19937 engine(seed);
	std::uniform_int_distribution<> dist(INT_MIN, INT_MAX);
	auto get_rand = [&engine, &dist](){ return dist(engine); };

	for (size_t i = 0; i < 10000; ++i)
	{
		int value_x = get_rand();
		int value_y = get_rand();

		dxle::pointi point1{ value_x, value_y };

		BOOST_CHECK_EQUAL(point1.x, value_x);
		BOOST_CHECK_EQUAL(point1.y, value_y);

		dxle::pointi point2(3, 2);
		dxle::pointi point3 = point1 + point2;
		BOOST_CHECK_EQUAL(point3.x, value_x + 3);
		BOOST_CHECK_EQUAL(point3.y, value_y + 2);

		auto point4 = point1 - point2;
		BOOST_CHECK_EQUAL(point4.x, value_x - 3);
		BOOST_CHECK_EQUAL(point4.y, value_y - 2);

		point1 += point2;
		BOOST_CHECK_EQUAL(point1.x, value_x + 3);
		BOOST_CHECK_EQUAL(point1.y, value_y + 2);

		point1 -= point2;
		BOOST_CHECK_EQUAL(point1.x, value_x);
		BOOST_CHECK_EQUAL(point1.y, value_y);

	}


}