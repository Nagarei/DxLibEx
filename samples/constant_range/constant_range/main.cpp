#include "dxlibex/utility.hpp"
#include <iostream>
int main() {
	for (int i : dxle::rep(10)) {
		std::cout << i;
	}
}
