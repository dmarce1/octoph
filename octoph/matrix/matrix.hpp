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

template<bool ...Args>
class row_mask {
public:
	static constexpr std::size_t ncols = sizeof...(Args);
private:
	static constexpr bool m_[ncols] = { Args... };
public:
	template<int I>
	static constexpr bool get() {
		static_assert(I < ncols);
		return m_[I];
	}

	template<std::size_t I>
	static constexpr std::size_t index() {
		static_assert(I < ncols);
		constexpr std::size_t IM1 = (I == 0) ? 0 : (I - 1);
		return (I == 0) ? 0 : index<IM1>() + (get<I>() ? 1 : 0);
	}

	static constexpr std::size_t size = index<ncols - 1>() + 1;

};

template<class ...Args>
class mask {
	static constexpr std::size_t nrows = sizeof...(Args);

	template<std::size_t N>
	using row_type = typename std::remove_reference<decltype(std::get<N>(std::tuple<Args...>()))>::type;

	static constexpr std::size_t ncols = row_type<0>::N;

	template<std::size_t I>
	static constexpr std::size_t row_index_sum() {
		static_assert(I<nrows);
		constexpr std::size_t IM1 = (I == 0) ? 0 : (I - 1);
		return (I == 0) ? 0 : row_type<IM1>::size() + (get<I>() ? 1 : 0);
	}

public:
	template<std::size_t N, std::size_t M>
	static constexpr bool get() {
		static_assert(N<nrows);
		static_assert(M<ncols);
		return row_type<N>::template get<M>();
	}

	template<std::size_t I, std::size_t J>
	static constexpr std::size_t index() {
		static_assert(I<nrows);
		static_assert(J<ncols);
		constexpr std::size_t IM1 = (I == 0) ? 0 : (I - 1);
		return row_type<I>::template index<J>() + ((I == 0) ? 0 : row_index_sum<IM1>());
	}

	static constexpr std::size_t size = index<nrows - 1, ncols - 1>() + 1;

};

template<std::size_t N, std::size_t M>
struct mask_all_true {
	template<std::size_t I, std::size_t J>
	static constexpr bool get() {
		static_assert(I<N);
		static_assert(J<M);
		return true;
	}

	template<std::size_t I, std::size_t J>
	static constexpr std::size_t index() {
		static_assert(I<N);
		static_assert(J<M);
		return I * M + J;
	}

	static constexpr std::size_t size = N * M;

};

template<class T, std::size_t N, std::size_t M, class MASK = mask_all_true<N, M> >
struct matrix {

	using value_type = T;
	static constexpr std::size_t nrow = N;
	static constexpr std::size_t ncol = M;
	static constexpr std::size_t size = MASK::size;

private:
	T A[size];

	template<class A, std::size_t I, std::size_t J>
	friend class copy;

	template<class A, std::size_t I, std::size_t J>
	struct copy {
		inline copy(matrix& me, const A& other) {
			constexpr bool this_on = MASK::template get<I, J>();
			constexpr bool other_on = A::MASK::template get<I, J>();
			static_assert(!(!this_on && other_on), "linear::copy - array shape mismatch");
			constexpr bool last = (I == 0) && (J == 0);
			constexpr std::size_t NI = (I == 0) ? (N - 1) : (I - 1);
			constexpr std::size_t NJ = (I == 0) ? ((J == 0) ? 0 : (J - 1)) : J;
			if (!last) {
				copy<A, NI, NJ> f(me, other);
			}
			const auto& same = me.A[MASK::template index<I, J>];
			me.A[MASK::template index<I, J>] = this_on ? (other_on ? other.template get<I, J>() : T(0)) : same;
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
		return MASK::template get<I, J>() ? A[MASK::template index<I, J>()] : T(0);
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
		static constexpr std::size_t NEXT = ITER == 0 ? 0 : ITER - 1;
		using next_execute = execute<N,M,NEXT>;
		inline execute(const A& a, const B& b) :
				n_(a.template get<N, I>() * b.template get<I, M>() + (ITER == 0) ? value_type(0) : next_execute(a, b).n_) {
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
