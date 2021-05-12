
#pragma once

#include <string>

namespace parser {

	struct Token {
		[[nodiscard]] virtual std::string getStringId() const = 0;
	};

}
