/*
 * comatrix.hpp
 *
 *  Created on: Jan 20, 20
 *      Author: dmarce1
 */

#ifndef OCTOPH_MATRIX_COMATRIX_HPP_
#define OCTOPH_MATRIX_COMATRIX_HPP_

namespace linear {
template<class A, int N, int M>
struct comatrix_type {
	static constexpr bool is_matrix = true;
	using value_type = typename std::enable_if<A::is_matrix,typename A::value_type>::type;
	static constexpr std::size_t nrow = A::nrow - 1;
	static constexpr std::size_t ncol = A::ncol - 1;
	const A& a_;
	comatrix_type(const A& a) :
			a_(a) {
	}
public:

	template<std::size_t I, std::size_t J>
	inline auto get() const {
		constexpr std::size_t P = I < N ? I : I + 1;
		constexpr std::size_t Q = J < M ? J : J + 1;
		return a_.template get<P, Q>();
	}

	template<std::size_t I, std::size_t J>
	static constexpr bool zero() {
		static_assert(I<nrow);
		static_assert(J<ncol);
		return A::template zero<I < N ? I : I + 1, J < M ? J : J + 1>();
	}

	template<class A1, std::size_t I1, std::size_t J1>
	friend comatrix_type<A1, I1, J1> comatrix(const A1& a);
};

template<class A, std::size_t I, std::size_t J>
comatrix_type<A, I, J> comatrix(const A& a) {
	comatrix_type<A, I, J> b(a);
	return b;
}

}

#endif /* OCTOPH_MATRIX_COMATRIX_HPP_ */
