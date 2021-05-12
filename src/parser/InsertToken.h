
#pragma once

#include "Token.h"

namespace parser {

	struct InsertToken : public Token {

		[[nodiscard]] std::string getStringId() const override;

	};

}
