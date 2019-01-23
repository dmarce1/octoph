/*
 * neg.hpp
 *
 *  Created on: Jan 23, 2019
 *      Author: dmarce1
 */

#ifndef OCTOPH_MATRIX_NEG_HPP_
#define OCTOPH_MATRIX_NEG_HPP_


namespace linear {

template<class A>
class neg_type {
public:
	static constexpr std::size_t nrow = A::nrow;
	static constexpr std::size_t ncol = A::nrow;
	static constexpr bool is_matrix = true;

	using value_type = typename std::enable_if<A::is_matrix, decltype(-(typename A::value_type()))>::type;

private:
	A a_;

	neg_type(const A& a) :
			a_(a) {

	}

public:

	template<std::size_t I, std::size_t J>
	value_type get() const {
		static_assert(I<nrow);
		static_assert(J<ncol);
		if constexpr (zero<I, J>()) {
			return value_type(0);
		} else {
			return -a_.template get<I, J>();
		}
	}

	template<std::size_t I, std::size_t J>
	static constexpr bool zero() {
		static_assert(I<nrow);
		static_assert(J<ncol);
		return A::template zero<I, J>();
	}


	template<class AA>
	friend neg_type<AA> neg(const AA& a);

};

template<class A>
neg_type<A> neg(const A& a) {
	neg_type<A> p(a);
	return p;
}

}




#endif /* OCTOPH_MATRIX_NEG_HPP_ */
