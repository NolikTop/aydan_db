
#pragma once

#include <string>

namespace parser {

	class Token {
		[[nodiscard]] virtual std::string getStringId() const = 0;
	};

}
