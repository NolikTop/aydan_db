
#pragma once

#include "Token.h"

namespace parser {

	struct DeleteToken : public Token {

		[[nodiscard]] std::string getStringId() const override;

	};

}
