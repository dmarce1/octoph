#ifndef __CHARACTERISTIC_HPP
#define __CHARACTERISTIC_HPP

#include <octoph/math/polynomial.hpp>
#include <octoph/math/containers.hpp>
#include <octoph/math/matrix/comatrix.hpp>
#include <octoph/math/matrix/matrix.hpp>

namespace math {

namespace matrix {

template<class A, class T, int J>
auto characteristic_polynomial1(const A& a, T& pin) {
	constexpr auto N = A::nrow;
	using type = typename A::value_type;
	printf( " in %i %i\n", N, J);
	polynomial<type, 2> p0;
	p0[0] = a.template get<0, 0>();
	if constexpr (N == 1) {
		return p0.conj(pin).get();
	} else {
		const auto co = comatrix<A, 0, J>(a);
		const auto next_pin = p0.conj(pin).get();
		const auto det_co = characteristic_polynomial1<decltype(co), decltype(next_pin), 0>(a, next_pin).get();
		if constexpr (J < N - 1) {
			if constexpr( J % 2 == 0 ) {
				return (det_co + characteristic_polynomial1<A, decltype(pin), J + 1>(a, pin)).get();
			} else {
				return (det_co - characteristic_polynomial1<A, decltype(pin), J + 1>(a, pin)).get();
			}
		} else {
			return pin.conj(det_co).get();
		}
	}

}

template<std::size_t>
class mask_diagonal;

template<class T, std::size_t N, std::size_t M, class >
struct matrix;

template<class T, std::size_t N>
struct identity: public matrix<T, N, N, mask_diagonal<N> > {

	constexpr identity() {
		for (int i = 0; i < N; i++) {
			(*this)(i, i) = 1;
		}
	}
	identity(identity&&) = default;
	identity(const identity&) = default;
	identity& operator=(identity&&) = default;
	identity& operator=(const identity&) = default;
};

template<class A>
auto characteristic_polynomial(const A& a) {
	constexpr auto N = A::nrow;
	using type = typename A::value_type;
	polynomial<type, 1> p0;
	p0[0] = 1;
	const identity<double, N> delta;
	return characteristic_polynomial1<A, decltype(p0), 0>(a, p0);
}

}

}

#endif
