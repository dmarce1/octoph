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


}
#endif /* OCTOPH_MATRIX_DETERMINANT_HPP_ */
