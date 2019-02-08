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

	template<class L>
	using split = multiindex;

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

#ifdef TEST_MATH

int main(int argc, char* argv[]) {
	hpx::init(argc, argv);
}

int hpx_main(int argc, char* argv[]) {

	multiindex<1,2,3,4,5> test;
	test.print();

	using split = typename decltype(test)::split<3>;
	split().first.print();
	split().second.print();

	using cat_type = decltype(multiindex_cat( split().first, split().second));
	cat_type::print();

	return hpx::finalize();

}
#endif
