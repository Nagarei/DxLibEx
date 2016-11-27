/*=============================================================================
Copyright (C) 2015-2016 DxLibEx project
https://github.com/Nagarei/DxLibEx/

Distributed under the Boost Software License, Version 1.0.
(See http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#include "../../3rd_party/iutest/include/iutest.hpp"
#include "basic_types/point2d.hpp"
std::mt19937 engine = dxle::create_engine();
int main(int argc, char** argv)
{
	IUTEST_INIT(&argc, argv);
	return IUTEST_RUN_ALL_TESTS();
}
