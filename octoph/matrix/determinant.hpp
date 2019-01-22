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

template<class A>
class determinant_type;

template<class A>
determinant_type<A> determinant(const A& a);

template<class A>
class determinant_type {
	static constexpr std::size_t nrow = A::nrow;
	static constexpr std::size_t ncol = A::ncol;
	static_assert(ncol==nrow);
	using value_type = typename A::value_type;
	const A& a_;

private:

	template<class A0, std::size_t I, std::size_t J = ncol - 1>
	struct row_zero {
		static constexpr bool value = A0::template zero<I, J>() && row_zero<A0, I, J - 1>::value;
	};

	template<class A0, std::size_t I>
	struct row_zero<A0, I, 0> {
		static constexpr bool value = A0::template zero<I, 0>();
	};

	template<class A0, std::size_t I = nrow - 1>
	struct some_row_zero {
		static constexpr bool value = row_zero<A0, I>::value || some_row_zero<A0, I - 1>::value;
	};

	template<class A0>
	struct some_row_zero<A0, 0> {
		static constexpr bool value = row_zero<A0, 0>::value;
	};

	template<class A0, std::size_t J, std::size_t I = nrow - 1>
	struct col_zero {
		static constexpr bool value = A0::template zero<I, J>() && col_zero<A0, I, J - 1>::value;
	};

	template<class A0, std::size_t I>
	struct col_zero<A0, 0, I> {
		static constexpr bool value = A0::template zero<0, I>();
	};

	template<class A0, std::size_t J = ncol - 1>
	struct some_col_zero {
		static constexpr bool value = col_zero<A0, J>::value || some_col_zero<A0, J - 1>::value;
	};

	template<class A0>
	struct some_col_zero<A0, 0> {
		static constexpr bool value = col_zero<A0, 0>::value;
	};

public:

	constexpr bool zero() const {
		return some_row_zero<A>::value || some_col_zero<A>::value;
	}

	template<class A0, std::size_t N, std::size_t I>
	struct compute {
		value_type operator()(const A0& a) {
			constexpr int sign = ((I % 2) == 0) ? 1 : -1;
			constexpr std::size_t IM1 = I - 1;
			auto b = comatrix<A, 0, I>(a);
			auto detB = determinant(b);
			compute<A0, N, IM1> next;
			if (!a.template zero<0, I>() && !detB.zero()) {
				if (sign == 1) {
					return next(a) + detB.get() * a.template get<0, I>();
				} else {
					return next(a) + value_type(-1) * detB.get() * a.template get<0, I>();
				}
			} else {
				return next(a);
			}
		}
	};

	template<class A0, std::size_t N>
	struct compute<A0, N, 0> {
		value_type operator()(const A0& a) {
			auto b = comatrix<A, 0, 0>(a);
			auto detB = determinant(b);
			if (!a.template zero<0, 0>() && !detB.zero()) {
				return detB.get() * a.template get<0, 0>();
			} else {
				return value_type(0);
			}
		}
	};

	template<class A0>
	struct compute<A0, 0, 0> {
		value_type operator()(const A0& a) {
			//		printf( "%i %i %e\n", 0, 0, a.template get<0,0>());
			return a.template get<0, 0>();
		}
	};

private:
	determinant_type(const A& a) :
			a_(a) {
	}

public:
	inline value_type get() const {
		compute<A, nrow - 1, nrow - 1> f;
		return f(a_);
	}
	template<class A1>
	friend determinant_type<A1> determinant(const A1& a);
};

template<class A>
determinant_type<A> determinant(const A& a) {
	return determinant_type<A>(a);
}

}
#endif /* OCTOPH_MATRIX_DETERMINANT_HPP_ */
