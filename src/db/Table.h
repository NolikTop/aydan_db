
#pragma once

#include <string>
#include <list/List.h>
#include "ColumnType.h"
#include "Column.h"

namespace db {

	class Table {
	public:
		std::string name;
		size_t rows;

		list::List<Column>* columns;

		Column* primaryKey;

		explicit Table(std::string name);
		Table(std::string name, size_t rows, list::List<Column>* columns);

		void addColumn(Column* c, bool isPrimaryKey, bool isAutoIncrement);
	};

}
