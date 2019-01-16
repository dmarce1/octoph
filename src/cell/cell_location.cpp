/*
 * cell_location.cpp
 *
 *  Created on: Jan 15, 2019
 *      Author: dmarce1
 */

#include "cell/cell_location.hpp"

#include <hpx/runtime/find_localities.hpp>

constexpr int cell_location::nbits_;
constexpr int cell_location::maxlevel_;

int cell_location::msb(int_type i) {
#ifdef __GNUC__
	if (i) {
		return nbits_ - __builtin_clzll(i);
	} else {
		return 0;
	}
#else
#error
#endif
}

bool cell_location::operator==( const cell_location& other ) const {
	return other.i_ == i_;

}


std::size_t cell_location::hash::operator()(const cell_location& cell) const {
	static const auto num_localities = hpx::find_all_localities().size();
	static const std::hash<int_type> f;
	return f(cell.i_ / num_localities);
}

std::size_t cell_location::locality_id() const {
	static const auto num_localities = hpx::find_all_localities().size();
	return i_ % num_localities;
}

int cell_location::level() const {
	return msb(i_) / ndim;
}

cell_location cell_location::gen_parent() const {
	cell_location cell;
	cell.i_ = i_ >> ndim;
	return cell;
}

cell_location cell_location::gen_child(child_type ci) const {
	cell_location cell;
	if (msb(i_) < nbits_ - ndim) {
		cell.i_ = (i_ << ndim) | ci;
	} else {
		cell.i_ = 0;
	}
	return cell;
}

cell_location cell_location::gen_sibling(dim_type dim, dir_type dir) const {
	cell_location cell;
	int_type mask = 1 << dim;
	int_type j = i_;
	if (dir > 0) {
		while ((mask & i_ == mask) && mask) {
			j &= ~mask;
			mask <<= ndim;
		}
	} else if (dir < 0) {
		while ((mask & i_ != mask) && mask) {
			j |= mask;
			mask <<= ndim;
		}
	}
	cell.i_ = mask ? (j ^ mask) : 0;
	return cell;
}

