/*
 * transpose.hpp
 *
 *  Created on: Jan 21, 2019
 *      Author: dmarce1
 */

#ifndef OCTOPH_MATRIX_TRANSPOSE_HPP_
#define OCTOPH_MATRIX_TRANSPOSE_HPP_

namespace linear {

template<class A>
struct transpose_type {
	static constexpr std::size_t nrow = A::ncol;
	static constexpr std::size_t ncol = A::nrow;
	using value_type = typename A::value_type;

private:
	A a_;


	transpose_type(const A& a) :
			a_(a) {

	}


public:
	template<std::size_t I, std::size_t J>
	value_type get() const {
		static_assert(I<nrow);
		static_assert(J<ncol);
		return a_.template get<J,I>();
	}

	template<std::size_t I, std::size_t J>
	constexpr bool zero() const {
		static_assert(I<nrow);
		static_assert(J<ncol);
		return A::template zero<J,I>();
	}

	template <class AA>
	friend transpose_type<AA> transpose(const AA& a);
};


template<class A>
transpose_type<A> transpose(const A& a) {
	transpose_type<A> p(a);
	return p;
}

}




#endif /* OCTOPH_MATRIX_TRANSPOSE_HPP_ */
