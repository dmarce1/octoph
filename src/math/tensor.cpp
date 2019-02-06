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

namespace math {

namespace tensor {


template<int N>
constexpr int int_pow(int x) {
	if constexpr (N == 0) {
		return 1;
	} else {
		return x * int_pow<N - 1>(x);
	}
}

template<class T, int R, int N, bool S>
struct tensor {

	static constexpr int size() {
		return int_pow<R, N>();
	}

	tensor() = default;
	tensor(const tensor& other) = default;
	tensor(tensor&& other)  = default;
	tensor& operator=(const tensor& other) = default;
	tensor& operator=(tensor&& other)  = default;


private:

	template<class U, int R0, int I>
	struct subtensor_type {
		static constexpr int M = S ? I : N;
		using type = decltype(std::tuple_cat(std::tuple<tensor<U,R0-1,M,S>>(), typename subtensor_type<U,R0-1,I-1>::type()));
	};

	template<class U, int R0>
	struct subtensor_type<U,R0,1> {
		static constexpr int M = S ? 1 : N;
		using type = std::tuple<tensor<U,R0-1,M,S>>;
	};

	template<class U, int I>
	struct subtensor_type<U,0,I> {
		using type = T;
	};

	template<class U>
	struct subtensor_type<U,0,1> {
		using type = std::tuple<T>;
	};

	using tuple_type = typename subtensor_type<T,R,N>::type;

	tuple_type a_;

};


template<class T, int N, bool S>
struct tensor<T,0,N,S> {

	static constexpr int size() {
		return 1;
	}

	tensor() = default;
	tensor(const tensor& other) = default;
	tensor(tensor&& other)  = default;
	tensor& operator=(const tensor& other) = default;
	tensor& operator=(tensor&& other)  = default;


private:
	T a_;

};

void test() {
	tensor<double, 2, 3,true> test;
}

}
}

