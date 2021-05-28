
#pragma once

#include <string>
#include "ColumnType.h"

namespace db {

	class Table {
		std::string name;
		size_t rows;

		unsigned char columnsAmount;
		COLUMN_T* columns;

		COLUMN_T primaryKey;
	};

}