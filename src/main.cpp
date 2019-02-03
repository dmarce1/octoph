
#include <hpx/hpx_init.hpp>

#include <octoph/math/basis/basis.hpp>

#include <octoph/containers/containers.hpp>


int main(int argc, char* argv[]) {
	hpx::init(argc, argv);
}

int hpx_main(int argc, char* argv[]) {
	return hpx::finalize();
}
