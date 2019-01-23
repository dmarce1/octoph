/*
 * QR.hpp
 *
 *  Created on: Jan 21, 2019
 *      Author: dmarce1
 */

#ifndef OCTOPH_MATRIX_QR_HPP_
#define OCTOPH_MATRIX_QR_HPP_

#include <matrix/matrix.hpp>

namespace linear {

template<class A, std::size_t L>
struct column_type {
	static constexpr bool is_matrix = true;
	using value_type = typename std::enable_if<A::is_matrix,typename A::value_type>::type;
	static_assert(L<A::ncol);
	static constexpr std::size_t nrow = A::nrow;
	static constexpr std::size_t ncol = 1;

private:
	const A& a_;

public:
	column_type(const A& a) :
			a_(a) {
	}

	template<std::size_t I, std::size_t J>
	value_type get() const {
		static_assert(J==0);
		static_assert(I<nrow);
		return a_.template get<I, L>();
	}

	template<std::size_t I, std::size_t J>
	static constexpr bool zero() {
		static_assert(J==0);
		static_assert(I<nrow);
		return A::template zero<I, L>();
	}

};

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

template<class T, std::size_t N>
struct QR_decomposition_type {
	static constexpr bool is_matrix = true;
	using value_type = typename std::enable_if<T::is_matrix,typename T::value_type>::type;

	template<class A, class U, std::size_t I>
	struct execute {
		auto operator()(const A& a0, U& u0) const {
			const auto u = column_type<T, I>(u0);
			const auto a = block_columns_left_type<T, I + 1>(a0);
			const auto Ta = transpose(a);
			const auto Tau = product(Ta, u);
			const auto Tu = transpose(u);
			const auto Tuu = product(Tu, u);
			const auto invTuu = inverse(Tuu);
			const auto TauinvTuu = product(Tau, invTuu);
			const auto TTauinvTuu = transpose(TauinvTuu);
			const auto du = product(u, TTauinvTuu);
			return du;
		}

	};

	QR_decomposition_type(T& A) {
		execute<T, T, 1> test;
		test(A, A);
	}

};
}

#endif /* OCTOPH_MATRIX_QR_HPP_ */
