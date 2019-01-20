/*
 * octoph.hpp
 *
 *  Created on: Jan 15, 2019
 *      Author: dmarce1
 */

#ifndef OCTOPH_OCTOPH_HPP_
#define OCTOPH_OCTOPH_HPP_

#include <array>
#include "math/math.hpp"


using dim_type = int;
using child_type = int;
using dir_type = int;


//using real = double;
using real = math::real;


#ifndef NDIM
#error
#endif

#ifndef PMAX
#define PMAX 128
#endif


#endif /* OCTOPH_OCTOPH_HPP_ */
