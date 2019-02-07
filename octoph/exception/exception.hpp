/*
 * exception.hpp
 *
 *  Created on: Jan 30, 2019
 *      Author: dmarce1
 */

#ifndef EXCEPTION_HPP_
#define EXCEPTION_HPP_

#include <string>

struct exception {
	int line_;
	std::string file_;
	std::string function_;
	std::string message_;

	template<class ...Args>
	exception(int line, const char* file, const char* function, Args&&...args) :
			line_(line), file_(file), function_(function), message_() {
		char* ptr;
		asprintf(&ptr, std::forward<Args>(args)...);
		message_ = ptr;
		free(ptr);
	}

	void print_and_exit() const;
};

#ifndef NDEBUG
#define THROW( ... ) throw exception( __LINE__, __FILE__, __FUNCTION__, __VA_ARGS__)
#else
#define THROW( ... ) exception( __LINE__, __FILE__, __FUNCTION__, __VA_ARGS__).print_and_exit();
#endif

#endif /* EXCEPTION_HPP_ */
