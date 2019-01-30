/*
 * product_type.hpp
 *
 *  Created on: Jan 20, 2019
 *      Author: dmarce1
 */

#ifndef OCTOPH_MATRIX_PRODUCT_HPP_
#define OCTOPH_MATRIX_PRODUCT_HPP_
namespace math {

namespace matrix {

template<class A, class B = A>
struct product_type {

	static constexpr bool is_matrix = true;
	static constexpr std::size_t nrow = A::nrow;
	static constexpr std::size_t ncol = B::ncol;
	using value_type = typename A::value_type;
private:

	static constexpr std::size_t L = B::nrow;
	static_assert(A::ncol == B::nrow);

	A a_;
	B b_;

	template<auto N, auto M, auto ITER = L>
	auto execute(const A& a, const B& b) const {
		static constexpr std::size_t I = L - ITER;
		if constexpr (ITER == 0) {
			return value_type(0);
		} else {
			if constexpr (!(a.template zero<N, I>() || b.template zero<I, M>())) {
				return a.template get<N, I>() * b.template get<I, M>() + execute<N, M, ITER - 1>(a, b);
			} else {
				return execute<N, M, ITER - 1>(a, b);
			}
		}
	}

	template<class A0, class B0, std::size_t N, std::size_t M, std::size_t ITER = L>
	static constexpr bool zero_entry() {
		if constexpr (ITER == 0) {
			return true;
		} else {
			return (A0::template zero<N, L - ITER>() || B0::template zero<L - ITER, M>()) ?
					zero_entry<A0, B0, N, M, ITER - 1>() : false;
		}
	}

	product_type(const A& a, const B& b) :
			a_(a), b_(b) {

	}

public:

	template<std::size_t I, std::size_t J>
	auto get() const {
		static_assert(I<nrow);
		static_assert(J<ncol);
		if constexpr (zero<I, J>()) {
			return value_type(0);
		} else {
			return execute<I, J>(a_, b_);
		}
	}

	template<std::size_t I, std::size_t J>
	static constexpr bool zero() {
		static_assert(I<nrow);
		static_assert(J<ncol);
		return zero_entry<A, B, I, J>();
	}

	template<class AA, class BB>
	friend product_type<AA, BB> product(const AA& a, const BB& b);
};

template<class A, class B>
product_type<A, B> product(const A& a, const B& b) {
	product_type<A, B> p(a, b);
	return p;
}
}


}

#endif /* OCTOPH_MATRIX_PRODUCT_HPP_ */
