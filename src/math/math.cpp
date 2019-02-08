#include <octoph/math/math.hpp>
#include <octoph/math/containers.hpp>
#include <hpx/hpx_init.hpp>



#ifdef TEST_MATH

int main(int argc, char* argv[]) {
	hpx::init(argc, argv);
}

int hpx_main(int argc, char* argv[]) {
	using namespace math;
	math_array<double,4000> t1;
	math_array<double,4000> t2;
	const auto t3 = (t1 + t2) * 5.0;
	const auto t5 = (t3 * t2) * t1;
	math_array<double,4000> t4 = t5;
	return hpx::finalize();

}
#endif
