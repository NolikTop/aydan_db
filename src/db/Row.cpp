#include "Row.h"
#include "Exception.h"

std::string db::Row::toString() const {
	std::string res;

	auto vElement = values->first;
	for(
		auto cElement = columns->first;
		cElement != nullptr && vElement != nullptr;
		cElement = cElement->next,
		vElement = vElement->next
	){
		auto col = cElement->element;
		auto val = vElement->element;

		res += col->name + ": " + val->toString();
	}

	return res;
}

void db::Row::deserialize(binary::Stream *bs) const {
	for(auto cElement = columns->first; cElement != nullptr; cElement = cElement->next){
		auto col = cElement->element;
		parser::UserValueBaseToken* val;
		switch (col->type) {
			case db::CT_STRING:
				val = new parser::UserValueToken<std::string>(col->type, bs->readShortString());
				break;
			case db::CT_NUMBER:
				val = new parser::UserValueToken<int32_t>(col->type, bs->readSignedInt32());
				break;
			case db::CT_BOOL:
				val = new parser::UserValueToken<bool>(col->type, bs->readUnsignedByte() != 0);
				break;
			default:
				throw Exception("Unknown type " + std::to_string(col->type));
		}

		values->push_back(val);
	}
}

void db::Row::serialize(binary::Stream* bs) const {
	auto vElement = values->first;
	for(
		auto cElement = columns->first;
		cElement != nullptr && vElement != nullptr;
		cElement = cElement->next,
		vElement = vElement->next
	) {
		auto col = cElement->element;
		auto val = vElement->element;

		if (col->type != val->type) {
			throw Exception("Wrong input for \"" + col->name + "\" column");
		}

		switch (val->type) {
			case db::CT_STRING:
				(dynamic_cast<parser::UserValueToken<std::string> *>(val))->serialize(bs, col->type);
				break;
			case db::CT_NUMBER:
				(dynamic_cast<parser::UserValueToken<int32_t> *>(val))->serialize(bs, col->type);
				break;
			case db::CT_BOOL:
				(dynamic_cast<parser::UserValueToken<bool> *>(val))->serialize(bs, col->type);
				break;
			default:
				throw Exception("Unknown type " + std::to_string(val->type));
		}
	}
}

db::Row::Row() : columns(new list::List<db::Column>()), values(new list::List<parser::UserValueBaseToken>()){

}

void db::Row::cleanValues() const {
	this->values->clear();
}
