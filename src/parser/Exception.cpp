//
// Created by NolikTop on 18.04.2021.
//

#include "Exception.h"

#include <utility>

using namespace parser;

Exception::Exception(const char *reason) : reason(std::string(reason)){}

Exception::Exception(std::string reason) : reason(std::move(reason)) {}

const char *Exception::what() const noexcept {
	return reason.c_str();
}
