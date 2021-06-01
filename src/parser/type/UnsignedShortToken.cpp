#include "UnsignedShortToken.h"

using namespace parser::type;

std::string UnsignedShortToken::getStringId() const {
	return "ushort";
}

db::COLUMN_T UnsignedShortToken::getColumnType() const {
	return db::CT_UNSIGNED_SHORT;
}
