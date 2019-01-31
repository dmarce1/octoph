/*
 * math.hpp
 *
 *  Created on: Jan 19, 2019
 *      Author: dmarce1
 */

#ifndef OCTOPH_MATRIX_MATH_HPP_
#define OCTOPH_MATRIX_MATH_HPP_

#include <algorithm>
#include <atomic>
#include <cstdint>
#include <cmath>
#include <limits>

namespace math {
namespace constants {

template<class T>
inline constexpr T pi = 3.1415926535897932384626433832795028841971;

template<class T>
inline constexpr T golden_ratio = 1.6180339887498948482045868343656381177203;

template<class T>
inline constexpr T golden_angle = T(2) * pi < T > *(T(1) - T(1) / golden_ratio<T> );

}

template<class T>
inline constexpr T large = std::sqrt(std::numeric_limits < T > ::max());

template<class T>
inline constexpr T small = std::sqrt(std::numeric_limits < T > ::min());

template<class T>
auto minmod(const T& a, const T& b) {
	return (std::copysign(0.5, a) + std::copysign(0.5, b)) * (std::min(std::abs(a), std::abs(b)));
}

}

using real = double;

#endif /* OCTOPH_MATRIX_MATH_HPP_ */
