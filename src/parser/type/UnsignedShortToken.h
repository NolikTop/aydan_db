
#pragma once

#include <parser/Token.h>
#include "TypeToken.h"

namespace parser::type {

	class UnsignedShortToken : public TypeToken {
		[[nodiscard]] std::string getStringId() const override;
		[[nodiscard]] db::COLUMN_T getColumnType() const override;
	};

}
