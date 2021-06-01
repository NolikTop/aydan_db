#include "SignedLongToken.h"

using namespace parser::type;

std::string SignedLongToken::getStringId() const {
	return "long";
}

db::COLUMN_T SignedLongToken::getColumnType() const {
	return db::CT_SIGNED_LONG;
}
