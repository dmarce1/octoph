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

template<class T, std::size_t N, std::size_t M, class MASK = mask_all_true<N, M> >
struct matrix {

	using value_type = T;
	static constexpr std::size_t nrow = N;
	static constexpr std::size_t ncol = M;
	static constexpr MASK mask_ = MASK();
	static constexpr std::size_t size = mask_.size;

private:
	std::array<T, size> a_;

	template<class A, std::size_t I, std::size_t J>
	friend class copy;

	template<class A, std::size_t I, std::size_t J>
	struct copy {
		inline copy(matrix& me, const A& other) {
			constexpr bool this_on = !zero<I, J>();
			const bool other_on = !other.template zero<I, J>();
			static_assert(!(!this_on && other_on), "linear::copy - array shape mismatch");
			constexpr bool last = (I == 0) && (J == 0);
			constexpr std::size_t NI = (I == 0) ? 0 : ((J == 0) ? (I - 1) : I);
			constexpr std::size_t NJ = (J == 0) ? (M - 1) : J - 1;
			if (!last) {
				copy<A, NI, NJ> f(me, other);
			}
			me.a_[mask_.template index<I, J>()] = other.template get<I, J>();
		}
	};

	template<std::size_t I1, std::size_t J1>
	void initialize(const std::array<std::array<value_type, M>, N>& init) {
		constexpr std::size_t J2 = (J1 == 0) ? 0 : ((I1 == 0) ? (J1 - 1) : J1);
		constexpr std::size_t I2 = (I1 == 0) ? (N - 1) : (I1 - 1);
		if (!((J1 == 0) && (I1 == 0))) {
			initialize<I2, J2>(init);
		}
		if (mask_.template get<I1, J1>()) {
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

	template<class A>
	matrix& operator=(const A& other) {
		copy<A, N - 1, M - 1> f(*this, other);
		return *this;
	}

	template<std::size_t I, std::size_t J>
	inline T get() const {
		static_assert(I < N);
		static_assert(J < M);
		return mask_.template get<I, J>() ? a_[mask_.template index<I, J>()] : 0;
	}

	template<std::size_t I, std::size_t J>
	inline T& get() {
		static T dummy = T(0);
		static_assert(I < N);
		static_assert(J < M);
		return mask_.template get<I, J>() ? a_[mask_.template index<I, J>()] : dummy;
	}

	value_type operator()(std::size_t i, std::size_t j) const {
		assert(i < N);
		assert(j < M);
		if (zero(i, j)) {
			return value_type(0);
		} else {
			return a_[mask_.index(i, j)];
		}
	}

	value_type& operator()(std::size_t i, std::size_t j) {
		assert(i < N);
		assert(j < M);
		if (zero(i, j)) {
			static value_type dummy = value_type(0);
			return dummy;
		} else {
			return a_[mask_.index(i, j)];
		}
	}

	template<std::size_t I, std::size_t J>
	static constexpr bool zero() {
		static_assert(I<N);
		static_assert(J<M);
		return (mask_.template get<I, J>() == false);
	}

	bool zero(int i, int j) const {
		assert(i < N);
		assert(j < M);
		return (mask_.get(i, j) == false);
	}

};

template<class A>
void print(const A& a) {
	for (int i = 0; i < A::nrow; i++) {
		for (int j = 0; j < A::ncol; j++) {
			printf("%16e ", a(i, j).get());
		}
		printf("\n");
	}
}

}

#endif /* OCTOPH_MATRIX_MATRIX_HPP_ */
