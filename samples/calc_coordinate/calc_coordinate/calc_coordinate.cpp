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
	auto p6 = dxle::point_c<float>(0);
	dxle::point_c<int> p7{ 0 };
	dxle::point_c<float> p7_1{ 0 };
	auto p7_2 = dxle::point_c<int>(1);
	cout << "p3:" << p3 << " p4:" << p4 << " p5:" << p5 << " p6:" << p6 << " p7:" << p7 << " p7_1:" << p7_1 << " p7_2:" << p7_2 << endl;

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
	dxle::pointd p11(-2.3, -3.4);
	auto p12 = dxle::abs(p11);
	cout << "p11:" << p11 << " p12:" << p12 << endl;

	//cast
	auto p10 = static_cast<dxle::pointu8i>(dxle::pointf(3.2f, 2.4f));
	cout << "p10:" << p10 << endl;
	cout << "3, 5 のように入力してください" << endl;
	dxle::pointi in_p;
	std::cin >> in_p;
	cout << "in_p:" << in_p << endl;
	{
		char c[100];
		std::cin >> c;
	}
	return 0;
}
