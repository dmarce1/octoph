#include <octoph/math/matrix/matrix.hpp>
#include <octoph/math/matrix/product.hpp>
#include <octoph/math/matrix/determinant.hpp>
#include <octoph/math/matrix/add.hpp>

#include <octoph/math/matrix/inverse.hpp>
#include <octoph/math/matrix/transpose.hpp>
#include <octoph/math/math.hpp>
#include <octoph/math/matrix/add.hpp>

#include <octoph/math/matrix/QR.hpp>
#include <limits>

#include <octoph/math/polynomial.hpp>
#include <array>

#ifdef TEST_MATRIX

class test {
public:
	int operator[]( std::array<double,3> t) {
		return 0;
	}
};
int main() {
	test t;
	return t[std::array<double,3>()];
}

#endif
