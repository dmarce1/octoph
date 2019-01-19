
#include "matrix/matrix.hpp"


#ifdef TEST_MATRIX

int main() {

	linear::matrix<double,4,4> m;
	linear::matrix<double,4,4> n;
	m.get<2,2>();

	linear::product<decltype(m),decltype(n)> p(m,n);
	p.get<0,0>();

	return 0;
}

#endif
