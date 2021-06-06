
#pragma once

#include <parser/UserValueToken.h>
#include <list/List.h>
#include "Column.h"
#include <binary/Stream.h>

namespace db {

	class Row {
	public:
		list::List<db::Column>* columns;
		list::List<parser::UserValueBaseToken>* values;

		Row();

		[[nodiscard]] std::string toString() const;

		void deserialize(binary::Stream* bs) const;
		void serialize(binary::Stream* bs) const;

		void checkForFilled() const;

		void cleanValues() const;
	};

}
