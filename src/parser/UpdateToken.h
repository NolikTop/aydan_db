
#pragma once

#include "Token.h"

namespace parser {

	struct UpdateToken : public Token {

		[[nodiscard]] std::string getStringId() const override;

	};

}
