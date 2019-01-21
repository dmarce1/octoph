/*
 * product.hpp
 *
 *  Created on: Jan 20, 2019
 *      Author: dmarce1
 */

#ifndef OCTOPH_MATRIX_PRODUCT_HPP_
#define OCTOPH_MATRIX_PRODUCT_HPP_

namespace linear {

template<class A, class B = A>
struct product {
	static constexpr std::size_t nrow = A::nrow;
	static constexpr std::size_t ncol = B::ncol;
	using value_type = decltype(typename A::value_type() * typename B::value_type());

private:

	static constexpr std::size_t L = B::nrow;
	static_assert(A::ncol == B::nrow);
	const A& a_;
	const B& b_;

	template<std::size_t N, std::size_t M, std::size_t ITER = L>
	struct execute {
		const value_type n_;
		static constexpr std::size_t I = L - ITER;
		using next_execute = execute<N,M,ITER-1>;
		inline execute(const A& a, const B& b) :
				n_(a.template get<N, I>() * b.template get<I, M>() + next_execute(a, b).n_) {
		}
	};

	template<std::size_t N, std::size_t M>
	struct execute<N, M, 1> {
		const value_type n_;
		inline execute(const A& a, const B& b) :
				n_(a.template get<N, L - 1>() * b.template get<L - 1, M>()) {
		}
	};



public:

	product(const A& a, const B& b) :
			a_(a), b_(b) {
	}

	template<std::size_t I, std::size_t J>
	inline value_type get() const {
		execute < I, J > f(a_, b_);
		return f.n_;
	}


	template<std::size_t I, std::size_t J>
	static constexpr bool zero() {
		static_assert(I<nrow);
		static_assert(J<ncol);
		return false;
	}

	static bool zero(int i, int j) {
		return false;
	}



};

}




#endif /* OCTOPH_MATRIX_PRODUCT_HPP_ */
