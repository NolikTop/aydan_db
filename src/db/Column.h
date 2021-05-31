
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
	};

}
