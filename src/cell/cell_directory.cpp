#include "cell/cell_directory.hpp"

cell_directory::directory_type cell_directory::dir_;
cell_directory::mutex_type cell_directory::mtx_;

HPX_PLAIN_ACTION(cell_directory::add_entry, add_entry_action);
HPX_PLAIN_ACTION(cell_directory::get_entry, get_entry_action);
HPX_PLAIN_ACTION(cell_directory::del_entry, del_entry_action);

hpx::future<void> cell_directory::add_entry(const cell_location& loc,
		const cell_client& client) {
	static const auto localities = hpx::find_all_localities();
	const auto id = loc.locality_id();
	hpx::future<void> fut;
	if (id == hpx::get_locality_id()) {
		{
			std::lock_guard<mutex_type> lock(mtx_);
			dir_[loc] = client;
		}
		fut = hpx::make_ready_future<void>();
	} else {
		fut = hpx::async<add_entry_action>(localities[id], loc, client);
	}
	return std::move(fut);
}

hpx::future<cell_client> cell_directory::get_entry(const cell_location& loc) {
	static const auto localities = hpx::find_all_localities();
	const auto id = loc.locality_id();
	hpx::future<cell_client> fut;
	if (id == hpx::get_locality_id()) {
		std::lock_guard<mutex_type> lock(mtx_);
		fut = hpx::make_ready_future(dir_[loc]);
	} else {
		fut = hpx::async<get_entry_action>(localities[id], loc);
	}
	return std::move(fut);
}

hpx::future<void> cell_directory::del_entry(const cell_location& loc) {
	static const auto localities = hpx::find_all_localities();
	hpx::future<void> fut;
	const auto id = loc.locality_id();
	if (id == hpx::get_locality_id()) {
		{
			std::lock_guard<mutex_type> lock(mtx_);
			dir_.erase(loc);
		}
		fut = hpx::make_ready_future<void>();
	} else {
		fut = hpx::async<del_entry_action>(localities[id], loc);
	}
	return std::move(fut);
}

static cell_directory cell_directory_instance_;

cell_directory::~cell_directory() {
	std::lock_guard<mutex_type> lock(mtx_);
	dir_.clear();
}



