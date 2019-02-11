/*
 * tensor.hpp
 *
 *  Created on: Feb 9, 2019
 *      Author: dmarce1
 */

#ifndef OCTOPH_MATH_TENSOR_HPP_
#define OCTOPH_MATH_TENSOR_HPP_

#include <octoph/math/multiindex.hpp>
#include <type_traits>
#include <tuple>

namespace math {

enum tensor_shape_type {
	REGULAR, SYMMETRIC, ANTISYMMETRIC
};

template<int Rank, int Dim, tensor_shape_type ...Shapes>
struct tensor_access {

	template<int I, int ...J>
	static constexpr bool index_allowed(const multiindex<I, J...>&) {
		if constexpr (sizeof...(J) > 0) {
			constexpr int K = std::get < 0 > (std::make_tuple(J...));
			constexpr int shape_index = Rank - int(sizeof...(J)) - 1;
			constexpr auto shape = std::get < shape_index > (std::make_tuple(Shapes...));
			if constexpr (shape == SYMMETRIC) {
				if constexpr (I >= K) {
					return index_allowed(multiindex<J...>());
				} else {
					return false;
				}
			} else if constexpr (shape == ANTISYMMETRIC) {
				if constexpr (I > K) {
					return index_allowed(multiindex<J...>());
				} else {
					return false;
				}
			} else {
				return index_allowed(multiindex<J...>());
			}
		} else {
			return true;
		}
	}

	template<class I>
	static constexpr int multiindex_to_flat_index(const I& i) {
		using next_index = decltype(multiindex_increment<Dim>(i));
		if constexpr (std::is_same<next_index, void>::value) {
			return -1;
		} else if constexpr (index_allowed(i)) {
			return 1 + multiindex_to_flat_index(next_index());
		} else {
			return multiindex_to_flat_index(next_index());
		}
	}

};

}

#endif /* OCTOPH_MATH_TENSOR_HPP_ */
