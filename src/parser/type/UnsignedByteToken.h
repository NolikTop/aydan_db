
#pragma once

#include <parser/Token.h>
#include "TypeToken.h"

namespace parser::type {

	class UnsignedByteToken : public TypeToken {
		[[nodiscard]] std::string getStringId() const override;
	};

}
