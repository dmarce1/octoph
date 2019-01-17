/*
 * ndim.hpp
 *
 *  Created on: Jan 17, 2019
 *      Author: dmarce1
 */

#ifndef OCTOPH_NDIM_HPP_
#define OCTOPH_NDIM_HPP_

#include "octoph.hpp"


#include <array>

#ifdef OCTOPH_3D
#define NDIM 3
#else
#ifdef OCTOPH_2D
#define NDIM 2
#else
#ifdef OCTOPH_1D
#define NDIM 1
#else
#error 'NDIM undefined'
#endif
#endif
#endif

template< class T >
using space_vector = std::array<T,NDIM>;






#endif /* OCTOPH_NDIM_HPP_ */
