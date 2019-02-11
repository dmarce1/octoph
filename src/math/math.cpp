#include <octoph/math/math.hpp>
#include <octoph/math/containers.hpp>
#include <hpx/hpx_init.hpp>

#include <octoph/math/tensor.hpp>

#ifdef TEST_MATH

int main(int argc, char* argv[]) {
	hpx::init(argc, argv);
}

int hpx_main(int argc, char* argv[]) {

	math::tensor_access<3,4,math::SYMMETRIC,math::SYMMETRIC> test;

	test.multiindex_to_flat_index(math::multiindex<0,0,0>());

	return hpx::finalize();

}
#endif
