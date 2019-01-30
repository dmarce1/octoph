/*
 * cell_client.hpp
 *
 *  Created on: Jan 15, 2019
 *      Author: dmarce1
 */

#ifndef OCTOPH_CELL_CELL_CLIENT_HPP_
#define OCTOPH_CELL_CELL_CLIENT_HPP_

#include <octoph/cell/cell_server.hpp>

class cell_client {
private:
	hpx::id_type id_;
public:
	template<class Arc>
	void serialize( Arc& arc, unsigned ) {
		arc & id_;
	}
};



#endif /* OCTOPH_CELL_CELL_CLIENT_HPP_ */
