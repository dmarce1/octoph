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

	using C::C;

	using value_type = typename C::value_type;

	inline C& operator=(const C& other) {
		return dynamic_cast<C*>(this)->operator=(other);
	}


	inline const value_type& operator[]( int i ) const  {
#ifndef NDEBUG
		if( i < 0 || i >= C::size()) {
			THROW( "Container index out of bounds - range is %i - to %i - %i requested\n", 0, int(C::size() - 1), i);
		}
#endif
		return dynamic_cast<const C*>(this)->operator[](i);
	}



	inline value_type& operator[]( int i ) {
#ifndef NDEBUG
		if( i < 0 || i >= C::size()) {
			THROW( "Container index out of bounds - range is %i - to %i - %i requested\n", 0, int(C::size() - 1), i);
		}
#endif
		return dynamic_cast<C*>(this)->operator[](i);
	}



};

template<class T>
using vector = safe_container<std::vector<T>>;


template<class T, auto N>
using array = safe_container<std::array<T,N>>;


}




#endif /* OCTOPH_CONTAINERS_CONTAINERS_HPP_ */
