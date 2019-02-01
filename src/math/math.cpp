#include <octoph/math/math.hpp>

#include <complex>
#include <octoph/math/containers.hpp>

#include <functional>
#include <octoph/math/polynomial.hpp>
#include <iostream>
#include <hpx/hpx_init.hpp>


constexpr auto N = 3;
constexpr auto L = 10000000;

#ifdef TEST_MATH
#include <array>

int main(int argc, char* argv[]) {
	hpx::init(argc, argv);
}

int hpx_main(int argc, char* argv[]) {
	using namespace math::containers;
	std::array<std::array<std::vector<double>, N>, N> A, B, C;
	for( int n = 0; n < N; n++) {
		for( int m = 0; m < N; m++) {
			A[n][m].resize(L);
			B[n][m].resize(L);
			C[n][m].resize(L);
			for( int l = 0; l < L; l++) {
				A[n][m][l] = double(rand() % 2000 - 1000);
				B[n][m][l] = double(rand() % 2000 - 1000);
				C[n][m][l] = 0.0;
			}
		}
	}
	long long nops;
	auto start = clock();
	for( int n = 0; n < N; n++) {
		for( int m = 0; m < N; m++) {
			auto tmp = A[n][0] * 4.0
		//	tmp.store(C[n][m]);
		//	nops += tmp.op_count();
		}
	}
	auto dt = (clock() - start) / double(CLOCKS_PER_SEC);
	std::cout << (nops/dt) << "\n";

	auto p = math::polynomial<double,5>(std::array<double,5>( { {1, 4, 1,0,-1}}));
	p.all_roots();
	return hpx::finalize();

}
#endif
