
#include <hpx/hpx_init.hpp>

#include "basis/basis.hpp"

int main(int argc, char* argv[]) {
	hpx::init(argc, argv);
}

int hpx_main(int argc, char* argv[]) {
	return hpx::finalize();
}
