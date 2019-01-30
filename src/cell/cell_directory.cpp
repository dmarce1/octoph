#include <octoph/cell/cell_directory.hpp>

#include <hpx/lcos/local/mutex.hpp>

#include <array>
#include <unordered_map>

using directory_type = std::unordered_map<cell_location,cell_client,cell_location::hash>;
using mutex_type = hpx::lcos::local::mutex;

static mutex_type mtx_;

static std::unordered_map<int, std::shared_ptr<directory_type>> dirs_;
static std::atomic<int> next_handle_ = 0;

static hpx::future<int> gen_handle();
static hpx::future<void> add(int, const cell_location&, cell_client);
static hpx::future<void> del(int, const cell_location&);
static hpx::future<cell_client> get(int, const cell_location&);

HPX_PLAIN_ACTION(gen_handle, gen_handle_action);
HPX_PLAIN_ACTION(add, add_action);
HPX_PLAIN_ACTION(del, del_action);
HPX_PLAIN_ACTION(get, get_action);

static hpx::future<void> add(int dir_index, const cell_location& location, cell_client client) {
	static const auto localities = hpx::find_all_localities();
	static const auto this_id = hpx::get_locality_id();
	hpx::future<void> fut;
	const auto loc_id = location.locality_id();
	if( this_id == loc_id ) {
		std::lock_guard<mutex_type> lock(mtx_);
		if( dirs_[dir_index] == nullptr ) {
			dirs_[dir_index] = std::make_shared<directory_type>();
		}
		(*dirs_[dir_index])[location] = client;
		fut = hpx::make_ready_future();
	} else {
		fut = hpx::async<add_action>(localities[loc_id], dir_index, location, client);
	}
	return std::move(fut);
}

static hpx::future<cell_client> get(int dir_index, const cell_location& location) {
	static const auto localities = hpx::find_all_localities();
	static const auto this_id = hpx::get_locality_id();
	hpx::future<cell_client> fut;
	const auto loc_id = location.locality_id();
	if( this_id == loc_id ) {
		std::lock_guard<mutex_type> lock(mtx_);
		fut = hpx::make_ready_future((*dirs_[dir_index])[location]);
	} else {
		fut = hpx::async<get_action>(localities[loc_id], dir_index, location);
	}
	return std::move(fut);
}

static hpx::future<void> del(int dir_index, const cell_location& location) {
	static const auto localities = hpx::find_all_localities();
	static const auto this_id = hpx::get_locality_id();
	hpx::future<void> fut;
	const auto loc_id = location.locality_id();
	if( this_id == loc_id ) {
		std::lock_guard<mutex_type> lock(mtx_);
		(*dirs_[dir_index]).erase(location);
	} else {
		fut = hpx::async<del_action>(localities[loc_id], dir_index, location);
	}
	return std::move(fut);
}

static hpx::future<int> gen_handle() {
	static const auto localities = hpx::find_all_localities();
	static const auto this_id = hpx::get_locality_id();
	if (this_id == 0) {
		return hpx::make_ready_future(next_handle_++);
	} else {
		return hpx::async<gen_handle_action>(localities[0]);
	}
}




