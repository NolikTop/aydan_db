#include "Column.h"

db::Column::Column(std::string name, COLUMN_T type, bool primaryKey, bool autoIncrement) : name(std::move(name)), type(type), primaryKey(primaryKey), autoIncrement(autoIncrement) {}
