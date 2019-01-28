#include "matrix/matrix.hpp"
#include "matrix/product.hpp"
#include "matrix/determinant.hpp"
#include "matrix/add.hpp"

#include <matrix/inverse.hpp>
#include <matrix/transpose.hpp>
#include <math/math.hpp>
#include "matrix/add.hpp"
using real = math::real;
#include <matrix/QR.hpp>

#ifdef TEST_MATRIX

int main() {
	using mask = linear::mask<linear::integer_sequence<int,1,0,0>,linear::integer_sequence<int,0,1,0>,linear::integer_sequence<int,0,0,1>>;
	auto A = linear::matrix<double,2,3>(std::array<std::array<double,3>,2>( { { {1,2,3}, {4,5,6}}}));
	auto C = linear::matrix<double,3,3>(std::array<std::array<double,3>,3>( { { {1,8,-1}, {9,10,2}, {11,12,3}}}));

//	print(linear::copy(linear::product(A,B)));

	print(C);
	for( int d = 0; d < 200; d++) {
		auto Q = linear::QR_decomposition(C);
		auto Qn1 = linear::transpose(Q);
		C = copy(product(Qn1,product(C,Q)));
		print(linear::copy(C));
	}
	return 0;
}


#endif
