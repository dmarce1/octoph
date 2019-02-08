#include <octoph/math/math.hpp>
#include <octoph/math/containers.hpp>
#include <hpx/hpx_init.hpp>

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

	template<int, int...>
	struct split_help;

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

	template<int L>
	using split = typename split_help<L,I,J,K...>::type;

	static void print(bool first = true) {
		if( first ) {
			printf( "\n");
		}
		printf( "%i ", I);
		multiindex<J,K...>::print(false);
		if( first ) {
			printf( "\n");
		}
	}

	multiindex() = default;
	multiindex(const multiindex&) = default;
	multiindex& operator=(const multiindex&) = default;

};

template<int I>
struct multiindex<I> : public std::integer_sequence<int, I> {
	using base_type = std::integer_sequence<int, I>;

	template<int L>
	static int get() {
		return I;
	}

	template<int L>
	using split = typename std::conditional< L==0, std::pair<multiindex<>, multiindex<I>>, std::pair<multiindex<I>, multiindex<>>>::type;

	static void print(bool first = true) {
		if (first) {
			printf("\n");
		}
		printf("%i ", I);
		if (first) {
			printf("\n");
		}
	}

	multiindex() = default;
	multiindex(const multiindex&) = default;
	multiindex& operator=(const multiindex&) = default;

};

template<>
struct multiindex<> : public std::integer_sequence<int> {
	using base_type = std::integer_sequence<int>;

	template<int L>
	using split =std::pair<multiindex<>, multiindex<>>;

	static void print(bool first = true) {
		if (first) {
			printf("\n");
		}
		if (first) {
			printf("\n");
		}
	}

	multiindex() = default;
	multiindex(const multiindex&) = default;
	multiindex& operator=(const multiindex&) = default;
};

template<int ...I, int ...J>
auto multiindex_cat(const multiindex<I...>& a, const multiindex<J...>& b) {
	multiindex<I..., J...> i;
	return i;
}

template<int I, int J, int ...K>
auto reorder_multiindex(const multiindex<K...>& a) {
	if constexpr( I > J ) {
		return reorder_multiindex<J,I>(a);
	} else if constexpr( I < J ) {
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

#ifdef TEST_MATH

int main(int argc, char* argv[]) {
	hpx::init(argc, argv);
}

int hpx_main(int argc, char* argv[]) {

	multiindex<0,1,2,3,4,5> test;

	reorder_multiindex<4,2>(test).print();

	return hpx::finalize();

}
#endif
