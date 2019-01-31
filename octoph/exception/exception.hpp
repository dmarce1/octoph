/*
 * exception.hpp
 *
 *  Created on: Jan 30, 2019
 *      Author: dmarce1
 */

#ifndef EXCEPTION_HPP_
#define EXCEPTION_HPP_




struct exception {
	int line_;
	std::string file_;
	std::string function_;
	std::string message_;
	exception(int line, const char* file, const char* function, const char* message = nullptr);
};


#endif /* EXCEPTION_HPP_ */
