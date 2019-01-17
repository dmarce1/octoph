/*
 * legendre.cpp
 *
 *  Created on: Jan 17, 2019
 *      Author: dmarce1
 */

#include "basis/legendre.hpp"

#include <functional>

namespace basis {

static real find_root(const std::function<real(real)>& f, real a, real c);

real legendre_p(int n, real x) {
	if (n > 1) {
		return (real(2 * n - 1) * x * legendre_p(n - 1, x) - (n - 1) * legendre_p(n - 2, x)) / real(n);
	} else if (n == 1) {
		return x;
	} else {
		return real(1);
	}
}

real dlegendre_p_dx(int n, real x) {
	if (n > 0) {
		return real(n) * (x * legendre_p(n, x) - legendre_p(n - 1, x)) / (x * x - real(1));
	} else {
		return real(0);
	}
}

std::vector<real> find_legendre_roots(int n, const std::vector<real>& prev_roots) {
	const auto p = [n](real x) {
		return legendre_p(n,x);
	};
	std::vector<real> roots;
	if (n > 1) {
		std::vector<std::pair<real, real>> ranges;
		ranges.push_back(std::make_pair(-real(1), prev_roots[0]));
		for (int i = 0; i < prev_roots.size() - 1; i++) {
			ranges.push_back(std::make_pair(prev_roots[i], prev_roots[i + 1]));
		}
		ranges.push_back(std::make_pair(prev_roots[n - 2], +real(1)));
		for (const auto& range : ranges) {
			const real root = find_root(p, range.first, range.second);
			roots.push_back(root);
		}
	} else {
		roots.push_back(real(0));
	}
	return std::move(roots);
}

static real find_root(const std::function<real(real)>& f, real a, real c) {
	real b;
	while (true) {
		b = (a + c) / real(2);
		if (a == b || b == c) {
			break;
		} else {
			const real fa = f(a);
			const real fb = f(b);
			if (fb == 0.0) {
				break;
			}
			if (fa * fb < 0.0) {
				c = b;
			} else {
				a = b;
			}
		}
	}
	return b;
}

}

#ifdef TESTME

#include <iostream>
#define PMAX 32

int main() {
	using namespace basis;
	printf( "Computing roots of first %i Legendre polynomials\n", PMAX);
	std::vector<real> roots;
	for( int n = 1; n < PMAX; n++) {
		auto tmp = roots;
		roots = find_legendre_roots(n,tmp);
		printf( "%3i ", n);
		for( auto r : roots) {
			printf( "%14e ", r );
		}
		printf( "\n");
	}
	return 0;
}

#endif
