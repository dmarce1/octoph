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
	auto B = linear::matrix<double,3,2>(std::array<std::array<double,2>,3>( { { {1,8}, {9,10}, {11,12}}}));
	auto C = linear::matrix<double,3,3>(std::array<std::array<double,3>,3>( { { {12,-51,4},
		                                                                             {6,167,-68},
																					 {-4,24,-41}}}));

//	print(linear::copy(linear::product(A,B)));

//	print(C);
	print(linear::copy(C));
	print(linear::copy(linear::QR_decomposition(C)));
	return 0;
}


#endif
