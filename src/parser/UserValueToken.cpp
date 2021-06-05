#include "UserValueToken.h"
#include "Exception.h"
#include <string>
#include <binary/Stream.h>

using namespace parser;

template<typename T>
UserValueToken<T>::UserValueToken(db::COLUMN_T type, T value) : UserValueBaseToken(type), value(value) {}

template<> std::string UserValueToken<std::string>::toString() const {
	return "(string) " + this->value;
}

template<> std::string UserValueToken<int32_t>::toString() const {
	return "(number) " + std::to_string(this->value);
}

template<typename T>
std::string UserValueToken<T>::getStringId() const {
	return "";
}

template<> void UserValueToken<std::string>::serialize(binary::Stream *bs, db::COLUMN_T type) const {
	bs->writeShortString(this->value);
}

template<> void UserValueToken<int32_t>::serialize(binary::Stream *bs, db::COLUMN_T type) const {
	bs->writeSignedInt32(this->value);
}

template<> void UserValueToken<std::string>::deserialize(binary::Stream *bs, db::COLUMN_T type) {
	this->value = bs->readShortString();
}

template<> void UserValueToken<int32_t>::deserialize(binary::Stream *bs, db::COLUMN_T type) {
	this->value = bs->readSignedInt32();
}

UserValueBaseToken::UserValueBaseToken(db::COLUMN_T type) : type(type){}

template class parser::UserValueToken<std::string>;
template class parser::UserValueToken<int32_t>;
