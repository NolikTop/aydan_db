#include "UnsignedInt32Token.h"

using namespace parser::type;

std::string UnsignedInt32Token::getStringId() const {
	return "uint";
}

db::COLUMN_T UnsignedInt32Token::getColumnType() const {
	return db::CT_UNSIGNED_INT32;
}
