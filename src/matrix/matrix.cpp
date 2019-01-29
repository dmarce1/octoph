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
#include <limits>

#include <math/polynomial.hpp>
#include <array>
namespace linear {


template<class A, int N>
polynomial<typename A::value_type, N> characteristic_polynomial(const A& a);


template<class A, int N, int I>
polynomial<typename A::value_type, N> helper(const A& a) {
	using T = typename A::value_type;
	constexpr auto sign = (I % 2 == 0) ? +1 : -1;
	const auto co = comatrix<A,0,I>(a);
	const auto p = characteristic_polynomial<A,N-1>(co);
	if( sign == -1 ) {
		for( auto i = 0; i < N-1; i++) {
			p[i] = -p[i];
		}
	}
	if constexpr (I != 0) {
		return p + helper<A, N, I - 1>(a);
	} else {
		return p;
	}
}

template<class A, int N>
polynomial<typename A::value_type, N> characteristic_polynomial(const A& a) {
	using T = typename A::value_type;
	polynomial<T, N> p;

	return p;
}
}

#ifdef TEST_MATRIX

int main() {
	using mask = linear::mask<linear::integer_sequence<int,1,0,0>,linear::integer_sequence<int,0,1,0>,linear::integer_sequence<int,0,0,1>>;
	auto A = linear::matrix<double,2,3>(std::array<std::array<double,3>,2>( { { {1,2,3}, {4,5,6}}}));
	auto C = linear::matrix<double,4,4>(std::array<std::array<double,4>,4>( { { {1,2,3,4}, {5,6,7,8}, {0,10,11,12}, {0,0,15,17}}}));
//	auto C = linear::matrix<double,2,2>(std::array<std::array<double,2>,2>( { { {1,0},
//		                                                                             {-3,-2},
//						 }}));

//	print(linear::copy(linear::product(A,B)));

//print(C);
	for( int d = 0; d < 10000000; d++) {
		auto Q = linear::QR_decomposition(C);
		auto Qn1 = linear::transpose(Q);
		C = copy(product(Qn1,product(C,Q)));
		double eps = 0.0;
		for( int i = 1; i < C.nrow; i++) {
			for( int j = 0; j < i; j++) {
				eps = std::max(eps,std::abs(C(i,j)));
			}
		}
		printf( "%i\n", d);
		print(linear::copy(C));
		double det = linear::determinant(C).get();
		printf( "%e %e\n", eps, std::pow(det, 1.0 / double((A.nrow+1)/2)));
		if( eps < std::abs(std::pow(det, 1.0 / double((A.nrow+1)/2)))*std::pow(0.1,std::numeric_limits<double>::digits10-1)) {
			break;
		}
	}
	printf( "\n");
	return 0;
}

#endif
