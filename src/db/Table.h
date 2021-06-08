
#pragma once

#include <string>
#include <list/List.h>
#include <functional>
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

        size_t columnIndex(Column* col) const;

        template<typename T>
        bool shouldAddRow(parser::UserValueBaseToken* val, const std::function<bool(T)>& check);

        template<typename T>
        list::List<db::Row>* find(Column *col, const std::function<bool(T)>& check);

        static void drop(const std::string& name);
		static Table* open(const std::string& name);
		static bool exists(const std::string& name);

	private:
		void saveInformation() const;
		void createAdditionFiles() const;
	};

}
