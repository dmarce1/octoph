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
	static constexpr std::size_t ncol = sizeof...(Args);
private:
	static constexpr bool m_[ncol] = { Args... };
public:

	template<int I>
	static constexpr bool get() {
		static_assert(I < ncol);
		return m_[I];
	}

	template<std::size_t I>
	static constexpr std::size_t index() {
		static_assert(I < ncol);
		constexpr std::size_t IM1 = (I == 0) ? 0 : (I - 1);
		return (I == 0) ? 0 : index<IM1>() + (get<I>() ? 1 : 0);
	}

	static constexpr std::size_t size = index<ncol - 1>() + 1;

};

template<class ...Args>
class mask {

	template<std::size_t N>
	using row_type = typename std::remove_reference<decltype(std::get<N>(std::tuple<Args...>()))>::type;

	static constexpr std::size_t nrow = sizeof...(Args);
	static constexpr std::size_t ncol = row_type<0>::N;
	static bool values[nrow][ncol];
	static std::size_t indexes[nrow][ncol];

	template<std::size_t I1, std::size_t J1>
	static void gen_dynamic() {
		constexpr std::size_t J2 = (J1 == 0) ? 0 : ((I1 == 0) ? (J1 - 1) : J1);
		constexpr std::size_t I2 = (I1 == 0) ? (nrow - 1) : (I1 - 1);
		if (!((I1 == 0) && (J1 == 0))) {
			gen_dynamic<I2, J2>();
		}
		values[I1][J1] = get<I1, J1>();
		indexes[I1][J1] = index<I1, J1>();
	}

	friend class mask_init;
	struct mask_init {
		mask_init() {
			mask::gen_dynamic<nrow - 1, ncol - 1>();
		}
	};
	const static mask_init mask_init_instance;

	template<std::size_t I>
	static constexpr std::size_t row_index_sum() {
		static_assert(I<nrow);
		constexpr std::size_t IM1 = (I == 0) ? 0 : (I - 1);
		return (I == 0) ? 0 : row_type<IM1>::size() + (get<I>() ? 1 : 0);
	}

public:

	template<std::size_t N, std::size_t M>
	static constexpr bool get() {
		static_assert(N<nrow);
		static_assert(M<ncol);
		return row_type<N>::template get<M>();
	}

	static const bool get(std::size_t i, std::size_t j) {
		return values[i][j];
	}

	template<std::size_t I, std::size_t J>
	static constexpr std::size_t index() {
		static_assert(I<nrow);
		static_assert(J<ncol);
		constexpr std::size_t IM1 = (I == 0) ? 0 : (I - 1);
		return row_type<I>::template index<J>() + ((I == 0) ? 0 : row_index_sum<IM1>());
	}

	static const std::size_t index(std::size_t i, std::size_t j) {
		return indexes[i][j];
	}

	static constexpr std::size_t size = index<nrow - 1, ncol - 1>() + 1;

};

template<class ...Args>
const typename mask<Args...>::mask_init mask_init_instance;

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

	static bool get(std::size_t, std::size_t) {
		return true;
	}

	static std::size_t index(std::size_t i, std::size_t j) {
		return i * M + j;
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
	T a_[size];

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
			auto& same = me.a_[MASK::template index<I, J>()];
			same = this_on ? (other_on ? other.template get<I, J>() : T(0)) : same;
		}
	};

	template<std::size_t I1, std::size_t J1>
	void initialize(const std::array<std::array<value_type, M>, N>& init) {
		constexpr std::size_t J2 = (J1 == 0) ? 0 : ((I1 == 0) ? (J1 - 1) : J1);
		constexpr std::size_t I2 = (I1 == 0) ? (N - 1) : (I1 - 1);
		if (!((J1 == 0) && (I1 == 0))) {
			initialize<I2, J2>(init);
		}
		if (MASK::template get<I1, J1>()) {
			get<I1, J1>() = init[I1][J1];
		}
	}

public:

	matrix() = default;

	matrix(const std::array<std::array<value_type, M>, N>& init) {
		initialize<N - 1, M - 1>(init);
	}

	template<class A>
	matrix(const A& other) {
		copy<A, N - 1, M - 1> f(*this, other);
	}

	template<std::size_t I, std::size_t J>
	inline const T& get() const {
		static_assert(I < N);
		static_assert(J < M);
		static T dummy = T(0);
		return MASK::template get<I, J>() ? a_[MASK::template index<I, J>()] : dummy;
	}

	template<std::size_t I, std::size_t J>
	inline T& get() {
		static T dummy = T(0);
		static_assert(I < N);
		static_assert(J < M);
		return MASK::template get<I, J>() ? a_[MASK::template index<I, J>()] : dummy;
	}

	value_type operator()(std::size_t i, std::size_t j) const {
		return a_[MASK::index(i, j)];
	}

	value_type& operator()(std::size_t i, std::size_t j) {
		return a_[MASK::index(i, j)];
	}

};

template<class A, int N, int M>
struct comatrix {
	static constexpr std::size_t nrow = A::nrow - 1;
	static constexpr std::size_t ncol = A::ncol - 1;
	using value_type = typename A::value_type;
	const A& a_;
public:
	comatrix(const A& a) :
			a_(a) {
	}
	template<std::size_t I, std::size_t J>
	inline value_type get() const {
		constexpr std::size_t P = I < N ? I : I + 1;
		constexpr std::size_t Q = J < M ? J : J + 1;
		return a_.template get<P, Q>();
	}
};

template<class A>
class determinant {
	static constexpr std::size_t nrow = A::nrow;
	static constexpr std::size_t ncol = A::ncol;
	static_assert(ncol==nrow);
	using value_type = typename A::value_type;
	const A& a_;

	template<std::size_t I>
	inline value_type compute() const {
		constexpr value_type sign = ((I % 2) == 0) ? value_type(1) : -value_type(1);
		constexpr std::size_t IM1 = (I == 0) ? 0 : I - 1;
		using B = comatrix<A,0,I>;
		B b(a_);
		determinant<B> detB(b);
		return (B::nrow == 0) ?
				value_type(1) : (sign * detB.get() * a_.template get<0, I>() + ((I == 0) ? value_type(0) : compute<IM1>()));
	}

public:
	determinant(const A& a) :
			a_(a) {
	}
	inline value_type get() const {
		return compute<nrow>();
	}
};

template<class A, class B = A, class MASK0 = mask_all_true<A::nrow, B::ncol> >
struct product {
	using MASK = MASK0;
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
		const value_type n_;
		static constexpr std::size_t I = L - ITER;
		using next_execute = execute<N,M,ITER-1>;
		inline execute(const A& a, const B& b) :
				n_(a.template get<N, I>() * b.template get<I, M>() + next_execute(a, b).n_) {
		}
	};

	template<std::size_t N, std::size_t M>
	struct execute<N, M, 1> {
		const value_type n_;
		inline execute(const A& a, const B& b) :
				n_(a.template get<N, L - 1>() * b.template get<L - 1, M>()) {
		}
	};

public:

	product(const A& a, const B& b) :
			a_(a), b_(b) {
	}

	template<std::size_t I, std::size_t J>
	inline value_type get() const {
		execute < I, J > f(a_, b_);
		return f.n_;
	}

};

template<class A>
void print(const A& a) {
	for (int i = 0; i < A::nrow; i++) {
		for (int j = 0; j < A::ncol; j++) {
			printf("%16e ", a(i, j));
		}
		printf("\n");
	}
}

}

#endif /* OCTOPH_MATRIX_MATRIX_HPP_ */
