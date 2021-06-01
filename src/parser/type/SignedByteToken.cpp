#include "SignedByteToken.h"

using namespace parser::type;

std::string SignedByteToken::getStringId() const {
	return "byte";
}

db::COLUMN_T SignedByteToken::getColumnType() const {
	return db::CT_SIGNED_BYTE;
}
