
#pragma once

#include "Token.h"
#include <db/ColumnType.h>
#include <string>

namespace parser {

	template<typename T>
	class UserValueToken : Token {
		db::COLUMN_T type;
		T value;

		[[nodiscard]] std::string getStringId() const override;

		UserValueToken(db::COLUMN_T type, T value);

		[[nodiscard]] std::string toString() const;
	};

}
