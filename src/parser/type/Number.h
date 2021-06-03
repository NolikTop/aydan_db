
#pragma once

#include <parser/Token.h>
#include "TypeToken.h"

namespace parser::type {

	class Number : public TypeToken {
	public:
		[[nodiscard]] std::string getStringId() const override;
		[[nodiscard]] db::COLUMN_T getColumnType() const override;
	};

}
