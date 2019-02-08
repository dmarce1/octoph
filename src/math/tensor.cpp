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
	struct swap_with_top_type;

	template<int L, int M, int ...N>
	struct swap_with_top_type<L,M,N...> {
		using type = typename std::conditional<
		L==0,
		multiindex<I, N..., M, J, K...>,
		typename multiindex<J, K...>::template swap_with_top_type<L - 1, M, N..., I>::type
		>::type;
	};

	template<int L>
	struct swap_with_top_type<L> {
		using type = typename std::conditional<
		L==0,
		multiindex<I, J, K...>,
		typename multiindex<J, K...>::template swap_with_top_type<L - 1, I>::type
		>::type;
	};

};

template<int I>
struct multiindex<I> : public std::integer_sequence<int, I> {
	using base_type = std::integer_sequence<int, I>;

	template<int L>
	static int get() {
		return I;
	}

	template<int, int...>
	struct swap_with_top_type;

	template<int L, int M, int ...N>
	struct swap_with_top_type<L,M,N...> {
		using type = multiindex<I, N..., M>;
	};

	template<int L>
	struct swap_with_top_type<L> {
		using type = multiindex;
	};

};

//
//template<bool, int...>
//struct static_multiindex;
//
//template<int I, int ...J>
//struct static_multiindex<false, I, J...> : public std::integer_sequence<int, I, J...> {
//	static constexpr int next_index = I;
//	using remove_next_index_type = static_multiindex<false,J...>;
//};
//
//template<int I>
//struct static_multiindex<false, I> : public std::integer_sequence<int, I> {
//	static constexpr int next_index = I;
//};
//
//template<int I, int J, int ...K>
//struct static_multiindex<true, I, J, K...> : public std::integer_sequence<int, I, J, K...> {
//	static constexpr int next_index =
//			I<J ? static_multiindex<true, J, K...>::next_index : static_multiindex < true, I, K...>::next_index;
//	using remove_next_index_type = typename std::conditional<
//	I < J,
//	typename static_multiindex<true,J, K...>::remove_next_index_type,
//	typename static_multiindex<true,I, K...>::remove_next_index_type
//	>::type;
//};
//
//template<int I, int J>
//struct static_multiindex<true, I, J> : public std::integer_sequence<int, I, J> {
//	static constexpr int next_index = I < J ? J : I;
//	using remove_next_index_type = typename std::conditional<
//	I < J,
//	static_multiindex<true,J>,
//	static_multiindex<true,I>
//	>::type;
//};
//
//template<int I>
//struct static_multiindex<true, I> : public std::integer_sequence<int, I> {
//	static constexpr int next_index = I;
//};
//
//template<class Type, int Rank, int Dim, bool Symmetric = false>
//struct tensor {
//
//	template<class I>
//	Type get() const {
//		return std::get < I::next_index > (tensor_tuple_).template get<typename I::remove_next_index_type>();
//	}
//
//	template<int I, int ...J>
//	Type get() const {
//		return get<static_multiindex<Symmetric, I, J...>>();
//	}
//
//private:
//	template<int I = Dim - 1>
//	static auto tuple_type_proto() {
//		constexpr int NextDim = Symmetric ? I + 1 : Dim;
//		std::tuple<tensor<Type, Rank - 1, NextDim>> this_tuple;
//		if constexpr (I == 0) {
//			return this_tuple;
//		} else {
//			auto rest_tuple = tuple_type_proto<I - 1>();
//			return std::tuple_cat(this_tuple, rest_tuple);
//		}
//	}
//
//	using tuple_type = decltype(tuple_type_proto());
//
//	tuple_type tensor_tuple_;
//
//};
//
//template<class Type, int Dim>
//struct tensor<Type, 1, Dim> {
//
//	template<class I>
//	Type get() const {
//		return std::get < I::next_index > (tensor_tuple_);
//	}
//
//	template<int I>
//	Type get() const {
//		return get<static_multiindex<I>>();
//	}
//
//private:
//	template<int I = Dim - 1>
//	static auto tuple_type_proto() {
//		std::tuple<Type> this_tuple;
//		if constexpr (I == 0) {
//			return this_tuple;
//		} else {
//			auto rest_tuple = tuple_type_proto<I - 1>();
//			return std::tuple_cat(this_tuple, rest_tuple);
//		}
//	}
//
//	using tuple_type = decltype(tuple_type_proto());
//
//	tuple_type tensor_tuple_;
//
//};

void test() {
//	tensor<double, 2, 3, false> test;
//	test.get<0, 0>();
}
