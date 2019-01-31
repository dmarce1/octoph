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

#define NUMBER_BINARY_OP( type, op )                        \
	inline type operator op (const type& other) const {     \
		gops_++;                                            \
		tops_++;                                            \
		return type(n_ op other.n_);                        \
    }                                                       \
	inline type& operator op##= (const type& other) {       \
		gops_++;                                            \
		tops_++;                                            \
		n_ op##= other.n_;                                  \
		return *this;                                       \
    }

#define NUMBER_LOGICAL_OP( type, op )                       \
	inline bool operator op (const type& other) const {     \
		gops_++;                                            \
		tops_++;                                            \
		return bool(n_ op other.n_);                        \
    }                                                       \


#define DECLARE_UNARY_TRANSCENDENTAL(type, func )           \
		friend type func(const type&);

#define DECLARE_BINARY_TRANSCENDENTAL(type, func )          \
		friend type func(const type&,const type&);

#define DEFINE_UNARY_TRANSCENDENTAL( type,func, op_count )  \
		inline type func (const type& n) {                  \
			type::gops_+=op_count;                          \
			type::tops_+=op_count;                          \
			return std:: func (n.n_);                       \
		}

#define DEFINE_BINARY_TRANSCENDENTAL( type,func, op_count ) \
		inline type func (const type& n, const type& m) {   \
			type::gops_+=op_count;                          \
			type::tops_+=op_count;                          \
			return std:: func (n.n_, m.n_);                 \
		}

#define DEFINE_COUNTER(name, counter, type)             \
	friend class name;                                  \
	class name {                                        \
	private:                                            \
		std::uint64_t start_;                           \
		std::uint64_t count_;                           \
		bool running_;                                  \
	public:                                             \
	    name () : start_(0), count_(0),                 \
                  running_(false)  {                    \
	     }                                              \
		void stop() {                                   \
			running_ = false;                           \
			count_ = type::counter - start_;            \
		}                                               \
		std::uint64_t read() const {                    \
			if (running_) {                             \
				return type::counter - start_ + count_; \
			} else {                                    \
				return count_;                          \
			}                                           \
		}                                               \
		void start() {                                  \
			running_ = true;                            \
			start_ = type::counter;                     \
		}                                               \
		void reset() {                                  \
			count_ = 0;                                 \
		}                                               \
	};

namespace math {

class real {
private:
	static std::atomic<std::uint64_t> gops_;
	static thread_local std::atomic<std::uint64_t> tops_;
	double n_;
public:
	double get() const {
		return n_;
	}
	inline real() :
			n_(0.0) {
	}
	inline ~real() = default;
	inline real(real&&) = default;
	inline real& operator=(real&&) = default;
	inline real(const real& n) :
			n_(n.n_) {
	}
	inline real& operator=(const real& n) {
		n_ = n.n_;
		return *this;
	}
	inline real(const double& n) :
			n_(n) {
	}
	inline real& operator=(const double& n) {
		n_ = n;
		return *this;
	}
	inline real operator+() {
		return *this;
	}
	inline real operator-() const {
		gops_++;
		tops_++;
		return real(-n_);
	}
	;DEFINE_COUNTER(global_counter, gops_, real )
	;DEFINE_COUNTER(local_counter, tops_, real )
	;NUMBER_BINARY_OP(real, + )
	;NUMBER_BINARY_OP(real, - )
	;NUMBER_BINARY_OP(real, * )
	;NUMBER_BINARY_OP(real, / )
	;NUMBER_LOGICAL_OP(real, < )
	;NUMBER_LOGICAL_OP(real, > )
	;NUMBER_LOGICAL_OP(real, <= )
	;NUMBER_LOGICAL_OP(real, >= )
	;NUMBER_LOGICAL_OP(real, == )
	;NUMBER_LOGICAL_OP(real, != )
	;DECLARE_UNARY_TRANSCENDENTAL(real, abs)
	;DECLARE_UNARY_TRANSCENDENTAL(real, sqrt)
	;DECLARE_UNARY_TRANSCENDENTAL(real, exp)
	;DECLARE_UNARY_TRANSCENDENTAL(real, log)
	;DECLARE_UNARY_TRANSCENDENTAL(real, sin)
	;DECLARE_UNARY_TRANSCENDENTAL(real, cos)
	;DECLARE_UNARY_TRANSCENDENTAL(real, tan)
	;DECLARE_UNARY_TRANSCENDENTAL(real, sinh)
	;DECLARE_UNARY_TRANSCENDENTAL(real, cosh)
	;DECLARE_UNARY_TRANSCENDENTAL(real, tanh)
	;DECLARE_UNARY_TRANSCENDENTAL(real, asin)
	;DECLARE_UNARY_TRANSCENDENTAL(real, acos)
	;DECLARE_UNARY_TRANSCENDENTAL(real, atan)
	;DECLARE_UNARY_TRANSCENDENTAL(real, asinh)
	;DECLARE_UNARY_TRANSCENDENTAL(real, acosh)
	;DECLARE_UNARY_TRANSCENDENTAL(real, atanh)
	;DECLARE_UNARY_TRANSCENDENTAL(real, log10)
	;DECLARE_BINARY_TRANSCENDENTAL(real, pow)
	;DECLARE_BINARY_TRANSCENDENTAL(real, max)
	;DECLARE_BINARY_TRANSCENDENTAL(real, min)
	;DECLARE_BINARY_TRANSCENDENTAL(real, atan2)
	;DECLARE_BINARY_TRANSCENDENTAL(real, copysign)

}
;

class integer {
private:
	static std::atomic<std::uint64_t> gops_;
	static thread_local std::atomic<std::uint64_t> tops_;
	int n_;
public:
	inline integer() :
			n_(0) {
	}
	inline ~integer() = default;
	inline integer(integer&&) = default;
	inline integer& operator=(integer&&) = default;
	inline integer(const integer& n) :
			n_(n.n_) {
	}
	inline integer& operator=(const integer& n) {
		n_ = n.n_;
		return *this;
	}
	inline integer(const double& n) :
			n_(n) {
	}
	inline integer& operator=(const double& n) {
		n_ = n;
		return *this;
	}
	inline integer operator+() {
		return *this;
	}
	inline integer operator-() const {
		gops_++;
		tops_++;
		return integer(-n_);
	}
	;DEFINE_COUNTER(global_counter, gops_, integer )
	;DEFINE_COUNTER(local_counter, tops_, integer )
	;NUMBER_LOGICAL_OP(integer, < )
	;NUMBER_LOGICAL_OP(integer, > )
	;NUMBER_LOGICAL_OP(integer, <= )
	;NUMBER_LOGICAL_OP(integer, >= )
	;NUMBER_LOGICAL_OP(integer, == )
	;NUMBER_LOGICAL_OP(integer, != )
	;NUMBER_BINARY_OP(integer, + )
	;NUMBER_BINARY_OP(integer, - )
	;NUMBER_BINARY_OP(integer, * )
	;NUMBER_BINARY_OP(integer, / )
	;DECLARE_UNARY_TRANSCENDENTAL(integer, abs)
	;DECLARE_BINARY_TRANSCENDENTAL(integer, pow)
	;DECLARE_BINARY_TRANSCENDENTAL(integer, max)
	;DECLARE_BINARY_TRANSCENDENTAL(integer, min)
	;DECLARE_BINARY_TRANSCENDENTAL(integer, copysign)

};

#define DEFAULT_TRANSCENNTAL_OP_COUNT 100

DEFINE_UNARY_TRANSCENDENTAL(integer, abs, 1);
DEFINE_UNARY_TRANSCENDENTAL(real, abs, 1);
DEFINE_UNARY_TRANSCENDENTAL(real, sqrt, DEFAULT_TRANSCENNTAL_OP_COUNT);
DEFINE_UNARY_TRANSCENDENTAL(real, exp, DEFAULT_TRANSCENNTAL_OP_COUNT);
DEFINE_UNARY_TRANSCENDENTAL(real, log, DEFAULT_TRANSCENNTAL_OP_COUNT);
DEFINE_UNARY_TRANSCENDENTAL(real, log10, DEFAULT_TRANSCENNTAL_OP_COUNT);
DEFINE_UNARY_TRANSCENDENTAL(real, sin, DEFAULT_TRANSCENNTAL_OP_COUNT);
DEFINE_UNARY_TRANSCENDENTAL(real, cos, DEFAULT_TRANSCENNTAL_OP_COUNT);
DEFINE_UNARY_TRANSCENDENTAL(real, tan, DEFAULT_TRANSCENNTAL_OP_COUNT);
DEFINE_UNARY_TRANSCENDENTAL(real, sinh, DEFAULT_TRANSCENNTAL_OP_COUNT);
DEFINE_UNARY_TRANSCENDENTAL(real, cosh, DEFAULT_TRANSCENNTAL_OP_COUNT);
DEFINE_UNARY_TRANSCENDENTAL(real, tanh, DEFAULT_TRANSCENNTAL_OP_COUNT);
DEFINE_UNARY_TRANSCENDENTAL(real, asinh, DEFAULT_TRANSCENNTAL_OP_COUNT);
DEFINE_UNARY_TRANSCENDENTAL(real, acosh, DEFAULT_TRANSCENNTAL_OP_COUNT);
DEFINE_UNARY_TRANSCENDENTAL(real, atanh, DEFAULT_TRANSCENNTAL_OP_COUNT);
DEFINE_UNARY_TRANSCENDENTAL(real, asin, DEFAULT_TRANSCENNTAL_OP_COUNT);
DEFINE_UNARY_TRANSCENDENTAL(real, acos, DEFAULT_TRANSCENNTAL_OP_COUNT);
DEFINE_UNARY_TRANSCENDENTAL(real, atan, DEFAULT_TRANSCENNTAL_OP_COUNT);
DEFINE_BINARY_TRANSCENDENTAL(integer, pow, DEFAULT_TRANSCENNTAL_OP_COUNT);
DEFINE_BINARY_TRANSCENDENTAL(real, pow, DEFAULT_TRANSCENNTAL_OP_COUNT);
DEFINE_BINARY_TRANSCENDENTAL(real, atan2, DEFAULT_TRANSCENNTAL_OP_COUNT);
DEFINE_BINARY_TRANSCENDENTAL(integer, max, 1);
DEFINE_BINARY_TRANSCENDENTAL(integer, min, 1);
DEFINE_BINARY_TRANSCENDENTAL(integer, copysign, 1);
DEFINE_BINARY_TRANSCENDENTAL(real, max, 1);
DEFINE_BINARY_TRANSCENDENTAL(real, min, 1);
DEFINE_BINARY_TRANSCENDENTAL(real, copysign, 1);

}
#endif /* OCTOPH_MATRIX_MATH_HPP_ */
