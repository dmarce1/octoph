/*
 * tensor.hpp
 *
 *  Created on: Feb 4, 2019
 *      Author: dmarce1
 */

#include <utility>
#include <cstdint>
#include <cmath>
#include <array>
#include <tuple>
#include <utility>

template<int...>
struct multiindex;

template<int I, int J, int ...K>
struct multiindex<I, J, K...> : public std::integer_sequence<int, I, J, K...> {
	using base_type = std::integer_sequence<int, I, J, K...>;

	template<int L>
	static int get() {
		if constexpr (L == 0) {
			return I;
		} else {
			return multiindex<J, K...>::template get<L - 1>();
		}
	}

	template<int L, int M, int ...N>
	struct split {
	private:
		template<int ...O>
		using type_help = typename std::conditional<
				L!=0,
				typename split<L - 1,N...>::template type<O...,M>,
				std::pair<multiindex<O...>, multiindex<M,N...>>
		>::type;
	public:
		using type = type_help<>;
	};

};

template<int I>
struct multiindex<I> : public std::integer_sequence<int, I> {
	using base_type = std::integer_sequence<int, I>;

	template<int L>
	static int get() {
		return I;
	}

};

void test() {
}
