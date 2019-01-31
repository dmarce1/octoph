/*
 * exception.cpp
 *
 *  Created on: Jan 30, 2019
 *      Author: dmarce1
 */

#include <octoph/exception/exception.hpp>

exception::exception(int line, const char* file, const char* function, const char* message = nullptr) :
		line_(line), file_(file), function_(function), message_(message) {
}
