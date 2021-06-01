
#pragma once

#include "parser/Token.h"

namespace parser::type {

	class TypeToken : public Token {
		[[nodiscard]] std::string getStringId() const override = 0;
	};

}
