/*
 * matrix.hpp
 *
 *  Created on: Jan 19, 2019
 *      Author: dmarce1
 */

#ifndef OCTOPH_MATRIX_MATRIX_HPP_

#define OCTOPH_MATRIX_MATRIX_HPP_

#include <functional>
#include <octoph/math/matrix/characteristic.hpp>

#include <octoph/math/matrix/mask.hpp>
namespace math {

namespace matrix {

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
		constexpr copy(matrix& me, const A& other) {
			if (!other.template zero<I, J>()) {
				//		printf("M:%i %i %i\n", I, J, mask_.template index<I, J>());
				me.a_[mask_.template index<I, J>()] = other.template get<I, J>();
			}
			if constexpr (I != 0) {
				if constexpr (J != 0) {
					copy<A, I, J - 1>(me, other);
				} else {
					copy<A, I - 1, M - 1>(me, other);
				}
			} else {
				if constexpr (J != 0) {
					copy<A, I, J - 1>(me, other);
				}
			}
		}
	};

	template<std::size_t I1, std::size_t J1>
	void initialize(const std::array<std::array<value_type, M>, N>& init) {
		get<I1, J1>() = init[I1][J1];
		if constexpr (I1 == 0 && J1 == 0) {
		} else if constexpr (I1 == 0) {
			initialize<I1, J1 - 1>(init);
		} else if constexpr (J1 == 0) {
			initialize<I1 - 1, M - 1>(init);
		} else {
			initialize<I1, J1 - 1>(init);
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

	template<class O>
	matrix& operator=(const O& other) {
		copy<matrix, N - 1, M - 1> f(*this, other);
		return *this;
	}

	template<std::size_t I, std::size_t J>
	inline T& get() {
		static_assert(I < N);
		static_assert(J < M);
		return a_[mask_.template index<I, J>()];
	}

	template<std::size_t I, std::size_t J>
	inline T get() const {
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
		//	printf( "%i\n", index(i,j));
		return a_[index(i, j)];
	}

	T& operator()(std::size_t i, std::size_t j) {
//		printf( "%i\n", index(i,j));
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

template<class A, std::size_t I = 0, std::size_t J = 0>
void print(const A& a) {
	constexpr auto N = A::nrow;
	constexpr auto M = A::ncol;
	if constexpr (!A::template zero<I, J>()) {
		printf("%16e ", a(I, J));
	} else {
		printf("%16e ", double(0));
	}
	if constexpr (J == M - 1) {
		printf("\n");
		if constexpr (I == N - 1) {
			printf("\n");
		}
	}
	if constexpr (J != M - 1) {
		print<A, I, J + 1>(a);
	} else {
		if constexpr (I != N - 1) {
			print<A, I + 1, 0>(a);
		}
	}

}

}
}
#endif /* OCTOPH_MATRIX_MATRIX_HPP_ */
