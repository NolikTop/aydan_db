
#pragma once

#include "Token.h"
#include "UserValueType.h"
#include <db/ColumnType.h>
#include <string>

namespace parser {

	template<typename T>
	class UserValueToken : public Token {
	public:
		USER_VALUE_T type;
		T value;

		[[nodiscard]] std::string getStringId() const override;

		UserValueToken(USER_VALUE_T type, T value);

		[[nodiscard]] std::string toString() const;
	};

}
