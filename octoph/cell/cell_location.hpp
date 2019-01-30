/*
 * cell_location.hpp
 *
 *  Created on: Jan 15, 2019
 *      Author: dmarce1
 */

#ifndef OCTOPH_CELL_CELL_LOCATION_HPP_
#define OCTOPH_CELL_CELL_LOCATION_HPP

#include <octoph/octoph.hpp>
#include <bits/stdc++.h>
#include <cstdint>

class cell_location {
private:
	using int_type = std::uint64_t;
	static constexpr int nbits_ = sizeof(int_type)*CHAR_BIT;
	static constexpr int maxlevel_ = (nbits_ - 1) / NDIM;
	static int msb(int_type);
	int_type i_;
public:
	cell_location() = default;
	cell_location(const cell_location&) = default;
	cell_location(cell_location&&) = default;
	cell_location& operator=(const cell_location&) = default;
	cell_location& operator=(cell_location&&) = default;
	~cell_location() = default;
	cell_location gen_parent() const;
	cell_location gen_child(child_type) const;
	cell_location gen_sibling(dim_type, dir_type) const;
	std::size_t locality_id() const;
	int level() const;
	bool operator==( const cell_location& other ) const;
	struct hash {
		std::size_t operator()( const cell_location& ) const;
	};
	template<class Arc>
	void serialize( Arc& arc, unsigned ) {
		arc & i_;
	}
};



#endif /* OCTOPH_CELL_CELL_LOCATION_HPP_ */
