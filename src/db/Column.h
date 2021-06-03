
#pragma once

#include "ColumnType.h"
#include <string>

namespace db {

	class Column {
	public:
		std::string name;
		bool primaryKey;
		bool autoIncrement;

		COLUMN_T type;

		Column(std::string name, COLUMN_T type, bool primaryKey = false, bool autoIncrement = false);
	};

}
