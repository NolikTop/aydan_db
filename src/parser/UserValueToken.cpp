#include "UserValueToken.h"
#include "Exception.h"
#include "UserValueType.h"
#include <string>

using namespace parser;

template<typename T>
UserValueToken<T>::UserValueToken(db::COLUMN_T type, T value) : type(type), value(value) {}

// только для дебага
template<typename T>
std::string UserValueToken<T>::toString() const {
	std::string str_val;

	switch(this->type){
		case UT_BOOL:
			if(this->value){
				str_val = "true";
			}else{
				str_val = "false";
			}
			return "(bool) " + str_val;
		case UT_FLOAT:
			str_val = std::to_string(this->value);
			return "(float) " + str_val;
		case UT_NUMBER:
			str_val = std::to_string(this->value);
			return "(number) " + str_val;
		case UT_STR:
			str_val = this->value;
			return "(string) \"" + str_val + "\"";
		default:
			return "unknown type (" + std::to_string(this->type) + ")"
	}
}

template<typename T>
std::string UserValueToken<T>::getStringId() const {
	return "";
}
