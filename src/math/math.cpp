#include "math/math.hpp"

namespace math {

std::atomic<std::uint64_t> real::gops_(0);
std::atomic<std::uint64_t> integer::gops_(0);

thread_local std::atomic<std::uint64_t> real::tops_(0);
thread_local std::atomic<std::uint64_t> integer::tops_(0);

}
using real = math::real;
#include <stdio.h>

#include <array>

template<class T>
constexpr auto update_index(T& j, T k) {
	if (j > k) {
		return j - 1;
	} else {
		return j;
	}
}

template<class T>
constexpr auto equal_to(T& j, T k) {
	return j == k;
}

template<class ...T>
constexpr auto one_true(T ...b) {
return (b || ... );
}

template<class I, class J, class ...Rest>
constexpr int levita_cita(I i, J j, Rest ...args) {
if constexpr (sizeof...(Rest) == 0) {
	if (i == j) {
		return 0;
	} else if (i == 0) {
		return +1;
	} else {
		return -1;
	}
} else if (i == j || one_true(equal_to(args,i)...)) {
	return 0;
} else if (i % 2 == 0) {
	return +levita_cita(update_index(j, i), update_index(args,i)...);
} else {
	return -levita_cita(update_index(j, i), update_index(args,i)...);
}
}

#include <complex>

#include "math/polynomial.hpp"
#ifdef TEST_MATH
#include <array>
int main() {

auto p = polynomial<std::complex<double>,3>(std::array<std::complex<double>,3>( { {1,0,-1}}));
p.roots();

return 0;

for( int i = 0; i < 4; i++) {
	for( int j = 0; j < 4; j++) {
		for( int k = 0; k < 4; k++) {
			for( int l = 0; l < 4; l++) {
				printf( "%i %i %i %i | %i\n", i, j, k, l, levita_cita(i,j,k,l));
			}
		}
	}
}

}
#endif

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
