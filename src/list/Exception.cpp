//
// Created by NolikTop on 18.04.2021.
//

#include "Exception.h"

using namespace list;

Exception::Exception(const char *reason) : reason(reason){}

Exception::Exception(const std::string& reason) : reason(reason.c_str()) {}

const char *Exception::what() const noexcept {
	return reason;
}
