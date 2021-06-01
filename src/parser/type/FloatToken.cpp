#include "FloatToken.h"

using namespace parser::type;

std::string FloatToken::getStringId() const {
	return "float";
}

db::COLUMN_T FloatToken::getColumnType() const {
	return db::CT_FLOAT;
}
