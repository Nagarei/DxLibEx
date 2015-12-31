/*=============================================================================
  Copyright (C) 2015-2016 DxLibEx project
  https://github.com/Nagarei/DxLibEx/

  Distributed under the Boost Software License, Version 1.0.
  (See http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#include <dxlibex/basic_types.hpp>
int main() {
	using std::cout;
	using std::endl;

	dxle::point_c<int> p1(3, 2);
	dxle::pointi p2{ 3, 5 };
	cout << "p1:" << p1 << " p2:" << p2 << endl;

	p2 = p1;//copy assignment operator call
	p1 = dxle::point_c<int>(4, 5);//move assignment operator call
	cout << "p1:" << p1 << " p2:" << p2 << endl;

	auto p3 = p2;//copy constructor call
	auto p4 = dxle::point_c<double>(4, 5);//move constructor call
	dxle::point_c<int> p5;//default constructor call
	auto p6 = dxle::point_c<float>(0, 0);
	dxle::point_c<int> p7{};
	cout << "p3:" << p3 << " p4:" << p4 << " p5:" << p5 << " p6:" << p6 << " p7:" << p7 << endl;

	bool is_zero = (0 == static_cast<bool>(p6)) ? true : false;//explicit operator bool call
	if(0 == p7) is_zero = false;

	//contextually converted to bool
	if (p7) is_zero = false;//explicit operator bool call
	cout << "is_zero:" << is_zero << endl;
	//implicitly converted to a type T
	dxle::point_c<int> p8 = dxle::make_point_c(std::pair<int, int>());//convert from std::pair move constructor call
	std::pair<int, int> pa{ 2, 3 };
	dxle::point_c<int> p9 = dxle::make_point_c(pa);//convert from std::pair copy constructor call
	cout << "p8:" << p8 << " p9:" << p9 << endl;

	//abs
	dxle::pointd p10(-2.3, -3.4);
	auto p11 = dxle::abs(p10);
	cout << "abs : p10(src):(" << p10 << ")  result=(" << p11 << ")" << endl;

	//distance
	auto d = dxle::distance(p1, p10);
	cout << "distance : p1(" << p1 << ") p10(" << p10 << ") result=" << d << endl;

	//dot(2D-vector)
	const dxle::pointi p_a1 = { 2, 3 };
	const dxle::pointi p_b1 = { 4, 1 };
	cout << "2D-vcror dot   : a(" << p_a1 << ") b(" << p_b1 << ") result=" << dxle::dot(p_a1, p_b1) << endl;
	cout << "2D-vcror cross : a(" << p_a1 << ") b(" << p_b1 << ") result=" << dxle::cross(p_a1, p_b1) << endl;

	//cast
	const dxle::pointf p12 = { 3.2f, 2.4f };
	auto p13 = static_cast<dxle::pointu8i>(p12);
	cout << "cast : p12(dxle::pointf):(" << p12 << ") result(p13)=(" << p13 << ")" << endl;
	cout << "3, 5 のように入力してください" << endl;
	dxle::pointi in_p;
	std::cin >> in_p;//input
	cout << "in_p:" << in_p << endl;

	//multiplication
	const auto p14 = p13 * 2;
	cout << "multiplication: p13:(" << p13 << ") * 2 = (" << p14 << ")" << endl;
	return 0;
}
