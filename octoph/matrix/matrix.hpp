/*
 * matrix.hpp
 *
 *  Created on: Jan 19, 2019
 *      Author: dmarce1
 */

#ifndef OCTOPH_MATRIX_MATRIX_HPP_

#define OCTOPH_MATRIX_MATRIX_HPP_

#include <functional>

#include "matrix/mask.hpp"


namespace linear {


template<class A, std::size_t I, std::size_t J>
struct init_indexes;


template<class T, std::size_t N, std::size_t M, class MASK = mask_all_true<N, M> >
struct matrix {
	static constexpr bool is_matrix = true;
	using value_type = T;
	static constexpr std::size_t nrow = N;
	static constexpr std::size_t ncol = M;
	static constexpr MASK mask_ = MASK();
	static constexpr std::size_t size = mask_.size;

private:
	std::array<T, size> a_;

	template<class, std::size_t, std::size_t>
	friend class init_indexes;

	static std::array<std::array<std::size_t, M>, N> indexes;

	template<class A, std::size_t I, std::size_t J>
	struct copy {
		inline copy(matrix& me, const A& other) {
			constexpr bool this_on = !zero<I, J>();
			const bool other_on = !other.template zero<I, J>();
			static_assert(!(!this_on && other_on), "linear::copy - array shape mismatch");
			constexpr bool last = (I == 0) && (J == 0);
			constexpr std::size_t NI = (I == 0) ? 0 : ((J == 0) ? (I - 1) : I);
			constexpr std::size_t NJ = (J == 0) ? (M - 1) : J - 1;
			if constexpr (!last) {
				copy<A, NI, NJ> f(me, other);
			}
			me.a_[mask_.template index<I, J>()] = other.template get<I, J>();
		}
	};

	template<std::size_t I1, std::size_t J1>
	void initialize(const std::array<std::array<value_type, M>, N>& init) {
		constexpr std::size_t J2 = (J1 == 0) ? 0 : ((I1 == 0) ? (J1 - 1) : J1);
		constexpr std::size_t I2 = (I1 == 0) ? (N - 1) : (I1 - 1);
		if constexpr (!((J1 == 0) && (I1 == 0))) {
			initialize<I2, J2>(init);
		}
		if constexpr (mask_.template get<I1, J1>()) {
			get<I1, J1>() = init[I1][J1];
		}
	}

	std::size_t index(std::size_t i, std::size_t j) const {
		static init_indexes<matrix, N - 1, M - 1> init_indexes_instance(*this);
		return indexes[i][j];
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

	matrix& operator=(const matrix& other) {
		copy<matrix, N - 1, M - 1> f(*this, other);
		return *this;
	}

	template<std::size_t I, std::size_t J>
	inline auto get() const {
		static_assert(I < N);
		static_assert(J < M);
		if constexpr (zero<I, J>()) {
			return value_type(0);
		} else {
			return a_[mask_.template index<I, J>()];
		}
	}

	template<std::size_t I, std::size_t J>
	static constexpr bool zero() {
		static_assert(I<N);
		static_assert(J<M);
		return (mask_.template get<I, J>() == false);
	}

	T operator()(std::size_t i, std::size_t j) const {
		return a_[index(i, j)];
	}

	T& operator()(std::size_t i, std::size_t j) {
		return a_[index(i, j)];
	}

	template<class A>
	friend auto copy(const A&);
};

template<class T>
auto copy(const T& A) {
	using mask_type = mask_derived<T>;
	using return_type = matrix<typename T::value_type, T::nrow, T::ncol, mask_type>;
	return return_type(A);
}

template<class T, std::size_t N, std::size_t M, class MASK>
std::array<std::array<std::size_t, M>, N> matrix<T, N, M, MASK>::indexes;


template<class A, std::size_t I, std::size_t J>
struct init_indexes {
	init_indexes(const A& a) {
		init_indexes<A, I, J - 1> next(a);
		A::indexes[I][J] = a.mask_.template index<I, J>();
	}
};

template<class A, std::size_t I>
struct init_indexes<A, I, 0> {
	init_indexes(const A& a) {
		init_indexes<A, I - 1, A::ncol - 1> next(a);
		A::indexes[I][0] = a.mask_.template index<I, 0>();
	}
};

template<class A>
struct init_indexes<A, 0, 0> {
	init_indexes(const A& a) {
		A::indexes[0][0] = a.mask_.template index<0, 0>();
	}
};


}
#endif /* OCTOPH_MATRIX_MATRIX_HPP_ */
