/*
 * cell_directory.hpp
 *
 *  Created on: Jan 16, 2019
 *      Author: dmarce1
 */

#ifndef OCTOPH_CELL_CELL_DIRECTORY_HPP_
#define OCTOPH_CELL_CELL_DIRECTORY_HPP_

#include "cell/cell_client.hpp"
#include "cell/cell_location.hpp"

#include <hpx/lcos/local/mutex.hpp>

#include <unordered_map>

class cell_directory {
private:
	using directory_type = std::unordered_map<cell_location,cell_client,cell_location::hash>;
	using mutex_type = hpx::lcos::local::mutex;
	static directory_type dir_;
	static mutex_type mtx_;
public:
	static hpx::future<void> add_entry(const cell_location&, const cell_client&);
	static hpx::future<cell_client> get_entry(const cell_location&);
	static hpx::future<void> del_entry(const cell_location&);
	~cell_directory();
};

#endif /* OCTOPH_CELL_CELL_DIRECTORY_HPP_ */
