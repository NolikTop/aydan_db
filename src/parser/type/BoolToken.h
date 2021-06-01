
#pragma once

#include <parser/Token.h>
#include "TypeToken.h"

namespace parser::type {

	class BoolToken : public TypeToken {
	public:
		[[nodiscard]] std::string getStringId() const override;
	};

}
