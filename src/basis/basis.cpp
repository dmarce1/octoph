/*
 * basis.cpp
 *
 *  Created on: Jan 16, 2019
 *      Author: dmarce1
 */

#include "basis/basis.hpp"
#include "basis/legendre.hpp"

#include <stdio.h>

#define PMAX 16

namespace basis {

static basis_type qpts_[PMAX + 1];
static basis_type qwts_[PMAX + 1];

struct basis_init {
	basis_init() {
		using namespace basis;
		std::vector<real> roots;
		for (int n = 1; n < PMAX; n++) {
			auto tmp = roots;
			roots = find_legendre_roots(n, tmp);
			qwts_[n].resize(roots.size());
			qpts_[n].resize(roots.size());
			for (int i = 0; i < roots.size(); i++) {
				qpts_[n][i] = roots[i];
				qwts_[n][i] = 2.0;
				qwts_[n][i] /= (real(1) - roots[i] * roots[i]);
				const real dpdx = dlegendre_p_dx(n, roots[i]);
				qwts_[n][i] /= dpdx * dpdx;
			}
		}
	}
};


basis_type p_to_h(const basis_type& p) {
	const int n = p.size();
	return basis_type();

}

basis_type h_to_p(const basis_type& h) {

	return basis_type();
}


static basis_init basis_init_instance;

}
