#include "UnsignedByteToken.h"

using namespace parser::type;

std::string UnsignedByteToken::getStringId() const {
	return "ubyte";
}

db::COLUMN_T UnsignedByteToken::getColumnType() const {
	return db::CT_UNSIGNED_BYTE;
}
