#include <dxlibex/DxLibEx_basic_types.hpp>
int main() {
	DxLibEx::point_c<int> p1(3, 2);
	DxLibEx::point_c<int> p2{ 3, 5 };
	p2 = p1;//copy assignment operator call
	p1 = DxLibEx::point_c<int>(4, 5);//move assignment operator call
	auto p3 = p2;//copy constructor call
	auto p4 = DxLibEx::point_c<double>(4, 5);//move constructor call
	DxLibEx::point_c<int> p6;//default constructor call
	auto p7 = DxLibEx::point_c<float>(0);//convert from nullptr constructor call
	DxLibEx::point_c<int> p8 = 0;//convert from nullptr constructor call
	//auto p7 = DxLibEx::point_c<int>(1);//error C2440: '<function-style-cast>': 'int' から 'DxLibEx::point_c<int,nullptr>' に変換できません。
	bool is_zero = (0 == static_cast<bool>(p6)) ? true : false;//explicit operator bool call
	if(0 == p7) is_zero = false;

	//contextually converted to bool
	if (p7) is_zero = false;//explicit operator bool call
	//implicitly converted to a type T
	DxLibEx::point_c<int> p9 = std::pair<int, int>();//convert from std::pair move constructor call
	std::pair<int, int> pa{ 2, 3 };
	DxLibEx::point_c<int> p10 = pa;//convert from std::pair copy constructor call
	return 0;
}
