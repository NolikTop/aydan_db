
#pragma once

#include "Token.h"
#include <db/ColumnType.h>
#include <string>
#include <binary/Stream.h>

namespace parser {

	class UserValueBaseToken : public Token {
	public:
		db::COLUMN_T type;

		[[nodiscard]] virtual std::string toString() const = 0;

		explicit UserValueBaseToken(db::COLUMN_T type);
	};

	template<typename T>
	class UserValueToken : public UserValueBaseToken {
	public:
		T value;

		[[nodiscard]] std::string getStringId() const override;

		UserValueToken(db::COLUMN_T type, T value);

		[[nodiscard]] std::string toString() const override;
		void serialize(binary::Stream* bs, db::COLUMN_T type) const;
		void deserialize(binary::Stream* bs, db::COLUMN_T type);
	};

}
