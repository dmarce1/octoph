#include "math/math.hpp"

namespace math {

std::atomic<std::uint64_t> real::gops_(0);
std::atomic<std::uint64_t> integer::gops_(0);

thread_local std::atomic<std::uint64_t> real::tops_(0);
thread_local std::atomic<std::uint64_t> integer::tops_(0);

}
using real = math::real;
#include <stdio.h>
/*
#define BINARY_OPERATOR(op)                   \
template<class A, class B>                    \
auto operator op (const A& a, const B& b) {   \
	constexpr auto N = a.size();              \
	static_assert(N==b.size());               \
	struct op_type {                          \
		constexpr bool is_operator() const {  \
			return true;                      \
		}                                     \
		const A& a_;                          \
		const B& b_;                          \
		op_type(const A& a, const B& b) :     \
				a_(a), b_(b) {                \
		}                                     \
		constexpr auto size() const {         \
			return N;                         \
		}                                     \
		inline auto operator[](int i) const { \
			return a_[i] op b_[i];            \
		}                                     \
	};                                        \
	return op_type(a, b);                     \
}

#define UNARY_OPERATOR(op)                    \
template<class A>                             \
auto operator op (const A& a) {               \
	constexpr auto N = a.size();              \
	struct op_type {                          \
		constexpr bool is_operator() const {  \
			return true;                      \
		}                                     \
		const A& a_;                          \
		op_type(const A& a) :                 \
				a_(a) {                       \
		}                                     \
		constexpr auto size() const {         \
			return N;                         \
		}                                     \
		inline auto operator[](int i) const { \
			return op a_[i];                  \
		}                                     \
	};                                        \
	return op_type(a);                        \
}

BINARY_OPERATOR(+);
BINARY_OPERATOR(-);
BINARY_OPERATOR(*);
BINARY_OPERATOR(/);
BINARY_OPERATOR(%);
BINARY_OPERATOR(&);
BINARY_OPERATOR(|);
BINARY_OPERATOR(^);
BINARY_OPERATOR(&&);
BINARY_OPERATOR(||);
BINARY_OPERATOR(>);
BINARY_OPERATOR(>=);
BINARY_OPERATOR(<);
BINARY_OPERATOR(<=);
BINARY_OPERATOR(==);
BINARY_OPERATOR(!=);
UNARY_OPERATOR(-);
UNARY_OPERATOR(+);
UNARY_OPERATOR(!);
UNARY_OPERATOR(~);
*/

#ifdef TEST_MATH
#include <array>
int main() {
//	std::array<double,4> A, B;
//	auto test = A * (A + B);

}
#endif
