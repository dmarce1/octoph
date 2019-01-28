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
	static constexpr std::size_t N = 1;

	template<std::size_t I>
	static constexpr T get() {
		static_assert(I==0);
		return Last;
	}
};

template<class A, std::size_t I, std::size_t J, std::size_t last = 0>
constexpr std::size_t find_index() {
	constexpr auto on = A::template get<I, J>() ? 1 : 0;
	if constexpr (A::N == 1) {
		if constexpr (J == 0) {
			return 0;
		} else {
			const auto i = last + find_index<A, 0, J - 1,on>();
			static_assert(i>=0);
			static_assert(i < A::N * A::M);
			return i;
		}
	} else if constexpr (A::M == 1) {
		if constexpr (I == 0) {
			constexpr auto i = last;
			static_assert(i>=0);
			static_assert(i < A::N * A::M);
			return i;
		} else {
			const auto i = last + find_index<A, I - 1, 0,on>();
			static_assert(i>=0);
			static_assert(i < A::N * A::M);
			return i;
		}
	} else if constexpr (I > 0) {
		if constexpr (J > 0) {
			const auto i = last + find_index<A, I, J - 1,on>();
			static_assert(i>=0);
			static_assert(i < A::N * A::M);
			return i;
		} else {
			const auto i = last + find_index<A, I - 1, A::M - 1,on>();
			static_assert(i>=0);
			static_assert(i < A::N * A::M);
			return i;
		}
	} else {
		if constexpr (J > 0) {
			const auto i = last + find_index<A, I, J - 1,on>();
			static_assert(i>=0);
			static_assert(i < A::N * A::M);
			return i;
		} else {
			const auto i = last;
			static_assert(i>=0);
			static_assert(i < A::N * A::M);
			return i;
		}
	}
}

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
		return find_index<mask, I, J>();
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
		return find_index<mask, I, J>();
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

	static constexpr std::size_t size = find_index<mask_derived, N - 1, M - 1>() + 1;

	template<std::size_t I, std::size_t J>
	static constexpr bool get() {
		static_assert(I<N);
		static_assert(J<M);
		return !(matrix_type::template zero<I, J>());
	}
	template<std::size_t I, std::size_t J>
	static constexpr std::size_t index() {
		return find_index<mask_derived, I, J>();
	}

}
;

}

#endif /* OCTOPH_MATRIX_MASK_HPP_ */
