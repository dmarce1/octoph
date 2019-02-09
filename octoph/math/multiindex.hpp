/*
 * multiindex.hpp
 *
 *  Created on: Feb 8, 2019
 *      Author: dmarce1
 */

#ifndef OCTOPH_MATH_MULTIINDEX_HPP_
#define OCTOPH_MATH_MULTIINDEX_HPP_

#include <utility>

template<int...>
struct multiindex;

template<int I, int J, int ...K>
struct multiindex<I, J, K...> : public std::integer_sequence<int, I, J, K...> {

	template<int L>
	static int get() {
		if constexpr (L == 0) {
			return I;
		} else {
			return multiindex<J, K...>::template get<L - 1>();
		}
	}

	static void print(bool first = true) {
		if (first) {
			printf("\n");
		}
		printf("%i ", I);
		multiindex<J, K...>::print(false);
		if (first) {
			printf("\n");
		}
	}

private:
	template<int, int...>
	struct split_help;

public:

	template<int L>
	using split = typename split_help<L,I,J,K...>::type;

private:

	template<int L, int M, int ...N>
	struct split_help<L,M,N...> {
		template<int ...O>
		using type_help = typename std::conditional<
		L!=0,
		typename split_help<L-1,N...>::template type_help<O...,M>,
		std::pair<multiindex<O...>, multiindex<M,N...>>
		>::type;
		using type = type_help<>;
	};

	template<int L>
	struct split_help<L> {
		template<int ...O>
		using type_help = std::pair<multiindex<O...>, multiindex<>>;
		using type = type_help<>;
	};

};

template<int I>
struct multiindex<I> : public std::integer_sequence<int, I> {

	template<int L>
	static int get() {
		return I;
	}

	static void print(bool first = true) {
		if (first) {
			printf("\n");
		}
		printf("%i ", I);
		if (first) {
			printf("\n");
		}
	}

	template<int L>
	using split = typename std::conditional< L==0, std::pair<multiindex<>, multiindex<I>>, std::pair<multiindex<I>, multiindex<>>>::type;

private:

};

template<>
struct multiindex<> : public std::integer_sequence<int> {

	static void print(bool first = true) {
		if (first) {
			printf("\n");
		}
		if (first) {
			printf("\n");
		}
	}

	template<int L>
	using split =std::pair<multiindex<>, multiindex<>>;
};

template<int ...I, int ...J>
auto multiindex_cat(const multiindex<I...>& a, const multiindex<J...>& b) {
	multiindex<I..., J...> i;
	return i;
}

template<int I, int J, int ...K>
auto reorder_multiindex(const multiindex<K...>& a) {
	if constexpr (I > J) {
		return reorder_multiindex<J, I>(a);
	} else if constexpr (I < J) {
		using type = multiindex<K...>;
		using ab = typename type::template split<I>;
		using bc = typename decltype(ab().second)::template split<J-I>;
		using part1 = decltype(ab().first);
		using part2 = decltype(bc().first);
		using part3 = decltype(bc().second);
		using part2_split = typename part2::template split<1>;
		using part3_split = typename part3::template split<1>;
		return multiindex_cat(part1(),
				multiindex_cat(part3_split().first,
						multiindex_cat(part2_split().second, multiindex_cat(part2_split().first, part3_split().second))));
	} else {
		return a;
	}
}

template<int Dim, int...>
struct multiindex_increment_type;

template<int Dim, int J, int ...K>
struct multiindex_increment_type<Dim, J, K...> {
	template<int ...I>
	using type_help = typename std::conditional<
			J < Dim - 1,
			multiindex<I...,J+1,K...>,
			typename multiindex_increment_type<Dim, K...>::template type_help<I...,0>
	>::type;

	using type = type_help<>;
};

template<int Dim>
struct multiindex_increment_type<Dim> {
	template<int ...I>
	using type_help = void;

	using type = type_help<>;
};

template<int Dim, int J, int ...K>
auto multiindex_increment(const multiindex<J, K...>& a) {
	using T = typename multiindex_increment_type<Dim,J,K...>::type;
	T b;
	return b;
}
;

#endif /* OCTOPH_MATH_MULTIINDEX_HPP_ */
