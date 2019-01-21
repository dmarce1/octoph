#include "matrix/matrix.hpp"
#include "matrix/product.hpp"
#include "matrix/determinant.hpp"



#ifdef TEST_MATRIX

int main() {

//	std::array<std::array<double,3>,3> a = { { {1,0,0}, {0,1,0}, {0,0,1}}};
	std::array<std::array<double,3>,3> a = { { {1,0,0}, {0,1,0}, {0,0,1}}};
	using mask_type = linear::mask<linear::integer_sequence<std::size_t,1,1,0>,linear::integer_sequence<std::size_t,1,1,1>,linear::integer_sequence<std::size_t,1,1,1>>;
	//using mask_type = linear::mask_all_true<3,3>;

	mask_type mask;
//	mask.print();
//	printf( "\n");
	linear::matrix<double,3,3,mask_type> test (a);
	print(test);
//	printf( "\n");
	auto test1 = linear::product(test,test);
	linear::matrix<double,3,3> test2 = test1;
//
//	printf( "\n");
///
	print(test2);
	auto c = linear::comatrix<decltype(test),1,2>(test);
//	printf( "\n");
//	linear::matrix<double,2,2> test3 = co;
//	print(test3);
	auto d = linear::determinant(test);
	printf( "%e\n", d.get());
	return 0;
}

#endif
