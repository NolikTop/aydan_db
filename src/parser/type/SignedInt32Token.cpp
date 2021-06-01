#include "SignedInt32Token.h"

using namespace parser::type;

std::string SignedInt32Token::getStringId() const {
	return "int";
}

db::COLUMN_T SignedInt32Token::getColumnType() const {
	return db::CT_SIGNED_INT32;
}
