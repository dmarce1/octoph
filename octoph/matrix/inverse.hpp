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
	static constexpr std::size_t nrow = A::ncol;
	static constexpr std::size_t ncol = A::nrow;
	using value_type = typename A::value_type;

private:
	A a_;
	std::array<value_type, nrow> dets_;
	value_type detinv;

	template<class A1, std::size_t I>
	struct fill_dets {
		void operator()(inverse_type<A1>& a) const {
			auto co = comatrix<A, 0, I>(a.a_);
			using next = fill_dets<A1,I-1>;
			next f;
			f(a);
			const auto det = determinant(co);
			if constexpr(det.zero()) {
				a.dets_[I] = value_type(0);
			} else {
				a.dets_[I] = det.get();
				constexpr int si = ((I % 2) == 0) ? 1 : -1;
				if constexpr(si == 1) {
					a.detinv += a.dets_[I];
				} else {
					a.detinv -= a.dets_[I];
				}
			}
		}
	};

	template<class A1>
	struct fill_dets<A1, 0> {
		void operator()(inverse_type<A1>& a) const {
			auto co = comatrix<A, 0, 0>(a.a_);
			const auto det = determinant(co);
			if constexpr(det.zero()) {
				a.dets_[0] = value_type(0);
			} else {
				a.dets_[0] = det.get();
				a.detinv += a.dets_[0];
			}
		}
	};

	template<class A1, std::size_t I>
	friend class fill_dets;

	inverse_type(const A& a) :
			a_(a), detinv(0) {
		fill_dets<A, nrow - 1> f;
		f(*this);
		detinv = value_type(1) / detinv;
	}
public:
	template<std::size_t J, std::size_t I>
	value_type get() const {
		static_assert(I<nrow);
		static_assert(J<ncol);
		const auto co = comatrix<A, I, J>(a_);
		const auto det = determinant(co);
		constexpr int si = ((I % 2) == 0) ? 1 : -1;
		constexpr int sj = ((J % 2) == 0) ? 1 : -1;
		if constexpr(det.zero()) {
			return value_type(0);
		} else {
			if (I == 0) {
				if constexpr(si * sj == 1) {
					return dets_[I] * detinv;
				} else {
					return dets_[I] * value_type(-1) * detinv;
				}
			} else {
				if constexpr(si * sj == 1) {
					return determinant(co).get() * detinv;
				} else {
					return determinant(co).get() * value_type(-1) * detinv;
				}
			}
		}
	}

	template<std::size_t J, std::size_t I>
	constexpr bool zero() const {
		static_assert(I<nrow);
		static_assert(J<ncol);
		return false;
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
