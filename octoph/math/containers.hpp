/*
 * containers.hpp
 *
 *  Created on: Jan 31, 2019
 *      Author: dmarce1
 */

#ifndef OCTOPH_MATH_CONTAINERS_HPP_
#define OCTOPH_MATH_CONTAINERS_HPP_

#include <stack>
#include <functional>
#include <memory>

template<class Container>
struct math_container: public Container {
	using Container::Container;
	using value_type = typename Container::value_type;

	Container& operator=(const Container& other) {
		return Container::operator=(other);
	}

	Container& operator=(Container&& other) {
		return Container::operator=(std::move(other));
	}

	~math_container() {
		if( owners.size()) {
			auto me = std::make_shared<Container>(std::move(*this));
			do {
				owners.top()(me);
				owners.pop();
			} while( owners.size());
		}
	}

private:
	std::stack<std::function<void(std::shared_ptr<Container>)>> owners;

};

#endif /* OCTOPH_MATH_CONTAINERS_HPP_ */
