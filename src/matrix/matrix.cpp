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




template<std::size_t N>
class sorted {
	template< class...Args, class S = std::make_index_sequence<sizeof...(Args)>>
	void func(std::tuple<Args...>& t, S& s ) {

	}

};

#ifdef TEST_MATRIX

int main() {
	std::array<std::array<real,2>,2> a = { { {1,0}, {0,1}}};

	using mask = linear::mask<linear::integer_sequence<std::size_t,1,1>,linear::integer_sequence<std::size_t,1,1>>;
	auto A = linear::matrix<real,2,2,mask>(a);
//auto A = linear::matrix<real,3,3>(a);

	auto B = linear::inverse(A);
	auto C = linear::copy(B);

	math::real::local_counter counter;
	counter.start();
	B = linear::inverse(A);
//	C = linear::copy(B);
	counter.stop();
	printf( "%i\n", int(counter.read()));
	decltype(A) b, c;
	linear::QR_decomposition(A);
	return 0;
}

#endif
