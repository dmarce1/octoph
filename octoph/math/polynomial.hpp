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

template<class T, std::size_t N>
class polynomial {
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
		T x = T(1);
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

	std::array<T, N - 1> roots() {
		const auto D = d_dx();
		std::array<T, N - 1> x;
		std::array<T, N - 1> w;
		for (auto i = 0; i < N - 1; i++) {
			x[i] = T(i);
		}
		//	for( auto y = -1.0; y < 1.0; y += 0.1) {
		//		printf( "%e %e %e\n", y, (*this)(y), d_dx()(y));
		//	}
		for (auto k = 0; k < N - 1; k++) {
			printf("%e ", x[k]);
		}
		printf("\n");
		for (auto i = 0; i < 100000000000; i++) {
			for (auto k = 0; k < N - 1; k++) {
				T factor = 0.0;
				for (auto j = 0; j < N - 1; j++) {
					if (k != j) {
						factor += T(1) / T(x[k] - x[j]);
					}
				}
				const auto P = (*this)(x[k]);
				const auto dP = D(x[k]);
				w[k] = -P * (dP - P * factor);
				if (k > 0) {
					if (w[k] < 0.0) {
						w[k] = std::max(w[k], T(T(0.49999999) * (x[k - 1] - x[k])));
					}
				}
				if (k < N - 2) {
					if (w[k] > 0.0) {
						w[k] = std::min(w[k], T(T(0.49999999) * (x[k + 1] - x[k])));
					}
				}
			}
			for (auto k = 0; k < N - 1; k++) {
				x[k] += w[k];
				printf("%e ", x[k]);
			}
			printf("\n");
		}
		return x;
	}

};

#endif /* OCTOPH_MATH_POLYNOMIAL_HPP_ */
