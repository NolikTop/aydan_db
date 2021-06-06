
#pragma once

#include "ColumnType.h"
#include <string>

namespace db {

	class Column {
	public:
		std::string name;
		bool primaryKey;
		bool autoIncrement;

		int maxWidthCol = 0;

		COLUMN_T type;

		Column(std::string name, COLUMN_T type, bool primaryKey = false, bool autoIncrement = false);
	    [[nodiscard]] std::string toString() const;
	    [[nodiscard]] std::string toCleanString() const;
	};

}
