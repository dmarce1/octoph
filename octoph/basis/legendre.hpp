/*
 * legendre.hpp
 *
 *  Created on: Jan 16, 2019
 *      Author: dmarce1
 */

#ifndef OCTOPH_BASIS_LEGENDRE_HPP_
#define OCTOPH_BASIS_LEGENDRE_HPP_

#include "octoph.hpp"

#include <vector>

namespace basis {

real legendre_p( int, real );
real dlegendre_p_dx( int, real );
std::vector<real> find_legendre_roots(int n, const std::vector<real>& prev_roots);

}

#endif /* OCTOPH_BASIS_LEGENDRE_HPP_ */
