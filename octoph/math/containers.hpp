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

namespace math {

template<class F, class ...A>
inline auto operation(const F&, const A&...);

namespace detail {
template<class F, class ...A>
class operation_type {
	static constexpr auto N = sizeof...(A);
	F f_;
	std::tuple<const A&...> a_;
	operation_type(const F& f, const A& ... a) :
			f_(f), a_(a...) {
	}
	template<class T, T ... I>
	inline auto operate(int i, std::integer_sequence<T, I...> = std::make_index_sequence<N>()) const {
		return f_(std::get<I>(a_)[i]...);
	}

public:
	template<class T>
	auto operator[](const T& i) const {
		return operate(i);
	}
	template<class G, class ...B>
	friend auto math::operation(const G&, const B&...);
};

}

template<class F, class ...A>
inline auto operation(const F& f, const A&...a) {
	return detail::operation_type(f, a...);

}



}

#endif /* OCTOPH_MATH_CONTAINERS_HPP_ */
