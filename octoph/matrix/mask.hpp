/*
 * mask.hpp
 *
 *  Created on: Jan 20, 2019
 *      Author: dmarce1
 */

#ifndef OCTOPH_MATRIX_MASK_HPP_
#define OCTOPH_MATRIX_MASK_HPP_

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
	static constexpr std::array<T,N> values_ = { First, Rest... };

	using indexes = typename integer_sequence_cat<T,integer_sequence<T,First>,typename integer_sequence_inc<T, typename next_sequence::indexes,First>::type>::type;
	template<std::size_t I>
	static constexpr T get() {
		static_assert(I < N);
		return values_[I];
	}

	static T get(std::size_t i) {
		return values_[i];
	}
};

template<class T, T First, T ...Rest>
constexpr std::array<T,integer_sequence<T, First, Rest...>::N> integer_sequence<T, First, Rest...>::values_;

template<class T, T Last>
class integer_sequence<T, Last> : public std::integer_sequence<T, Last> {
public:
	using type = T;
	using indexes = integer_sequence<T,Last>;
	static constexpr std::size_t N = 1;
	static constexpr std::array<T,1> values_ = { Last };

	template<std::size_t I>
	static constexpr T get() {
		static_assert(I==0);
		return values_[0];
	}

	static T get(std::size_t i) {
		return values_[0];
	}
};

template<class First, class ...Rest>
struct mask {
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

	static bool get(std::size_t i, std::size_t j) {
		return type::get(i * M + j);
	}

	template<std::size_t I, std::size_t J>
	static constexpr std::size_t index() {
		static_assert(I<N);
		static_assert(J<M);
		return type::indexes::template get<I * M + J>() - 1;
	}

	static std::size_t index(std::size_t i, std::size_t j) {
		return type::indexes::get(i * M + j) - 1;
	}
	static constexpr std::size_t size = index<N - 1, M - 1>() + 1;
	static void print() {
		for (int i = 0; i < N; i++) {
			for (int j = 0; j < M; j++) {
				printf("%4i ", int(index(i, j)));
			}
			printf("\n");
		}
	}
};

template<class Last>
class mask<Last> {
public:
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

	static bool get(std::size_t i, std::size_t j) {
		return type::get(i * M + j);
	}

	template<std::size_t I, std::size_t J>
	static constexpr std::size_t index() {
		static_assert(I==0);
		static_assert(J<M);
		return type::indexes::template get<I * M + J>() - 1;
	}

	static std::size_t index(std::size_t i, std::size_t j) {
		return type::indexes::get(i * M + j) - 1;
	}
	static constexpr std::size_t size = index<N - 1, M - 1>() + 1;

	static void print() {
		for (int i = 0; i < N; i++) {
			for (int j = 0; j < M; j++) {
				printf("%4i ", int(index(i, j)));
			}
			printf("\n");
		}
	}

};

template<std::size_t N0, std::size_t M0>
class mask_all_true {
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

	static bool get(std::size_t i, std::size_t j) {
		return true;
	}

	template<std::size_t I, std::size_t J>
	static constexpr std::size_t index() {
		static_assert(I<N);
		static_assert(J<M);
		return I * M + J;
	}

	static std::size_t index(std::size_t i, std::size_t j) {
		return i * M + j;
	}
};

}

#endif /* OCTOPH_MATRIX_MASK_HPP_ */
