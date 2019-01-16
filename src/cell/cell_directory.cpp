#include "cell/cell_directory.hpp"

std::vector<cell_directory::directory_type> cell_directory::dir_;
std::vector<cell_directory::mutex_type> cell_directory::mtx_;

HPX_PLAIN_ACTION(cell_directory::add_entry_with_id, add_entry_with_id_action);
HPX_PLAIN_ACTION(cell_directory::get_entry_with_id, get_entry_with_id_action);
HPX_PLAIN_ACTION(cell_directory::del_entry_with_id, del_entry_with_id_action);

hpx::future<void> cell_directory::add_entry(int i, const cell_location& loc,
		const cell_client& client) {
	static const auto localities = hpx::find_all_localities();
	const auto id = loc.locality_id();
	hpx::future<void> fut;
	if (id == hpx::get_locality_id()) {
		{
			std::lock_guard<mutex_type> lock(mtx_[i]);
			dir_[i][loc] = client;
		}
		fut = hpx::make_ready_future<void>();
	} else {
		fut = hpx::async<add_entry_with_id_action>(localities[id], i, loc, client);
	}
	return std::move(fut);
}

hpx::future<cell_client> cell_directory::get_entry_with_id(int i, const cell_location& loc) {
	static const auto localities = hpx::find_all_localities();
	const auto id = loc.locality_id();
	hpx::future<cell_client> fut;
	if (id == hpx::get_locality_id()) {
		std::lock_guard<mutex_type> lock(mtx_[i]);
		fut = hpx::make_ready_future(dir_[i][loc]);
	} else {
		fut = hpx::async<get_entry_with_id_action>(localities[id], i, loc);
	}
	return std::move(fut);
}

hpx::future<void> cell_directory::del_entry_with_id(int i, const cell_location& loc) {
	static const auto localities = hpx::find_all_localities();
	hpx::future<void> fut;
	const auto id = loc.locality_id();
	if (id == hpx::get_locality_id()) {
		{
			std::lock_guard<mutex_type> lock(mtx_[i]);
			dir_[i].erase(loc);
		}
		fut = hpx::make_ready_future<void>();
	} else {
		fut = hpx::async<del_entry_with_id_action>(localities[i], loc);
	}
	return std::move(fut);
}

