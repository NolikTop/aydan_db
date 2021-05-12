//
// Created by NolikTop on 18.04.2021.
//

#include <iostream>
#include <exception>
#include <string>

#pragma once

namespace parser {

	struct Exception : public std::exception {

		const char *reason;

		explicit Exception(const char *reason);
		explicit Exception(const std::string& reason);

		[[nodiscard]] const char *what() const noexcept override;

	};

}