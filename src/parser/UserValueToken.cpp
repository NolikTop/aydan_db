#include "UserValueToken.h"
#include "Exception.h"
#include "UserValueType.h"
#include <string>

using namespace parser;

UserValueToken::UserValueToken(db::COLUMN_T type, void *value) : type(type), value(value) {}

// только для дебага
std::string UserValueToken::toString() const {
	std::string str_val;

	switch(this->type){
		case UT_BOOL:
			if(*((bool *)this->value)){
				str_val = "true";
			}else{
				str_val = "false";
			}
			return "(bool) " + str_val;
		case UT_FLOAT:
			str_val = std::to_string(*((float*)this->value));
			return "(float) " + str_val;
		case UT_NUMBER:
			str_val = std::to_string(*((int*)this->value));
			return "(number) " + str_val;
		case UT_STR:
			str_val = *((std::string*)this->value);
			return "(string) \"" + str_val + "\"";
		default:
			return "unknown type (" + std::to_string(this->type) + ")"
	}
}

std::string UserValueToken::getStringId() const {
	return "";
}
