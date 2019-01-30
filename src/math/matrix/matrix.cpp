#include <octoph/math/matrix/matrix.hpp>
#include <octoph/math/matrix/product.hpp>
#include <octoph/math/matrix/determinant.hpp>
#include <octoph/math/matrix/add.hpp>

#include <octoph/math/matrix/inverse.hpp>
#include <octoph/math/matrix/transpose.hpp>
#include <octoph/math/math.hpp>
#include <octoph/math/matrix/add.hpp>
using real = math::real;
#include <octoph/math/matrix/QR.hpp>
#include <limits>

#include <octoph/math/polynomial.hpp>
#include <array>

#ifdef TEST_MATRIX

int main() {
	using mask = math::matrix::mask<math::matrix::integer_sequence<int,1,0,0>,math::matrix::integer_sequence<int,0,1,0>,math::matrix::integer_sequence<int,0,0,1>>;
	auto A = math::matrix::matrix<double,2,3>(std::array<std::array<double,3>,2>( { { {1,2,3}, {4,5,6}}}));
	auto C = math::matrix::matrix<double,4,4>(std::array<std::array<double,4>,4>( { { {1,2,3,4}, {5,6,7,8}, {0,10,11,12}, {0,0,15,17}}}));
//	auto C = math::matrix::matrix<double,2,2>(std::array<std::array<double,2>,2>( { { {1,0},
//		                                                                             {-3,-2},
//						 }}));

//	print(math::matrix::copy(math::matrix::product(A,B)));

//print(C);
	for( int d = 0; d < 10000000; d++) {
		auto Q = math::matrix::QR_decomposition(C);
		auto Qn1 = math::matrix::transpose(Q);
		C = copy(product(Qn1,product(C,Q)));
		double eps = 0.0;
		for( int i = 1; i < C.nrow; i++) {
			for( int j = 0; j < i; j++) {
				eps = std::max(eps,std::abs(C(i,j)));
			}
		}
		printf( "%i\n", d);
		print(math::matrix::copy(C));
		double det = math::matrix::determinant(C).get();
		printf( "%e %e\n", eps, std::pow(det, 1.0 / double((A.nrow+1)/2)));
		if( eps < std::abs(std::pow(det, 1.0 / double((A.nrow+1)/2)))*std::pow(0.1,std::numeric_limits<double>::digits10-1)) {
			break;
		}
	}
	printf( "\n");
	return 0;
}

#endif
