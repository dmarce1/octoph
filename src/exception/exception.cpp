/*
 * exception.cpp
 *
 *  Created on: Jan 30, 2019
 *      Author: dmarce1
 */

#include <octoph/exception/exception.hpp>

exception::exception(int line, const char* file, const char* function, const char* message) :
		line_(line), file_(file), function_(function), message_(message == nullptr ? "" : message) {
}

void exception::print_and_exit() const {
	printf("Exception thrown by %s: %s line %i.\n", function_.c_str(), file_.c_str(), line_);
	if (message_ != std::string()) {
		printf("Error Message: %s\n", message_.c_str());
	}
	std::exit(-1);
}

#ifdef TEST_EXCEPTION

int main() {
	try {
		THROW( "testing");
	} catch(const exception& e) {
		e.print_and_exit();
	}
	return 0;
}

#endif
