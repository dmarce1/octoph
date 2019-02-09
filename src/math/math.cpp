#include <octoph/math/math.hpp>
#include <octoph/math/containers.hpp>
#include <hpx/hpx_init.hpp>

#include <octoph/math/multiindex.hpp>

#ifdef TEST_MATH

int main(int argc, char* argv[]) {
	hpx::init(argc, argv);
}

int hpx_main(int argc, char* argv[]) {

	multiindex<0,0,0> test;

	auto a = multiindex_increment<3>(test);
	auto b = multiindex_increment<3>(a);
	auto c = multiindex_increment<3>(b);
	auto d = multiindex_increment<3>(c);
	auto e = multiindex_increment<3>(d);
	auto f = multiindex_increment<3>(e);
	auto g = multiindex_increment<3>(f);
	auto h = multiindex_increment<3>(g);
	auto i = multiindex_increment<3>(h);
	auto j = multiindex_increment<3>(i);
	a.print();
	b.print();
	c.print();
	d.print();
	e.print();
	f.print();
	g.print();
	h.print();
	i.print();
	j.print();

	return hpx::finalize();

}
#endif
