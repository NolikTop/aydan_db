
#pragma once

#include <parser/Token.h>
#include "TypeToken.h"

namespace parser::type {

	class SignedLongToken : public TypeToken {
		[[nodiscard]] std::string getStringId() const override;
	};

}
