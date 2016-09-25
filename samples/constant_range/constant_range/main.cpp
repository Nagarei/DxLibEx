#include "dxlibex/utility.hpp"
#include <iostream>
int main() {
	for (int i : dxle::rep(10)) {
		std::cout << i;
	}
	auto r = *dxle::rep(10).begin();
	std::cout << r;
}
