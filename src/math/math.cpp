#include <octoph/math/math.hpp>


#include <complex>
#include <octoph/math/containers.hpp>

#include <functional>
#include <octoph/math/polynomial.hpp>
#ifdef TEST_MATH
#include <array>
int main() {

	using namespace math;
	std::array<double,5> a, b;
	auto test = operation(std::function<double(double,double)>([]( const double& a, const double& b){return a+b;}), a, b);


auto p = math::polynomial<double,5>(std::array<double,5>( { {1, 4, 1,0,-1}}));
p.all_roots();

}
#endif
