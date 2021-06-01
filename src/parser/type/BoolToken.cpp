#include "BoolToken.h"

using namespace parser::type;

std::string BoolToken::getStringId() const {
	return "bool";
}

db::COLUMN_T BoolToken::getColumnType() const {
	return db::CT_BOOL;
}
