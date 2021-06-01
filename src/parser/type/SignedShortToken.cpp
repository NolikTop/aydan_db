#include "SignedShortToken.h"

using namespace parser::type;

std::string SignedShortToken::getStringId() const {
	return "short";
}

db::COLUMN_T SignedShortToken::getColumnType() const {
	return db::CT_SIGNED_SHORT;
}
