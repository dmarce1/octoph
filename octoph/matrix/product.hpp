/*
 * product_type.hpp
 *
 *  Created on: Jan 20, 2019
 *      Author: dmarce1
 */

#ifndef OCTOPH_MATRIX_PRODUCT_HPP_
#define OCTOPH_MATRIX_PRODUCT_HPP_

namespace linear {

template<class A, class B = A>
struct product_type {
	static constexpr std::size_t nrow = A::nrow;
	static constexpr std::size_t ncol = B::ncol;
	using value_type = decltype(typename A::value_type() * typename B::value_type());

private:

	static constexpr std::size_t L = B::nrow;
	static_assert(A::ncol == B::nrow);
	A a_;
	B b_;

	template<std::size_t N, std::size_t M, std::size_t ITER = L>
	struct execute {
		inline value_type operator()(const A& a, const B& b) const {
			static constexpr std::size_t I = L - ITER;
			const execute<N, M, ITER - 1> next;
			if constexpr( !(a.template zero<N,I>() || b.template zero<I,M>() )) {
				return a.template get<N, I>() * b.template get<I, M>() + next(a, b);
			} else {
				return next(a, b);
			}
		}
	};

	template<std::size_t N, std::size_t M>
	struct execute<N, M, 1> {
		inline value_type operator()(const A& a, const B& b) const {
			static constexpr std::size_t I = L - 1;
			if constexpr( !(a.template zero<N,I>() || b.template zero<I,M>() )) {
				return a.template get<N, I>() * b.template get<I, M>();
			} else {
				return value_type(0);
			}
		}
	};

	product_type(const A& a, const B& b) :
			a_(a), b_(b) {

	}


public:
	template<std::size_t I, std::size_t J>
	value_type get() const {
		static_assert(I<nrow);
		static_assert(J<ncol);
		execute<I, J> f;
		return f(a_, b_);
	}

	template<std::size_t I, std::size_t J>
	static constexpr bool zero() {
		static_assert(I<nrow);
		static_assert(J<ncol);
		return false;
	}


	template <class AA, class BB>
	friend product_type<AA,BB> product(const AA& a, const BB& b);
};



template<class A>
struct product_type<A,typename A::value_type> {
	static constexpr std::size_t nrow = A::nrow;
	static constexpr std::size_t ncol = A::ncol;
	using value_type = typename A::value_type;

private:

	A a_;
	value_type b_;


	product_type(const A& a, const value_type& b) :
			a_(a), b_(b) {

	}


public:
	template<std::size_t I, std::size_t J>
	value_type get() const {
		static_assert(I<nrow);
		static_assert(J<ncol);
		return a_.template get<I,J>() * b_;
	}

	template<std::size_t I, std::size_t J>
	static constexpr bool zero() {
		static_assert(I<nrow);
		static_assert(J<ncol);
		return A::template zero<I,J>();
	}


	template <class AA, class BB>
	friend product_type<AA,BB> product(const AA& a, const BB& b);
};


template<class A, class B>
product_type<A,B> product(const A& a, const B& b) {
	product_type<A,B> p(a,b);
	return p;
}

}

#endif /* OCTOPH_MATRIX_PRODUCT_HPP_ */
