
#pragma once

#include "Token.h"

namespace parser {

	struct DropToken : public Token {

		[[nodiscard]] std::string getStringId() const override;

	};

}
