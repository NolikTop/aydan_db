
#pragma once

#include <string>
#include <list/List.h>
#include "ColumnType.h"
#include "Column.h"
#include "Row.h"

namespace db {

	class Table {
	public:
		std::string name;
		int32_t rows;

		list::List<Column>* columns;

		Column* primaryKey;

		int32_t autoIncrementId;

		[[nodiscard]] std::string path() const;

		explicit Table(std::string name);
		Table(std::string name, int32_t rows, list::List<Column>* columns, int32_t autoIncrementId);

		void addColumn(Column* c);
		[[nodiscard]] bool hasColumn(const std::string& colName) const;
		[[nodiscard]] Column* getColumn(const std::string& colName) const;

		void createFiles();

		void appendRow(Row* row);

		[[nodiscard]] Row* getBaseRow() const;

		static void drop(const std::string& name);
		static Table* open(const std::string& name);
		static bool exists(const std::string& name);

	private:
		void saveInformation() const;
		void createAdditionFiles() const;
	};


	template<typename T>
	struct tableFind {
		list::List<db::Row>* operator()(Table* table, Column *col, const std::function<bool(T)>& check);
	};
}
