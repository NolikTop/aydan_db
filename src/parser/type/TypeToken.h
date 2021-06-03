
#pragma once

#include <db/ColumnType.h>
#include "parser/Token.h"

namespace parser::type {

	class TypeToken : public Token {
	public:
		[[nodiscard]] std::string getStringId() const override = 0;
		[[nodiscard]] virtual db::COLUMN_T getColumnType() const = 0;
	};

}
