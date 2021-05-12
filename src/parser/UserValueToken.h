
#pragma once

#include "Token.h"
#include <db/ColumnType.h>
#include <string>

namespace parser {

	struct UserValueToken : Token {
		db::COLUMN_T type;
		void* value;

		[[nodiscard]] std::string getStringId() const override;

		UserValueToken(db::COLUMN_T type, void* value);

		[[nodiscard]] std::string toString() const;
	};

}
