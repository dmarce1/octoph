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
#include <limits>
#include <complex>

template<class T, std::size_t N>
class polynomial {
	using abs_type = decltype(std::abs(T()));
	const static abs_type abs_zero;
	const static abs_type abs_one;
	const static abs_type abs_two;
	constexpr static auto zero = T(0);
	constexpr static auto one = T(1);
	constexpr static auto two = T(2);
	std::array<T, N> c_;
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

	abs_type root_upper_bound() const {
		abs_type amax = abs_zero;
		for (auto i = 0; i < N - 1; i++) {
			amax = std::max(amax, std::abs(c_[i]));
		}
		return abs_one + amax / std::abs(c_[N - 1]);
	}

	abs_type root_lower_bound() const {
		abs_type amax = abs_zero;
		const auto abs_c0 = std::abs(c_[0]);
		for (auto i = 1; i < N; i++) {
			amax = std::max(amax, std::abs(c_[i]));
		}
		return abs_c0 / (abs_c0 + amax / std::abs(c_[N - 1]));
	}
	using roots_type = std::array<T, N - 1>;
private:

	roots_type roots_init() {
		const auto r_max = root_upper_bound();
		roots_type x;
		if constexpr (std::is_floating_point < T > ::value) {
			for (auto i = 0; i < N - 1; i++) {
				x[i] = (T(2) * (T(i) / T(N - 1)) - one) * r_max;
			}
		} else {
			const auto r_min = root_lower_bound();
			const auto dtheta = constants::golden_angle<abs_type>;
			abs_type theta = dtheta / abs_two;
			for (auto i = 0; i < N - 1; i++) {
				const abs_type b = (abs_type(2) * (abs_type(i) / abs_type(N - 1)) - abs_type(1));
				const auto r = b * (r_max - r_min) + r_min;
				x[i].real(r * std::cos(theta));
				x[i].imag(r * std::sin(theta));
				theta += dtheta;
			}
		}
		return x;
	}
public:

	roots_type roots() {
		const auto D = d_dx();
		roots_type w;
		auto x = roots_init();
		typename std::remove_const<decltype(abs_zero)>::type max_change;
		const auto toler = 1.0e-10;
		abs_type err;
		do {
			err = abs_zero;
			for (auto k = 0; k < N - 1; k++) {
				T factor = 0.0;
				for (auto j = 0; j < N - 1; j++) {
					if (k != j) {
						factor += one / (x[k] - x[j]);
					}
				}
				const auto P = (*this)(x[k]);
				const auto dP = D(x[k]);
				w[k] = P / (P * factor - dP);
				err += std::abs(P * P);
			}
			for (auto k = 0; k < N - 1; k++) {
				x[k] += w[k];
				printf("(%e,%e) %e %e ", x[k].real(), x[k].imag(), (*this)(x[k]).real(), (*this)(x[k]).imag());
			}
			err = std::sqrt(err);
			printf("%e %e\n", err, toler);
		} while (err > toler);
		return x;
	}

};

template<class T, std::size_t N>
const decltype(std::abs(T())) polynomial<T, N>::abs_zero = std::abs(T(0));

template<class T, std::size_t N>
const decltype(std::abs(T())) polynomial<T, N>::abs_one = std::abs(T(1));

template<class T, std::size_t N>
const decltype(std::abs(T())) polynomial<T, N>::abs_two = std::abs(T(2));

#endif /* OCTOPH_MATH_POLYNOMIAL_HPP_ */
