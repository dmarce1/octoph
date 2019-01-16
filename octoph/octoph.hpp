/*
 * octoph.hpp
 *
 *  Created on: Jan 15, 2019
 *      Author: dmarce1
 */

#ifndef OCTOPH_OCTOPH_HPP_
#define OCTOPH_OCTOPH_HPP_

#include <array>

static constexpr int ndim = 3;

using dim_type = int;
using child_type = int;
using dir_type = int;

template< class T >
using space_vector = std::array<T,ndim>;


#endif /* OCTOPH_OCTOPH_HPP_ */
