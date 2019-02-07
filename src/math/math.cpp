#include <octoph/math/math.hpp>

#include <complex>
#include <octoph/math/containers.hpp>

#include <functional>
#include <octoph/math/polynomial.hpp>
#include <iostream>
#include <hpx/hpx_init.hpp>

constexpr auto N = 3;
constexpr auto L = 10000000;

#include <utility>

struct accesst {
	int begin();
};

struct no_access {
};

struct class1 {
	~class1() {
		printf("1\n");
	}
};
struct class2 {
	class2() = default;
	std::vector<double> v;
	class2(class2&& o) {
		printf( "moving\n");
	}
	class2& operator=(class2&& o) = delete;
	class2(const class2&) = delete;
	class2& operator=(const class2&) = delete;
	~class2() {
		printf("2\n");
	}
};

class2 test() {
	class1 one;
	class2 two;
	return two;
}

#ifdef TEST_MATH
#include <array>

int main(int argc, char* argv[]) {
	test();
	hpx::init(argc, argv);
}

int hpx_main(int argc, char* argv[]) {

	math_container<std::vector<double>> test;

	return hpx::finalize();

}
#endif
