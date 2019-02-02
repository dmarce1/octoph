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

#define OCTOPH_MATH_CONTAINERS_BINARY_OP( NAME, OP )                                                         \
		template<class A, class B>                                                                           \
		struct NAME##_type {                                                                                 \
			constexpr NAME##_type() = default;                                                               \
			inline auto operator()(const typename A::value_type& a, const typename B::value_type& b) const { \
				return a OP b;                                                                               \
			}                                                                                                \
			using A_type = A;                                                                                \
			using B_type = B;                                                                                \
		};                                                                                                   \
		template<class A>                                                                                    \
		struct NAME##_type<A,typename A::value_type> {                                                       \
			using B = typename A::value_type;                                                                \
			constexpr NAME##_type() = default;                                                               \
			inline auto operator()(const typename A::value_type& a, const B& b) const {                      \
				return a OP b;                                                                               \
			}                                                                                                \
			using A_type = A;                                                                                \
			using B_type = detail::singleton<B>;                                                             \
		};                                                                                                   \
		template<class B>                                                                                    \
		struct NAME##_type<typename B::value_type,B> {                                                       \
			using A = typename B::value_type;                                                                \
			constexpr NAME##_type() = default;                                                               \
			inline auto operator()(const A& a,const typename B::value_type& b) const {                       \
				return a OP b;                                                                               \
			}                                                                                                \
			using A_type = detail::singleton<A>;                                                             \
			using B_type = B;                                                                                \
		};                                                                                                   \
		template<class A, class B>                                                                           \
		inline auto operator OP (const A& a, const  B& b ) {                                                 \
			using op_type = NAME##_type<A,B>;                                                                \
			constexpr op_type op;                                                                            \
			return operation(op,                                           \
					typename op_type::A_type(a), typename op_type::B_type(b), std::integral_constant<int, 1>(),std::integral_constant<bool,false>());                               \
		}

#define OCTOPH_MATH_CONTAINERS_UNARY_OP( NAME, OP )                                                           \
		template<class A>                                                                                     \
		struct NAME##_type {                                                                                  \
			constexpr NAME##_type() = default;                                                                \
			inline auto operator()(const typename A::value_type& a, const typename A::value_type&) const {                                   \
				return OP a;                                                                                  \
			}                                                                                                 \
		};                                                                                                    \
		template<class A>                                                                                     \
		inline auto operator OP (const A& a) {                                                                \
			constexpr NAME##_type<A> op;                                                                      \
			return operation(op, a, a, std::integral_constant<int, 1>(),std::integral_constant<bool,true>());                                        \
		}

namespace math {
namespace containers {

template<class, class, class, int, bool>
class operation;

namespace detail {

template<class T>
class singleton {
	T value_;
public:

	inline constexpr int op_count() const {
		return 0;
	}
	using value_type = T;
	inline singleton(T&& value) :
			value_(std::move(value)) {
	}
	inline singleton(const T& value) :
			value_(value) {
	}
	inline const T& operator[](int) const {
		return value_;
	}
};

template<class A>
struct is_operation {
	static constexpr auto value = false;
};

template<class F, class A, class B, int OP_COUNT, bool UNARY>
struct is_operation<operation<F, A, B, OP_COUNT, UNARY>> {
	static constexpr auto value = true;
};

template<class T>
struct is_operation<singleton<T>> {
	static constexpr auto value = true;
};

template<class C>
struct ctr {
	using value_type = C;
};

template<class T>
struct ctr<singleton<T>> {
	using value_type = char;
};

template<class F, class A, class B, int OP_COUNT, bool UNARY>
struct ctr<math::containers::operation<F, A, B, OP_COUNT, UNARY>> {
	using A_type = typename ctr<A>::value_type;
	using B_type = typename ctr<B>::value_type;
	using value_type = typename std::conditional<!std::is_same<A_type,char>::value,A_type, B_type>::type;
};

template<class T, class U, int I>
struct is_storage_type {
	using type = typename std::tuple_element<I,T>::type;
	static constexpr bool value =
			std::is_same<U, typename std::remove_cv<typename std::remove_reference<type>::type>::type>::value
					|| is_storage_type<T, U, I - 1>::value;
};

template<class T, class U>
struct is_storage_type<T, U, 0> {
	using type = typename std::tuple_element<0,T>::type;
	static constexpr bool value =
			std::is_same<U, typename std::remove_cv<typename std::remove_reference<type>::type>::type>::value;
};

}

template<class F, class A, class B, int OP_COUNT, bool UNARY>
class operation {
	using A_type = typename std::conditional<detail::is_operation<A>::value,A,const A&>::type;
	using B_type = typename std::conditional<detail::is_operation<B>::value,B,const B&>::type;

	A_type a_;
	B_type b_;
	F f_;

	template<class T>
	class has_resize {
		using one = char;
		using two = long;

		template<class C>
		static one test(decltype(&C::resize));
		template<class C>
		static two test(...);

	public:
		static constexpr bool value = sizeof(test<T>(0)) == sizeof(char);
	};

public:

	using value_type = typename std::result_of<F(typename A::value_type,typename B::value_type)>::type;
	using container_type = typename detail::ctr<operation<F,A,B,OP_COUNT,UNARY>>::value_type;

	auto size() const {
		return a_.size();
	}

	explicit operation(const F& f, const A& a, const B& b, std::integral_constant<int, OP_COUNT> I,
			std::integral_constant<bool, UNARY> U) :
			f_(f), a_(a), b_(b) {
	}

	inline int op_count() const {
		if constexpr (detail::is_operation<B>::value && !UNARY) {
			if constexpr (detail::is_operation<A>::value) {
				return OP_COUNT * a_.size() + a_.op_count() + b_.op_count();
			} else {
				return OP_COUNT * a_.size() + b_.op_count();
			}

		} else {
			if constexpr (detail::is_operation<A>::value) {
				return OP_COUNT * a_.size() + a_.op_count();
			} else {
				return OP_COUNT * a_.size();
			}
		}
	}

	template<class T>
	inline value_type operator[](const T& i) const {
		F f;
		return f(a_[i], b_[i]);
	}

	inline operator container_type() const {
		container_type copy;
		if constexpr (has_resize<container_type>::value) {
			copy.resize(size());
		}
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
				for( int i = begin; i < end; i++ ) {
					copy[i] = operator[](i);
				}
			});
		}
		const auto begin = 0;
		const auto end = size() / nthreads;
		for (int i = 0; i < end; i++) {
			copy[i] = operator[](i);
		}
		hpx::wait_all(std::begin(futs), std::end(futs));
		return std::move(copy);
	}

}
;

OCTOPH_MATH_CONTAINERS_BINARY_OP(mul, *);
OCTOPH_MATH_CONTAINERS_BINARY_OP(add, +);
OCTOPH_MATH_CONTAINERS_BINARY_OP(sub, -);
OCTOPH_MATH_CONTAINERS_BINARY_OP(div, /);
OCTOPH_MATH_CONTAINERS_BINARY_OP(mod, %);
OCTOPH_MATH_CONTAINERS_BINARY_OP(gt, >);
OCTOPH_MATH_CONTAINERS_BINARY_OP(lt, <);
OCTOPH_MATH_CONTAINERS_BINARY_OP(gte, >=);
OCTOPH_MATH_CONTAINERS_BINARY_OP(lte, <=);
OCTOPH_MATH_CONTAINERS_BINARY_OP(eq, ==);
OCTOPH_MATH_CONTAINERS_BINARY_OP(neq, !=);
OCTOPH_MATH_CONTAINERS_BINARY_OP(bxor, ^);
OCTOPH_MATH_CONTAINERS_BINARY_OP(band, &);
OCTOPH_MATH_CONTAINERS_BINARY_OP(bor, |);
OCTOPH_MATH_CONTAINERS_BINARY_OP(land, &&);
OCTOPH_MATH_CONTAINERS_BINARY_OP(lor, ||);
OCTOPH_MATH_CONTAINERS_BINARY_OP(shr, >>);
OCTOPH_MATH_CONTAINERS_BINARY_OP(shl, <<);
OCTOPH_MATH_CONTAINERS_UNARY_OP(pos, +);
OCTOPH_MATH_CONTAINERS_UNARY_OP(neg, -);
OCTOPH_MATH_CONTAINERS_UNARY_OP(bnot, ~);
OCTOPH_MATH_CONTAINERS_UNARY_OP(lnot, !);

}
}
#endif /* OCTOPH_MATH_CONTAINERS_HPP_ */
