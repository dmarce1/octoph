/*
 * QR.hpp
 *
 *  Created on: Jan 21, 2019
 *      Author: dmarce1
 */

#ifndef OCTOPH_MATRIX_QR_HPP_
#define OCTOPH_MATRIX_QR_HPP_

#include <octoph/math/matrix/matrix.hpp>
#include <octoph/math/matrix/add.hpp>
#include <octoph/math/matrix/neg.hpp>
#include <octoph/math/matrix/column.hpp>

namespace math {

namespace matrix {
namespace detail {

template<class A, std::size_t L>
struct block_columns_left_type {
	static constexpr bool is_matrix = true;
	using value_type = typename std::enable_if<A::is_matrix,typename A::value_type>::type;
	static_assert(L<=A::ncol);
	static constexpr std::size_t nrow = A::nrow;
	static constexpr std::size_t ncol = A::ncol;

private:
	const A& a_;

public:
	block_columns_left_type(const A& a) :
			a_(a) {
	}

	template<std::size_t I, std::size_t J>
	value_type get() const {
		static_assert(J<ncol);
		static_assert(I<nrow);
		if constexpr (J < L) {
			return value_type(0);
		} else if (A::template zero<I, L>()) {
			return 0;
		} else {
			return a_.template get<I, J>();
		}
	}

	template<std::size_t I, std::size_t J>
	static constexpr bool zero() {
		static_assert(J<ncol);
		static_assert(I<nrow);
		if constexpr (J < L) {
			return false;
		} else {
			return A::template zero<I, L>();
		}
	}

};

template<class A, class U, class V, std::size_t I = 1>
auto QR_helper(const A& a0, U& u0, V& v) {
	constexpr auto N = A::nrow;
	static_assert(I>=1);
	if constexpr (I < N) {
		const auto u = column<U, I - 1>(u0);
		const auto a = detail::block_columns_left_type<A, I>(a0);
		const auto Ta = transpose(a);
		const auto Tau = product(Ta, u);
		const auto Tu = transpose(u);
		const auto Tuu = copy(product(Tu, u));
		const auto invTuu = inverse(Tuu);
		const auto TauinvTuu = product(Tau, invTuu);
		const auto TTauinvTuu = transpose(TauinvTuu);
		const auto du = product(u, TTauinvTuu);
		const auto negdu = neg(du);
		const auto newU = copy(add(u0, negdu));
		v(I - 1, I - 1) = Tuu(0, 0);
		return QR_helper<A, decltype(newU), V, I + 1>(a0, newU, v);
	} else {
		const auto u = column<U, I - 1>(u0);
		const auto Tu = transpose(u);
		const auto Tuu = copy(product(Tu, u));
		v(I - 1, I - 1) = Tuu(0, 0);
		return copy(u0);
	}
}
}

template<class A>
auto QR_decomposition(const A& a) {
	using type = typename A::value_type;
	constexpr auto N = A::nrow;
	matrix<type, N, N> V;
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			V(i, j) = 0.0;
		}
	}
	auto U = a;
	auto newU = detail::QR_helper<A, decltype(U), decltype(V)>(a, U, V);
	for (int i = 0; i < N; i++) {
		V(i, i) = std::sqrt(V(i, i));
	}
	return copy(product(newU, inverse(V)));
}
}
}
#endif /* OCTOPH_MATRIX_QR_HPP_ */
