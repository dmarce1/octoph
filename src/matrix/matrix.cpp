#include "matrix/matrix.hpp"
#include <tuple>




#ifdef TEST_MATRIX

int main() {
	std::array<std::array<double,3>,3> a = {{{1,2,3}, {4,5,6}, {7,8,9}}};
	linear::matrix<double,3,3> test (a);
	linear::product<linear::matrix<double,3,3>> test1(test,test);
	linear::matrix<double,3,3> test2 = test1;
	linear::print(test2);
	return 0;
}

#endif
