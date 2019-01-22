#include "matrix/matrix.hpp"
#include "matrix/product.hpp"
#include "matrix/determinant.hpp"


#include <matrix/inverse.hpp>
#include <math/math.hpp>

using real = math::real;

#ifdef TEST_MATRIX

int main() {
	std::array<std::array<real,3>,3> a = { { {1,0,0}, {0,1,0}, {0,0,1}}};

	using mask = linear::mask<linear::integer_sequence<std::size_t,1,0,0>,linear::integer_sequence<std::size_t,0,1,0>,linear::integer_sequence<std::size_t,0,0,1>>;
	auto A = linear::matrix<real,3,3,mask>(a);
	//auto A = linear::matrix<real,3,3>(a);

	math::real::local_counter counter;
	counter.start();
	auto B = linear::inverse(A);
	linear::matrix<real,3,3> C = B;
	counter.stop();
	printf( "%i\n", int(counter.read()));

	return 0;
}

#endif
