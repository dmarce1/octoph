#include <octoph/math/math.hpp>


#include <complex>
#include <octoph/math/containers.hpp>

#include <functional>
#include <octoph/math/polynomial.hpp>
#include <iostream>
#ifdef TEST_MATH
#include <array>
int main() {

	using namespace math;
	std::array<double,5> a, b, c;
	auto test = a + b;
	auto test2 = b + test;
	c = test2;
	std::cout << test2.op_count() << "\n";


auto p = math::polynomial<double,5>(std::array<double,5>( { {1, 4, 1,0,-1}}));
p.all_roots();

}
#endif
