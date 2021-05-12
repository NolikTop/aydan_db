
#pragma once

#include "Token.h"

namespace parser {

	struct CreateToken : public Token {

		[[nodiscard]] std::string getStringId() const override;

	};

}
