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

	/************************************************
	**************       point_c       **************
	*************************************************/
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

	//dot(2D-vector), cross(2D-vector)
	const dxle::pointi p_a1 = { 2, 3 };
	const dxle::pointi p_b1 = { 4, 1 };
	cout << "2D-vcror dot   : a(" << p_a1 << ") b(" << p_b1 << ") result=" << dxle::dot(p_a1, p_b1) << endl;
	cout << "2D-vcror cross : a(" << p_a1 << ") b(" << p_b1 << ") result=" << dxle::cross(p_a1, p_b1) << endl;

	//cast
	const dxle::pointf p12 = { 3.2f, 2.4f };
	auto p13 = static_cast<dxle::pointu8i>(p12);
	cout << "cast : p12(dxle::pointf):(" << p12 << ") result(p13)=(" << p13 << ")" << endl;

	//istream
	cout << "3, 5 のように入力してください" << endl;
	dxle::pointi in_p;
	std::cin >> in_p;//input
	cout << "in_p:" << in_p << endl;

	//multiplication
	const auto p14 = p13 * 2;
	cout << "multiplication: p13:(" << p13 << ") * 2 = (" << p14 << ")" << endl;

	//convert to point_c
	const auto s15 = static_cast<dxle::sizei>(in_p);
	cout << "s15:" << s15 << endl;

	/************************************************
	**************       size_c        **************
	*************************************************/
	dxle::size_c<int> s1(3, 2);
	dxle::sizei s2{ 3, 5 };
	cout << "s1:" << s1 << " s2:" << s2 << endl;

	s2 = s1;//copy assignment operator call
	s1 = dxle::size_c<int>(4, 5);//move assignment operator call
	cout << "s1:" << s1 << " s2:" << s2 << endl;

	auto s3 = s2;//copy constructor call
	auto s4 = dxle::size_c<double>(4, 5);//move constructor call
	dxle::size_c<int> s5;//default constructor call
	auto s6 = dxle::size_c<float>(0, 0);
	dxle::size_c<int> s7{};
	cout << "s3:" << s3 << " s4:" << s4 << " s5:" << s5 << " s6:" << s6 << " s7:" << s7 << endl;

	bool is_zero2 = (0 == static_cast<bool>(s6)) ? true : false;//explicit operator bool call
	if (0 == s7) is_zero2 = false;

	//contextually converted to bool
	if (s7) is_zero2 = false;//explicit operator bool call
	cout << "is_zero:" << is_zero2 << endl;
	//implicitly converted to a type T
	dxle::size_c<int> s8 = dxle::make_size_c(std::pair<int, int>());//convert from std::pair move constructor call
	std::pair<int, int> pa2{ 2, 3 };
	dxle::size_c<int> s9 = dxle::make_size_c(pa2);//convert from std::pair copy constructor call
	cout << "s8:" << s8 << " s9:" << s9 << endl;

	//abs
	dxle::sized s10(-2.3, -3.4);
	auto s11 = dxle::abs(s10);
	cout << "abs : s10(src):(" << s10 << ")  result=(" << s11 << ")" << endl;

	//cast
	const dxle::sizef s12 = { 3.2f, 2.4f };
	auto s13 = static_cast<dxle::sizeui>(s12);
	cout << "cast : s12(dxle::sizef):(" << s12 << ") result(s13)=(" << s13 << ")" << endl;

	//istream
	cout << "3, 5 のように入力してください" << endl;
	dxle::sizei in_s;
	std::cin >> in_s;//input
	cout << "in_p:" << in_s << endl;

	//multiplication
	const auto s14 = s13 * 2U;
	cout << "multiplication: s13:(" << s13 << ") * 2 = (" << s14 << ")" << endl;

	//convert to point_c
	const auto p15 = static_cast<dxle::pointi>(in_s);
	cout << "p15:" << p15 << endl;


	/************************************************
	**************  point_c & size_c   **************
	*************************************************/
	const auto c1 = p14 + s14;
	const auto c2 = p14 - s13;
	const auto c3 = s14 + p14;
	const auto c4 = s14 - p13;
	cout << "c1:" << c1 << " c2:" << c2 << " c3:" << c3 << " c4:" << c4 << endl;
	return 0;
}
