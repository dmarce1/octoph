/*
 * determinant.hpp
 *
 *  Created on: Jan 20, 2019
 *      Author: dmarce1
 */

#ifndef OCTOPH_MATRIX_DETERMINANT_HPP_
#define OCTOPH_MATRIX_DETERMINANT_HPP_

#include "matrix/comatrix.hpp"

namespace linear {

template<class A, std::size_t>
class determinant_type;

template<class A>
determinant_type<A, A::ncol> determinant(const A& a);

template<class A, std::size_t N>
class determinant_type {
	using value_type = typename std::enable_if<A::is_matrix,typename A::value_type>::type;
	static constexpr std::size_t nrow = N;
	static constexpr std::size_t ncol = N;
	static_assert(N==A::ncol && N==A::nrow);
	const A& a_;

private:

	template<std::size_t I, std::size_t J>
	static constexpr bool row_zero() {
		if constexpr (A::template zero<I, J>()) {
			if constexpr (J > 0) {
				return row_zero<I, J - 1>();
			} else {
				return true;
			}
		} else {
			return false;
		}
	}

	template<std::size_t I, std::size_t J>
	static constexpr bool col_zero() {
		if constexpr (A::template zero<J, I>()) {
			if constexpr (J > 0) {
				return col_zero<I, J - 1>();
			} else {
				return true;
			}
		} else {
			return false;
		}
	}

	template<std::size_t I>
	static constexpr bool determinant_is_zero() {
		if constexpr (col_zero<I, N - 1>()) {
			return true;
		} else if constexpr (row_zero<I, N - 1>()) {
			return true;
		} else if constexpr (I == 0) {
			return false;
		} else {
			return determinant_is_zero<I - 1>();
		}
	}
public:

	static constexpr bool zero() {
		return determinant_is_zero<nrow - 1>();
	}

	template<std::size_t M, std::size_t I>
	auto compute() const {
		constexpr int sign = ((I % 2) == 0) ? 1 : -1;
		constexpr std::size_t IM1 = I - 1;
		auto b = comatrix<A, 0, I>(a_);
		auto detB = determinant(b);
		if constexpr (I == 0) {
			if constexpr (M == 0) {
				return a_.template get<0, 0>();
			} else {
				if constexpr (!A::template zero<0, 0>() && !detB.zero()) {
					return detB.get() * a_.template get<0, 0>();
				} else {
					return value_type(0);
				}
			}
		} else {
			if constexpr (!A::template zero<0, I>() && !detB.zero()) {
				if constexpr (sign == 1) {
					return compute<M, IM1>() + detB.get() * a_.template get<0, I>();
				} else {
					return compute<M, IM1>() + value_type(-1) * detB.get() * a_.template get<0, I>();
				}
			} else {
				return compute<M, IM1>();
			}
		}
	}

private:
	determinant_type(const A& a) :
			a_(a) {
	}

public:
	auto get() const {
		if constexpr (!zero()) {
			return compute<nrow - 1, nrow - 1>();
		} else {
			return value_type(0);
		}
	}
	template<class A1>
	friend determinant_type<A1, A1::ncol> determinant(const A1& a);
};

template<class A>
class determinant_type<A, 0> {
	static constexpr std::size_t nrow = 0;
	static constexpr std::size_t ncol = 0;
	static_assert(0==A::ncol && 0==A::nrow);
	using value_type = typename A::value_type;
private:

public:

	static constexpr bool zero() {
		return false;
	}
private:
	determinant_type(const A& a) {
	}

public:
	inline constexpr value_type get() const {
		return value_type(1);
	}
	template<class A1>
	friend determinant_type<A1, A1::ncol> determinant(const A1& a);
};

template<class A>
determinant_type<A, A::ncol> determinant(const A& a) {
	return determinant_type<A, A::ncol>(a);
}

}
#endif /* OCTOPH_MATRIX_DETERMINANT_HPP_ */
