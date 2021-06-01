
#pragma once

#include "TypeToken.h"

namespace parser::type {

	class StringToken : public TypeToken {
		[[nodiscard]] std::string getStringId() const override;
	};

}
