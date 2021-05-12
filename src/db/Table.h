
#pragma once

#include <string>
#include "ColumnType.h"

namespace db {

	struct Table {
		std::string name;
		size_t rows;

		unsigned char columnsAmount;
		COLUMN_T* columns;

		COLUMN_T primaryKey;
	};

	typedef struct Table DataBase;

}
