#ifndef __CHARACTERISTIC_HPP
#define __CHARACTERISTIC_HPP

#include <octoph/math/polynomial.hpp>

namespace math {


namespace matrix {


template<class A>
struct charactertistic {
	static constexpr auto N = A::nrow;
	static_assert(N==A::ncol);

	charactertistic() {

	}



};



}


}





#endif
