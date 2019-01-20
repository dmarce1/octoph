#include "matrix/matrix.hpp"
#include <tuple>

namespace linear {

mask<row_mask<1, 0, 0>, row_mask<1, 0, 1>, row_mask<0, 1, 1>> test;


}

#ifdef TEST_MATRIX

int main() {
	using row_type = typename linear::row_mask<1,0,0>;
	linear::matrix<double,4,5> test;
	row_type::index<1>();
	return 0;
}

#endif
