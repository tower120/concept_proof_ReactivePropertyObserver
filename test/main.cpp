#include <iostream>

#include "TestPropertyObserver.h"
#include "Benchmark.h"

int main() {
    //TestPropertyObserver().test_all();
    Benchmark().benchmark_all();

	char ch;
	std::cin >> ch;
    return 0;
}