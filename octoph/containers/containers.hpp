/*
 * containers.hpp
 *
 *  Created on: Feb 3, 2019
 *      Author: dmarce1
 */

#ifndef OCTOPH_CONTAINERS_CONTAINERS_HPP_
#define OCTOPH_CONTAINERS_CONTAINERS_HPP_


#include <octoph/exception/exception.hpp>

namespace ctrs {


template<class C>
struct safe_container : public C {
private:
	inline auto access( auto i )  {
#ifndef NDEBUG
		if( i < 0 || i >= C::size()) {
			THROW( "Container index out of bounds\n");
		}
#endif
		return C::operator[](i);
	}
public:
	using C::C;
	inline auto operator[]( auto i ) const  {
		return access(i);
	}
	inline auto& operator[]( auto i )  {
		return access(i);
	}
};

template<class T>
using vector = safe_container<std::vector<T>>;


template<class T, auto N>
using array = safe_container<std::array<T,N>>;


}




#endif /* OCTOPH_CONTAINERS_CONTAINERS_HPP_ */
