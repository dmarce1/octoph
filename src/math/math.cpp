#include "math/math.hpp"

namespace math {

std::atomic<std::uint64_t> real::gops_(0);
std::atomic<std::uint64_t> integer::gops_(0);

thread_local std::atomic<std::uint64_t> real::tops_(0);
thread_local std::atomic<std::uint64_t> integer::tops_(0);

}
using real = math::real;
#include <stdio.h>

#ifdef TEST_MATH


int main() {
}
#endif
