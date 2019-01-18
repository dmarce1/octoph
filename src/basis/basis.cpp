/*
 * basis.cpp
 *
 *  Created on: Jan 16, 2019
 *      Author: dmarce1
 */

#include "svec.hpp"
#include "basis/basis.hpp"
#include "basis/legendre.hpp"

#include <stdio.h>

namespace basis {

static std::vector<real> qpts_[PMAX + 1];
static std::vector<real> qwts_[PMAX + 1];

class tri_indexer {
private:
	svec<int> i_;
	int n_;
	bool done_;
public:
	tri_indexer(int);
	tri_indexer& operator++();
	int operator[](int) const;
	bool done() const;
};

bool tri_indexer::done() const {
	return done_;
}

int tri_indexer::operator[](int i) const {
	if (i == NDIM - 1) {
		return i_[i];
	} else {
		return i_[i] - i_[i + 1];
	}
}

tri_indexer::tri_indexer(int n) :
		i_(0), n_(n), done_(false) {
}

tri_indexer& tri_indexer::operator++() {
	const auto tmp = *this;
	int d = NDIM - 1;
	while (!done_ && (i_[d] == i_[d - 1])) {
		d--;
		if (d == 0) {
			done_ = bool(i_[0] == n_);
		}
	}
	i_[d]++;
	for (int z = d + 1; z < NDIM; z++) {
		i_[z] = 0;
	}
	return *this;
}

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

static basis_init basis_init_instance;

}

#ifdef TEST_BASIS

int main() {

	for( basis::tri_indexer i(4); !i.done(); ++i ) {
		for(int d = 0; d < NDIM; d++) {
			printf( "%i ", i[d]);
		}
		printf( "\n");
	}

	return 0;
}

#endif
