#include "UnsignedLongToken.h"

using namespace parser::type;

std::string UnsignedLongToken::getStringId() const {
	return "ulong";
}

db::COLUMN_T UnsignedLongToken::getColumnType() const {
	return db::CT_UNSIGNED_LONG;
}
