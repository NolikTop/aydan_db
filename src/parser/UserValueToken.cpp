#include "UserValueToken.h"
#include "Exception.h"
#include <string>
#include <binary/Stream.h>

using namespace parser;

template<typename T>
UserValueToken<T>::UserValueToken(db::COLUMN_T type, T value) : UserValueBaseToken(type), value(value) {}

// только для дебага
template<typename T>
std::string UserValueToken<T>::toString() const {
	std::string str_val;

	switch(this->type){
		case db::CT_BOOL:
			if(this->value){
				str_val = "true";
			}else{
				str_val = "false";
			}
			return "(bool) " + str_val;
		case db::CT_NUMBER:
			str_val = std::to_string(this->value);
			return "(number) " + str_val;
		case db::CT_STRING:
			str_val = this->value;
			return "(string) \"" + str_val + "\"";
		default:
			return "unknown type (" + std::to_string(this->type) + ")";
	}
}

template<typename T>
std::string UserValueToken<T>::getStringId() const {
	return "";
}

template<typename T>
void UserValueToken<T>::serialize(binary::Stream *bs, db::COLUMN_T type) const {
	switch(type){
		case db::CT_NUMBER:
			bs->writeSignedInt32(this->value);
			break;
		case db::CT_BOOL:
			bs->writeUnsignedByte(this->type ? 1 : 0);
			break;
		case db::CT_STRING:
			bs->writeShortString(this->value);
			break;
		default:
			throw Exception("Unknown column type " + this->type);
	}
}

template<typename T>
void UserValueToken<T>::deserialize(binary::Stream *bs, db::COLUMN_T type) {
	switch(type){
		case db::CT_NUMBER:
			this->value = bs->readSignedInt32();
			break;
		case db::CT_BOOL:
			this->type = bs->readUnsignedByte() != 0;
			break;
		case db::CT_STRING:
			this->value = bs->readShortString();
			break;
		default:
			throw Exception("Unknown column type " + this->type);
	}
}

UserValueBaseToken::UserValueBaseToken(db::COLUMN_T type) : type(type){}
