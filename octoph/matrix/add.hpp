/*
 * add.hpp
 *
 *  Created on: Jan 23, 2019
 *      Author: dmarce1
 */

#ifndef OCTOPH_MATRIX_ADD_HPP_
#define OCTOPH_MATRIX_ADD_HPP_

namespace linear {

template<class A, class B>
class add_type {
public:
	static constexpr std::size_t nrow = A::nrow;
	static constexpr std::size_t ncol = A::nrow;
	static_assert(nrow==B::nrow);
	static_assert(ncol==B::ncol);
	static constexpr bool is_matrix = true;
	using value_type = typename std::enable_if<A::is_matrix && B::is_matrix,decltype(typename A::value_type()+typename B::value_type())>::type;
private:
	A a_;
	B b_;

	add_type(const A& a, const B& b) :
			a_(a), b_(b) {

	}

public:

	template<std::size_t I, std::size_t J>
	value_type get() const {
		static_assert((I < nrow)  && (J < ncol));
		if constexpr (zero<I, J>()) {
			return value_type(0);
		} else {
			return a_.template get<I, J>() + b_.template get<I, J>();
		}
	}

//
//	template<std::size_t I, std::size_t J>
//	static constexpr std::size_t op_count() {
//		static_assert((I < nrow)  && (J < ncol));
//		if constexpr (A::template zero<I, J>() || B::template zero<I, J>()) {
//			return 0;
//		} else {
//			return A::template op_count<I,J>() + B::template op_count<I,J>() + 1;
//		}
//	}

	template<std::size_t I, std::size_t J>
	static constexpr bool zero() {
		static_assert(I<nrow);
		static_assert(J<ncol);
		return A::template zero<I, J>() && B::template zero<I, J>();
	}

	template<class AA, class BB>
	friend add_type<AA, BB> add(const AA& a, const BB& b);

};

template<class A, class B>
add_type<A, B> add(const A& a, const B& b) {
	add_type<A, B> p(a, b);
	return p;
}

}

#endif /* OCTOPH_MATRIX_ADD_HPP_ */
