
#pragma once

#include "Token.h"

namespace parser {

	struct SelectToken : public Token {
		[[nodiscard]] std::string getStringId() const override;
	};

}
