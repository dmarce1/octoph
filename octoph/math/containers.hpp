/*
 * containers.hpp
 *
 *  Created on: Jan 31, 2019
 *      Author: dmarce1
 */

#ifndef OCTOPH_MATH_CONTAINERS_HPP_
#define OCTOPH_MATH_CONTAINERS_HPP_

#include <utility>
#include <tuple>
#include <type_traits>
#include <array>
#include <functional>
#include <thread>
#include <hpx/async.hpp>

#include <octoph/exception/exception.hpp>

#define OCTOPH_MATH_CONTAINERS_BINARY_OP( OP )                                                           \
template<class A, class B>                                                                               \
inline decltype(typename A::value_type()*typename B::value_type()) \
operator OP (const A& a, const B& b) {                                                       \
	struct op_type {                                                                                     \
		constexpr op_type() = default;                                                                   \
		inline auto operator()(const typename A::value_type& a, const typename B::value_type& b) const { \
			return a OP b;                                                                               \
		}                                                                                                \
	};                                                                                                   \
	constexpr op_type op;                                                                                \
	return operation(op, std::integral_constant<int, 1>(), a, b);                                        \
} \
template<class A, class B>                                                                               \
inline decltype(typename A::value_type()*B()) \
operator OP (const A& a, const B& b) {                                                       \
	struct op_type {                                                                                     \
		constexpr op_type() = default;                                                                   \
		inline auto operator()(const typename A::value_type& a, const B& b) const { \
			return a OP b;                                                                               \
		}                                                                                                \
	};                                                                                                   \
	constexpr op_type op;                                                                                \
	return operation(op, std::integral_constant<int, 1>(), a, b);                                        \
}

#define OCTOPH_MATH_CONTAINERS_UNARY_OP( OP )                           \
template<class A>                                                       \
inline auto operator OP (const A& a) {                                  \
	struct op_type {                                                    \
		constexpr op_type() = default;                                  \
		inline auto operator()(const typename A::value_type& a) const { \
			return OP a;                                                \
		}                                                               \
	};                                                                  \
	constexpr op_type op;                                               \
	return operation(op, std::integral_constant<int, 1>(), a);          \
}

namespace math {
namespace containers {

template<class A>
struct is_operation {
	static constexpr auto value = false;
};

template<class F, auto OP_COUNT, class ...A>
class operation;

template<class F, auto OP_COUNT, class ...A>
struct is_operation<operation<F, OP_COUNT, A...>> {
	static constexpr auto value = true;
};

template<class F, auto OP_COUNT, class ...A>
class operation {
	static constexpr auto N = sizeof...(A);
	using tuple_type =
	std::tuple<
	typename std::conditional<is_operation<A>::value,A,const A&>::type...>;

	tuple_type a_;
	F f_;

	template<class G>
	auto ops_count(const G&) const {
		return 0;
	}

	template<class F1, auto OP_COUNT1, class ...A1>
	auto ops_count(const operation<F1, OP_COUNT1, A1...>& g) const {
		return g.op_count();
	}

	template<auto M>
	auto other_ops() const {
		const auto these_ops = ops_count(std::get < M > (a_));
		if constexpr (M == 0) {
			return these_ops;
		} else {
			return these_ops + other_ops<M - 1>();
		}
	}

public:

	using value_type = typename std::result_of<F(typename A::value_type...)>::type;

	template<std::size_t ... I>
	inline value_type operate(auto i, std::integer_sequence<std::size_t, I...>) const {
		return f_(std::get<I>(a_)[i]...);
	}

	auto size() const {
		return std::get < 0 > (a_).size();
	}

	explicit operation(const F& f, std::integral_constant<auto, OP_COUNT> I, const A& ... a) :
			f_(f), a_(a...) {
	}

	template<class T>
	inline value_type operator[](const T& i) const {
		return operate(i, std::make_index_sequence<N>());
	}

	inline int op_count() const {
		return OP_COUNT * std::get < 0 > (a_).size() + other_ops<N - 1>();
	}

	template<class C>
	inline void store(C& copy) const {
		if (copy.size() < size()) {
			THROW("store container too small");
		}
		static const auto max_threads = std::thread::hardware_concurrency();
		constexpr auto min_ops_per_thread = 1024 * 256;
		auto nops = op_count();
		const auto nthreads = std::min(int(nops / min_ops_per_thread), int(max_threads));
		std::vector<hpx::future<void>> futs(nthreads - 1);
		for (auto i = 1; i < nthreads; i++) {
			const auto begin = i * size() / nthreads;
			const auto end = (i + 1) * size() / nthreads;
			futs[i - 1] = hpx::async([begin, end, &copy, this]() {
#pragma GCC ivdep
					for( int i = begin; i < end; i++ ) {
						copy[i] = operator[](i);
					}
				});
		}
		const auto begin = 0;
		const auto end = size() / nthreads;
#pragma GCC ivdep
		for (int i = 0; i < end; i++) {
			copy[i] = operator[](i);
		}
		hpx::wait_all(std::begin(futs), std::end(futs));
	}

}
;

OCTOPH_MATH_CONTAINERS_UNARY_OP(+);
OCTOPH_MATH_CONTAINERS_UNARY_OP(-);
OCTOPH_MATH_CONTAINERS_UNARY_OP(~);
OCTOPH_MATH_CONTAINERS_UNARY_OP(!);
OCTOPH_MATH_CONTAINERS_BINARY_OP(+);
OCTOPH_MATH_CONTAINERS_BINARY_OP(-);
OCTOPH_MATH_CONTAINERS_BINARY_OP(*);
OCTOPH_MATH_CONTAINERS_BINARY_OP(/);
OCTOPH_MATH_CONTAINERS_BINARY_OP(%);
OCTOPH_MATH_CONTAINERS_BINARY_OP(>);
OCTOPH_MATH_CONTAINERS_BINARY_OP(<);
OCTOPH_MATH_CONTAINERS_BINARY_OP(>=);
OCTOPH_MATH_CONTAINERS_BINARY_OP(<=);
OCTOPH_MATH_CONTAINERS_BINARY_OP(==);
OCTOPH_MATH_CONTAINERS_BINARY_OP(!=);
OCTOPH_MATH_CONTAINERS_BINARY_OP(^);
OCTOPH_MATH_CONTAINERS_BINARY_OP(&);
OCTOPH_MATH_CONTAINERS_BINARY_OP(|);
OCTOPH_MATH_CONTAINERS_BINARY_OP(&&);
OCTOPH_MATH_CONTAINERS_BINARY_OP(||);
OCTOPH_MATH_CONTAINERS_BINARY_OP(>>);
OCTOPH_MATH_CONTAINERS_BINARY_OP(<<);

}
}
#endif /* OCTOPH_MATH_CONTAINERS_HPP_ */
