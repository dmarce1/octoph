/*
 * inverse.hpp
 *
 *  Created on: Jan 21, 2019
 *      Author: dmarce1
 */

#ifndef OCTOPH_MATRIX_INVERSE_HPP_
#define OCTOPH_MATRIX_INVERSE_HPP_

#include "matrix/determinant.hpp"

namespace linear {

template<class A>
struct inverse_type {
	static constexpr bool is_matrix = true;
	using value_type = typename A::value_type;
	static constexpr std::size_t nrow = A::ncol;
	static constexpr std::size_t ncol = A::nrow;

private:
	A a_;
	std::array<value_type, nrow> dets_;
	value_type detinv;

	template<std::size_t I>
	void fill_dets() {
		auto co = comatrix<A, 0, I>(a_);
		const auto det = determinant(co);
		if constexpr (det.zero()) {
			dets_[I] = value_type(0);
		} else {
			dets_[I] = det.get();
			constexpr int si = ((I % 2) == 0) ? 1 : -1;
			if constexpr (si == 1) {
				detinv += dets_[I];
			} else {
				detinv -= dets_[I];
			}
		}
		if constexpr( I > 0 ) {
			fill_dets<I-1>();
		}
	}


	template<class A1, std::size_t I>
	friend class fill_dets;

	inverse_type(const A& a) :
			a_(a), detinv(value_type(0)) {
		fill_dets<nrow - 1>();
		detinv = value_type(1) / detinv;
	}
public:
	template<std::size_t J, std::size_t I>
	auto get() const {
		static_assert(I<nrow);
		static_assert(J<ncol);
		const auto co = comatrix<A, I, J>(a_);
		const auto det = determinant(co);
		constexpr int si = ((I % 2) == 0) ? 1 : -1;
		constexpr int sj = ((J % 2) == 0) ? 1 : -1;
		if constexpr (zero<I, J>()) {
			return value_type(0);
		} else {
			if (I == 0) {
				if constexpr (si * sj == 1) {
					return dets_[I] * detinv;
				} else {
					return -dets_[I] * detinv;
				}
			} else {
				if constexpr (si * sj == 1) {
					return determinant(co).get() * detinv;
				} else {
					return -determinant(co).get() * detinv;
				}
			}
		}
	}

	template<std::size_t J, std::size_t I>
	static constexpr bool zero() {
		static_assert(I<nrow);
		static_assert(J<ncol);
		using det_type = decltype(determinant(comatrix<A, I, J>(a_)));
		return bool(det_type::zero() || A::template zero<I, J>());
	}

	template<class AA>
	friend inverse_type<AA> inverse(const AA& a);
}
;

template<class A>
inverse_type<A> inverse(const A& a) {
	inverse_type<A> p(a);
	return p;
}

}

#endif /* OCTOPH_MATRIX_INVERSE_HPP_ */
