/*
 * matrix.hpp
 *
 *  Created on: Jan 19, 2019
 *      Author: dmarce1
 */

#ifndef OCTOPH_MATRIX_MATRIX_HPP_
#define OCTOPH_MATRIX_MATRIX_HPP_

#include <functional>

namespace linear {

template<class T, std::size_t N, std::size_t M>

struct matrix {

	using value_type = T;
	static constexpr std::size_t nrow = N;
	static constexpr std::size_t ncol = M;

private:
	T A[N][M];

	template<class A, std::size_t I, std::size_t J>
	friend class copy;

	template<class A, std::size_t I, std::size_t J>
	struct copy {
		inline copy(matrix& me, const A& other) {
			copy<A, I, J - 1> f(me, other);
			me.A[I][J] = other.template get<I, J>();
		}
	};

	template<class A, std::size_t I>
	struct copy<A, I, 0> {
		inline copy(matrix& me, const A& other) {
			copy<A, I - 1, M - 1> f(me, other);
			me.A[I][0] = other.template get<I, 0>();
		}
	};

	template<class A>
	struct copy<A, 0, 0> {
		inline copy(matrix& me, const A& other) {
			me.A[0][0] = other.template get<0, 0>();
		}
	};

public:

	matrix() = default;

	template<class A>
	matrix(const A& other) {
		copy<A, N - 1, M - 1> f(*this, other);
	}

	template<std::size_t I, std::size_t J>
	inline const T& get() const {
		static_assert(I < N);
		static_assert(J < M);
		return A[I][J];
	}

	template<std::size_t I, std::size_t J>
	inline T& get() {
		static_assert(I < N);
		static_assert(J < M);
		return A[I][J];
	}

};

template<class A, class B = A>
struct product {

	static constexpr std::size_t nrow = A::nrow;
	static constexpr std::size_t ncol = B::ncol;
	using value_type = decltype(typename A::value_type() * typename B::value_type());

private:

	static constexpr std::size_t L = B::nrow;
	static_assert(A::ncol == B::nrow);
	const A& a_;
	const B& b_;

	template<std::size_t N, std::size_t M, std::size_t ITER = L>
	struct execute {
		static constexpr std::size_t I = L - ITER;
		const value_type n_;
		using next_execute = execute<N,M,ITER-1>;
		execute(const A& a, const B& b) :
				n_(a.template get<N, I>() * b.template get<I, M>() + next_execute(a, b).n_) {
		}
	};

	template<std::size_t N, std::size_t M>
	struct execute<N, M, 1> {
		static constexpr std::size_t I = L - 1;
		const value_type n_;
		execute(const A& a, const B& b) :
				n_(a.template get<N, I>() * b.template get<I, M>()) {
		}
	};

public:

	product(const A& a, const B& b) :
			a_(a), b_(b) {
	}

	template<std::size_t I, std::size_t J>
	inline value_type get() const {
		execute<I, J, L> f(a_, b_);
		return f.n_;
	}

};

}

#endif /* OCTOPH_MATRIX_MATRIX_HPP_ */
