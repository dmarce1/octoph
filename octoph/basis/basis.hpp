/*
 * basis.hpp
 *
 *  Created on: Jan 16, 2019
 *      Author: dmarce1
 */

#ifndef OCTOPH_BASIS_BASIS_HPP_
#define OCTOPH_BASIS_BASIS_HPP_

#include "basis/legendre.hpp"

#include <valarray>

namespace basis {

using basis_type = std::valarray<real>;

basis_type p_to_h( const basis_type& );
basis_type h_to_p( const basis_type& );

}


#endif /* OCTOPH_BASIS_BASIS_HPP_ */
