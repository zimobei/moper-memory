#include "allocator.h"
#include "m_string.h"
#include <string>
#include <iostream>
#include <time.h>
using namespace Memory;

int main() {

	clock_t timer;

	for (std::size_t i = 0; i < 10; ++i) {
		timer = clock();
		for (std::size_t j = 0; j < 1000000; ++j) {
			MString strs("This is a test for MString class which uses Allocator<char>");
			strs += " Push Strings";
		}
		timer = clock() - timer;
		std::cout << " MString time = " << timer << std::endl;
	}

	std::cout << std::endl;

	for (std::size_t i = 0; i < 10; ++i) {
		timer = clock();
		for (std::size_t j = 0; j < 1000000; ++j) {
			std::string strs("This is a test for string class ***************************");
			strs += " Push Strings";
		}
		timer = clock() - timer;
		std::cout << " std::string time = " << timer << std::endl;
	}
	return 0;
}