/*
 * column.hpp
 *
 *  Created on: Jan 23, 2019
 *      Author: dmarce1
 */

#ifndef OCTOPH_MATRIX_COLUMN_HPP_
#define OCTOPH_MATRIX_COLUMN_HPP_





namespace linear {

template<class A, std::size_t L>
struct column_type {
	static constexpr bool is_matrix = true;
	using value_type = typename std::enable_if<A::is_matrix,typename A::value_type>::type;
	static_assert(L<A::ncol);
	static constexpr std::size_t nrow = A::nrow;
	static constexpr std::size_t ncol = 1;

private:
	const A& a_;
	column_type(const A& a) :
			a_(a) {
	}

public:

	template<std::size_t I, std::size_t J>
	value_type get() const {
		static_assert(J==0);
		static_assert(I<nrow);
		return a_.template get<I, L>();
	}

	template<std::size_t I, std::size_t J>
	static constexpr bool zero() {
		static_assert(J==0);
		static_assert(I<nrow);
		return A::template zero<I, L>();
	}

	template<class B, std::size_t>
	friend auto column(const B& a);

};

template<class A, std::size_t I>
auto column(const A& a) {
	return column_type<A, I>(a);
}
}
#endif /* OCTOPH_MATRIX_COLUMN_HPP_ */
