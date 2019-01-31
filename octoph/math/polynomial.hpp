/*
 * polynomial.hpp
 *
 *  Created on: Jan 29, 2019
 *      Author: dmarce1
 */

#ifndef OCTOPH_MATH_POLYNOMIAL_HPP_
#define OCTOPH_MATH_POLYNOMIAL_HPP_

#include <array>
#include <algorithm>
#include <cmath>
#include <limits>
#include <complex>
#include <octoph/exception/exception.hpp>
#include <octoph/math/math.hpp>

namespace math {

template<class T, std::size_t N>
class polynomial {
	constexpr static auto zero = T(0);
	constexpr static auto one = T(1);
	constexpr static auto two = T(2);
	std::array<T, N> c_;
	using roots_type = std::array<std::complex<T>, N - 1>;
public:

	polynomial(const std::array<T, N>& c) :
			c_(c) {
	}

	T operator[](int i) const {
		return c_[i];
	}

	const T& operator[](int i) {
		return c_[i];
	}

	T operator()(const T& x0) const {
		T x = one;
		T f = c_[0];
		for (auto i = 1; i < N; i++) {
			x *= x0;
			f += c_[i] * x;
		}
		return f;
	}

	std::complex<T> operator()(const std::complex<T>& x0) const {
		std::complex<T> x = one;
		std::complex<T> f = c_[0];
		for (auto i = 1; i < N; i++) {
			x *= x0;
			f += c_[i] * x;
		}
		return f;
	}

	polynomial<T, N - 1> d_dx() const {
		std::array<T, N - 1> d;
		for (auto i = 1; i < N; i++) {
			d[i - 1] = T(i) * c_[i];
		}
		return polynomial<T, N - 1>(d);
	}

	polynomial<T, N + 1> integral() const {
		std::array<T, N + 1> I;
		I[0] = 0;
		for (auto i = 1; i < N + 1; i++) {
			I[i] = c_[i - 1] / T(i);
		}
		return polynomial(I);
	}

	T root_upper_bound() const {
		T amax = zero;
		for (auto i = 0; i < N - 1; i++) {
			amax = std::max(amax, std::abs(c_[i]));
		}
		return one + amax / std::abs(c_[N - 1]);
	}

	T root_lower_bound() const {
		T amax = zero;
		const auto c0 = std::abs(c_[0]);
		for (auto i = 1; i < N; i++) {
			amax = std::max(amax, std::abs(c_[i]));
		}
		return c0 / (c0 + amax / std::abs(c_[N - 1]));
	}

	auto all_roots() {
		constexpr auto iters_max = 40;
		const auto D = d_dx();
		const auto r_max = root_upper_bound();
		const auto r_min = root_lower_bound();
		const auto dtheta = constants::golden_angle<T>;
		T theta = dtheta / two;
		roots_type w;
		roots_type x;
		for (auto i = 0; i < N - 1; i++) {
			const T b = (T(2) * (T(i) / T(N - 1)) - T(1));
			const auto r = b * (r_max - r_min) + r_min;
			x[i].real(r * std::cos(theta));
			x[i].imag(r * std::sin(theta));
			theta += dtheta;
		}
		typename std::remove_const<T>::type max_change;
		const auto toler = std::pow(0.1, std::numeric_limits < T > ::digits10 - 1);
		T err;
		auto iters = 0;
		do {
			if( iters > iters_max ) {
				THROW("root solver failed to converge after 40 iterations.");
			}
			iters++;
			err = zero;
			for (auto k = 0; k < N - 1; k++) {
				RETRY: std::complex<T> factor = 0.0;
				for (auto j = 0; j < N - 1; j++) {
					if (k != j) {
						if (std::abs(x[k] - x[j]) < small<T>) {
							x[k] += small<T>;
							goto RETRY;
						} else {
							factor += one / (x[k] - x[j]);
						}
					}
				}
				const auto P = (*this)(x[k]);
				const auto dP = D(x[k]);
				w[k] = P / (P * factor - dP);
				err += std::abs(P * P);
			}
			for (auto k = 0; k < N - 1; k++) {
				x[k] += w[k];
			}
			if (std::isnan(err)) {
				THROW("root solver failed to converge ending in NAN.");
			}
			err = std::sqrt(err);
		} while (err > toler);
		return x;
	}

};

}

#endif /* OCTOPH_MATH_POLYNOMIAL_HPP_ */
