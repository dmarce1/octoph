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

	template<class A0, std::size_t N,  std::size_t I>
	struct compute {
		value_type operator()(const A0& a) {
			constexpr value_type sign = ((I % 2) == 0) ? value_type(1) : -value_type(1);
			constexpr std::size_t IM1 = I - 1;
			auto b = comatrix<A, 0, I>(a);
		 	auto detB = determinant(b);
			compute<A0,N,IM1> next;
			auto rc = next(a) + sign * detB.get() * a.template get<0,I>();
//			printf( "%i %i %e\n", N, I, rc);
			return rc;
		}
	};

	template<class A0, std::size_t N>
	struct compute<A0, N, 0> {
		value_type operator()(const A0& a) {
			auto b = comatrix<A, 0, 0>(a);
			auto detB = determinant(b);
			auto rc = detB.get() * a.template get<0,0>();
	//		printf( "%i %i %e\n", N, 0, rc);
			return rc;
		}
	};


	template<class A0>
	struct compute<A0, 0, 0> {
		value_type operator()(const A0& a) {
	//		printf( "%i %i %e\n", 0, 0, a.template get<0,0>());
			return a.template get<0,0>();
		}
	};

private:
	determinant_type(const A& a) :
			a_(a) {
	}

public:
	inline value_type get() const {
		compute<A, nrow-1, nrow - 1> f;
		return f(a_);;
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
