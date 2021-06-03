#include "Number.h"

using namespace parser::type;

std::string Number::getStringId() const {
	return "number";
}

db::COLUMN_T Number::getColumnType() const {
	return db::CT_NUMBER;
}
