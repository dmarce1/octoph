/*
 * mask.hpp
 *
 *  Created on: Jan 20, 2019
 *      Author: dmarce1
 */

#ifndef OCTOPH_MATRIX_MASK_HPP_
#define OCTOPH_MATRIX_MASK_HPP_

#include <cassert>

namespace linear {

template<class T, T First, T ...Rest>
class integer_sequence;

template<class T, class IS1, class IS2>
class integer_sequence_cat {
public:
	template<T ...Args1, T ...Args2>
	static integer_sequence<T, Args1..., Args2...> helper(integer_sequence<T, Args1...> &&, integer_sequence<T, Args2...> &&);
	using type = decltype(helper(IS1(),IS2()));
};

template<class T, class S, T value>
struct integer_sequence_inc {
public:

	template<T ...Args>
	static integer_sequence<T, value + Args...> helper(integer_sequence<T, Args...> &&);
	using type = decltype(helper(S()));

};

template<class T, T First, T ...Rest>
class integer_sequence: public std::integer_sequence<T, First, Rest...> {
public:
	using type = T;
	using next_sequence = integer_sequence<T,Rest...>;
	static constexpr std::size_t N = 1 + sizeof...(Rest);

	using indexes = typename integer_sequence_cat<T,integer_sequence<T,First>,typename integer_sequence_inc<T, typename next_sequence::indexes,First>::type>::type;
	template<std::size_t I>
	static constexpr T get() {
		static_assert(I < N);
		constexpr std::array<T, N> values = { First, Rest... };
		return values[I];
	}
};

template<class T, T Last>
class integer_sequence<T, Last> : public std::integer_sequence<T, Last> {
public:
	using type = T;
	using indexes = integer_sequence<T,Last>;
	static constexpr std::size_t N = 1;

	template<std::size_t I>
	static constexpr T get() {
		static_assert(I==0);
		return Last;
	}
};

template<class First, class ...Rest>
struct mask {
	static constexpr bool is_mask = true;
	using next_mask = mask<Rest...>;
	using T = typename First::type;
	using type = typename integer_sequence_cat<T,First,typename next_mask::type>::type;
	static constexpr std::size_t N = 1 + sizeof...(Rest);
	static constexpr std::size_t M = First::N;

	template<std::size_t I, std::size_t J>
	static constexpr bool get() {
		static_assert(I<N);
		static_assert(J<M);
		return type::template get<I * M + J>();
	}

	template<std::size_t I, std::size_t J>
	static constexpr std::size_t index() {
		static_assert(I<N);
		static_assert(J<M);
		return type::indexes::template get<I * M + J>() - 1;
	}

	static constexpr std::size_t size = index<N - 1, M - 1>() + 1;
};

template<class Last>
class mask<Last> {
public:
	static constexpr bool is_mask = true;
	using T = typename Last::type;
	using type = Last;
	static constexpr std::size_t N = 1;
	static constexpr std::size_t M = Last::N;

	template<std::size_t I, std::size_t J>
	static constexpr bool get() {
		static_assert(I==0);
		static_assert(J<M);
		return type::template get<I * M + J>();
	}

	template<std::size_t I, std::size_t J>
	static constexpr std::size_t index() {
		static_assert(I==0);
		static_assert(J<M);
		return type::indexes::template get<I * M + J>() - 1;
	}
	static constexpr std::size_t size = index<N - 1, M - 1>() + 1;

};

template<std::size_t N0, std::size_t M0>
class mask_all_true {
	static constexpr bool is_mask = true;
public:
	static constexpr std::size_t N = N0;
	static constexpr std::size_t M = M0;
	static constexpr std::size_t size = N * M;

	template<std::size_t I, std::size_t J>
	static constexpr bool get() {
		static_assert(I<N);
		static_assert(J<M);
		return true;
	}

	template<std::size_t I, std::size_t J>
	static constexpr std::size_t index() {
		static_assert(I<N);
		static_assert(J<M);
		return I * M + J;
	}

};

template<std::size_t N0>
class mask_diagonal {
	static constexpr bool is_mask = true;
public:
	static constexpr std::size_t N = N0;
	static constexpr std::size_t size = N;

	template<std::size_t I, std::size_t J>
	static constexpr bool get() {
		static_assert(I<N);
		static_assert(J<N);
		return I == J;
	}

	template<std::size_t I, std::size_t J>
	static constexpr std::size_t index() {
		static_assert(I<N);
		static_assert(J<N);
		return I;
	}

};

template<class A>
class mask_derived {
public:

	static constexpr bool is_mask = true;
	using matrix_type = typename std::enable_if<A::is_matrix,A>::type;
	static constexpr std::size_t N = A::nrow;
	static constexpr std::size_t M = A::ncol;

	template<std::size_t I, std::size_t J, class T>
	struct index_helper {
		constexpr index_helper() {
		}
		using prev = index_helper<I-1,J,int>;
		static constexpr std::size_t index = prev::index + (A::template zero<I, J>() ? 0 : 1);
	};

	template<std::size_t J, class T>
	struct index_helper<0, J, T> {
		constexpr index_helper() {
		}
		using prev = index_helper<N - 1,J-1,int>;
		static constexpr std::size_t index = prev::index + (A::template zero<0, J>() ? 0 : 1);
	};

	template<class T>
	struct index_helper<0, 0, T> {
		constexpr index_helper() {
		}
		static constexpr std::size_t index = A::template zero<0, 0>() ? 0 : 1;
	};

	static constexpr std::size_t size = N * M;

	template<std::size_t I, std::size_t J>
	static constexpr bool get() {
		static_assert(I<N);
		static_assert(J<M);
		return !(matrix_type::template zero<I, J>());
	}

	template<std::size_t I, std::size_t J>
	static constexpr std::size_t index() {
		static_assert(I<N);
		static_assert(J<M);
		return index_helper<I, J, int>::index - 1;
	}

};

}

#endif /* OCTOPH_MATRIX_MASK_HPP_ */
