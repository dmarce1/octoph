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
	std::array<std::array<double,2>,2> b_ = {{{1,1},{1,1}}};
	auto b = linear::matrix<double,2,2>(b_);
	auto c = linear::column<decltype(b),0>(b);

	printf( "Hello\n");
	std::array<std::array<double,3>,3> a = {{ {12,-51,  4},
			                                   {6,167,-68},
											  {-4, 24,-41}}};
	linear::matrix<double,3,3> A(a);
	print(A);

	print(linear::QR_decomposition(A));
	return 0;
}

#endif
