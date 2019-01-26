/*
 * number.hpp
 *
 *  Created on: Jan 25, 2019
 *      Author: dmarce1
 */

#ifndef OCTOPH_MATH_NUMBER_HPP_
#define OCTOPH_MATH_NUMBER_HPP_

#include <atomic>






#define NUMBER_BINARY_OPERATOR( name, op )                          \
template<class A, class B>                                          \
struct name {                                                       \
	const typename std::enable_if<A::is_number, A>::type& A_;       \
	const typename std::enable_if<B::is_number, B>::type& B_;       \
	static constexpr auto OP_COUNT = A::OP_COUNT + B::OP_COUNT + 1; \
	name(const A& a, const B& b) :                                  \
			A_(a), B_(b) {                                          \
	}                                                               \
	auto get() const {                                              \
		return A_.get() op B_.get();                                \
	}                                                               \
};                                                                  \
template<class V>                                                   \
auto operator op (const V& other) const {                           \
	return name(*this, other);                                      \
}

#define NUMBER_BINARY_OPERATOR_ACCUMULATE( name, op )               \
template<class A>                                                   \
number& operator op##= (const A& a) {                               \
	n_ += a.get();                                                  \
	global_ops_ op##= A::OP_COUNT + 1;                              \
	local_ops_ op##= A::OP_COUNT + 1;                               \
	return *this;                                                   \
}

#define NUMBER_UNARY_OPERATOR( name, op )                           \
template<class A>                                                   \
struct name {                                                       \
	const typename std::enable_if<A::is_number, A>::type& A_;       \
	static constexpr auto OP_COUNT = A::OP_COUNT + 1;               \
	name(const A& a) :                                              \
			A_(a) {                                                 \
	}                                                               \
	T get() const {                                                 \
		return op A_.get();                                         \
	}                                                               \
};                                                                  \
template<class V>                                                   \
auto operator op () const {                                         \
	return name(*this);                                             \
}

#define DECLARE_NUMBER_UNARY_FUNCTION( op )                         \
template<class A>                                                   \
struct op##_type {                                                  \
	const typename std::enable_if<A::is_number, A>::type& A_;       \
	static constexpr auto OP_COUNT = A::OP_COUNT + 1;               \
	op##_type(const A& a) :                                         \
			A_(a) {                                                 \
	}                                                               \
	T get() const {                                                 \
		return std:: op ( A_.get() );                               \
	}                                                               \
};                                                                  \
template<class V>                                                   \
friend auto op (const V&)

#define DEFINE_NUMBER_UNARY_FUNCTION( op )                          \
template<class V>                                                   \
auto op (const V& v) {                                              \
	return number<V>::op##_type(v);                                 \
}

template<class T = double>
struct number {
	static constexpr bool is_number = true;
	static std::atomic<std::uint64_t> global_ops_;
	static thread_local std::uint64_t local_ops_;

	static constexpr auto OP_COUNT = 0;

	number() = default;
	number(const number& other) = default;
	number& operator=(const number& other) = default;
	number(const T& other) :
			n_(other) {
	}
	auto operator=(const T& other) {
		n_ = other;
		return *this;
	}

	auto get() const {
		return n_;
	}

	operator T() const {
		return n_;
	}

	DECLARE_NUMBER_UNARY_FUNCTION(sin);
	DECLARE_NUMBER_UNARY_FUNCTION(cos);
	DECLARE_NUMBER_UNARY_FUNCTION(tan);
	DECLARE_NUMBER_UNARY_FUNCTION(sinh);
	DECLARE_NUMBER_UNARY_FUNCTION(cosh);
	DECLARE_NUMBER_UNARY_FUNCTION(tanh);
	DECLARE_NUMBER_UNARY_FUNCTION(asin);
	DECLARE_NUMBER_UNARY_FUNCTION(acos);
	DECLARE_NUMBER_UNARY_FUNCTION(atan);
	DECLARE_NUMBER_UNARY_FUNCTION(asinh);
	DECLARE_NUMBER_UNARY_FUNCTION(acosh);
	DECLARE_NUMBER_UNARY_FUNCTION(atanh);
	DECLARE_NUMBER_UNARY_FUNCTION(exp);
	DECLARE_NUMBER_UNARY_FUNCTION(log);
	DECLARE_NUMBER_UNARY_FUNCTION(abs);

	NUMBER_BINARY_OPERATOR_ACCUMULATE(add,+);
	NUMBER_BINARY_OPERATOR_ACCUMULATE(sub,-);
	NUMBER_BINARY_OPERATOR_ACCUMULATE(div,/);
	NUMBER_BINARY_OPERATOR_ACCUMULATE(mod,%);
	NUMBER_BINARY_OPERATOR_ACCUMULATE(mul,*);

	NUMBER_BINARY_OPERATOR(add,+);
	NUMBER_BINARY_OPERATOR(sub,-);
	NUMBER_BINARY_OPERATOR(mul,*);
	NUMBER_BINARY_OPERATOR(div,/);
	NUMBER_BINARY_OPERATOR(mod,%);
	NUMBER_UNARY_OPERATOR(pos,+);
	NUMBER_UNARY_OPERATOR(neg,-);

	NUMBER_BINARY_OPERATOR(bor,|);
	NUMBER_BINARY_OPERATOR(band,&);
	NUMBER_BINARY_OPERATOR(bxor,^);
	NUMBER_UNARY_OPERATOR(bnot,~);

	NUMBER_BINARY_OPERATOR(gt,>);
	NUMBER_BINARY_OPERATOR(gte,>=);
	NUMBER_BINARY_OPERATOR(lt,<);
	NUMBER_BINARY_OPERATOR(lte,<=);
	NUMBER_BINARY_OPERATOR(eq,==);
	NUMBER_BINARY_OPERATOR(neq,!=);

	auto operator++() {
		global_ops_++;
		local_ops_++;
		n_++;
		return *this;
	}

	auto operator++(int) {
		global_ops_++;
		local_ops_++;
		n_++;
		return *this;
	}

	auto operator--() {
		global_ops_++;
		local_ops_++;
		n_--;
		return *this;
	}

	auto operator--(int) {
		global_ops_++;
		local_ops_++;
		n_--;
		return *this;
	}

	template<class A>
	number& operator=(const A& a) {
		n_ = a.get();
		global_ops_ += A::OP_COUNT;
		local_ops_ += A::OP_COUNT;
		return *this;
	}

	static auto make_global_counter() {
		return global_counter();
	}

	static auto make_local_counter() {
		return local_counter();
	}

	friend class global_counter;
	friend class local_counter;

private:
	T n_;

	struct global_counter {
		global_counter() : start_(number::global_ops_) {
		}
		int read() const {
			return number::global_ops_ - start_;
		}
	private:
		int start_;
	};

	struct local_counter {
		local_counter() : start_(number::local_ops_) {
		}
		int read() const {
			return number::global_ops_ - start_;
		}
	private:
		int start_;
	};
};

DEFINE_NUMBER_UNARY_FUNCTION(sin);
DEFINE_NUMBER_UNARY_FUNCTION(cos);
DEFINE_NUMBER_UNARY_FUNCTION(tan);
DEFINE_NUMBER_UNARY_FUNCTION(asin);
DEFINE_NUMBER_UNARY_FUNCTION(acos);
DEFINE_NUMBER_UNARY_FUNCTION(atan);
DEFINE_NUMBER_UNARY_FUNCTION(asinh);
DEFINE_NUMBER_UNARY_FUNCTION(acosh);
DEFINE_NUMBER_UNARY_FUNCTION(atanh);
DEFINE_NUMBER_UNARY_FUNCTION(sinh);
DEFINE_NUMBER_UNARY_FUNCTION(cosh);
DEFINE_NUMBER_UNARY_FUNCTION(tanh);
DEFINE_NUMBER_UNARY_FUNCTION(log);
DEFINE_NUMBER_UNARY_FUNCTION(exp);
DEFINE_NUMBER_UNARY_FUNCTION(abs);

template<class T>
std::atomic<std::uint64_t> number<T>::global_ops_;

template<class T>
thread_local std::uint64_t number<T>::local_ops_;
DEFINE_BINARY_TRANSCENDENTAL(real, copysign, 1);


#endif /* OCTOPH_MATH_NUMBER_HPP_ */
