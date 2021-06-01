#include "StringToken.h"

using namespace parser::type;

std::string StringToken::getStringId() const {
	return "string";
}

db::COLUMN_T StringToken::getColumnType() const {
	return db::CT_STRING;
}
