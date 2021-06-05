#include "Table.h"
#include "Exception.h"
#include "FS.h"
#include "SettingsType.h"

#include <utility>
#include <binary/Stream.h>
#include "Row.h"
#include <list/List.h>
#include <string>
#include <parser/Exception.h>

using namespace db;

std::string Table::path() const {
	return db::dbPath + this->name + "/";
}

Table::Table(std::string name) :
name(std::move(name)), rows(0),
columns(new list::List<Column>()),
primaryKey(nullptr), autoIncrementId(0) {}

Table::Table(std::string name, int32_t rows, list::List<Column>* columns, int32_t autoIncrementId) :
name(std::move(name)), rows(rows),
columns(columns), autoIncrementId(autoIncrementId)
{
	if(columns->len() > 255){
		throw Exception("Max columns amount is 255");
	}
	for(auto i = columns->first; i != nullptr; i = i->next){
		if(i->element->primaryKey){
			this->primaryKey = i->element;
			return;
		}
	}

	throw Exception("Couldn't find primaryKey column");
}

void Table::addColumn(Column *c) {
	if(this->columns->len() == 255) {
		throw Exception("Max columns amount is 255");
	}
	this->columns->push_back(c);
	if(c->primaryKey){
		if(this->primaryKey != nullptr){
			throw Exception("Can't add column \"" + c->name + "\": there is another primary key column \"" + this->primaryKey->name + "\"");
		}

		this->primaryKey = c;
	}
}

bool Table::hasColumn(const std::string& colName) const {
	for(auto i = this->columns->first; i != nullptr; i = i->next){
		if(i->element->name == colName){
			return true;
		}
	}
	return false;
}

Column* Table::getColumn(const std::string &colName) const {
	for(auto i = this->columns->first; i != nullptr; i = i->next){
		if(i->element->name == colName){
			return i->element;
		}
	}
	throw Exception("Unknown column \"" + colName + "\"");
}

void Table::createFiles() {
	this->saveInformation();
	this->createAdditionFiles();
}

void Table::appendRow(Row* row) {
	auto bs = new binary::Stream();

	row->serialize(bs);
	auto buf = bs->getBufferAsString();
	delete bs;

	this->rows++;
	this->saveInformation();

	std::ofstream dataset(this->path() + "dataset.bin", std::ios::app);
	dataset << buf;
	dataset.close();
}

size_t Table::columnIndex(Column *col) const {
    auto c = 0;
    for(auto cElement = this->columns->first; cElement != nullptr; cElement = cElement->next, ++c){
        if(col->name == cElement->element->name){
            return c;
        }
    }

    throw Exception("Column \"" + col->name + "\" not found");
}

template<typename T>
bool Table::shouldAddRow(parser::UserValueBaseToken* val, const std::function<bool(T)> &check) {
	T vall = (dynamic_cast<parser::UserValueToken<T> *>(val))->value;

	return check(vall);
}

template<typename T>
list::List<db::Row> *Table::find(Column *col, const std::function<bool(T)> &check) {
    std::ifstream dataset(this->path() + "dataset.bin");
    if(!dataset.good()){
        throw Exception("Couldn't open dataset.bin");
    }
    auto foundRows = new list::List<db::Row>();

    std::string str((std::istreambuf_iterator<char>(dataset)),
                    std::istreambuf_iterator<char>());

    auto bs = new binary::Stream(str.length(), (byte*)str.c_str());
    auto colIndex = this->columnIndex(col);

    while(!bs->eof()){
        auto row = this->getBaseRow();

        row->deserialize(bs);
        auto condVal = row->values->at(colIndex);

        if(this->shouldAddRow<T>(condVal, check)) {
            foundRows->push_back(row);
        }
    }

    dataset.close();

	return foundRows;
}

void Table::drop(const std::string &name) {
	const auto path = db::dbPath + name + "/";

	auto cmd = "rm -r \"" + path + "\"";

	system(cmd.c_str());
}

Table *Table::open(const std::string &name) {
	const auto path = db::dbPath + name + "/";

	std::ifstream information(path + "information.bin");
	if(!information.good()){
		throw Exception("Table \"" + name + "\" does not exists");
	}

	std::string info((std::istreambuf_iterator<char>(information)),
	                     std::istreambuf_iterator<char>());
	information.close();

	auto table = new Table(name);
	auto bs = new binary::Stream(info.length(), (byte *) info.c_str());

	auto colsCount = bs->readUnsignedByte();
	for(auto i = 0; i < colsCount; ++i){
		auto colName = bs->readByteString();
		auto colType = bs->readUnsignedByte();

		table->addColumn(new Column(colName, colType));
	}

	auto settingsAmount = bs->readUnsignedByte();
	for(auto i = 0; i < settingsAmount; ++i){
		auto setting = bs->readUnsignedByte();
		auto settingField = bs->readByteString();
		switch (setting) {
			case ST_AUTO_INCREMENT:
				if(table->primaryKey == nullptr){
					throw Exception("Cant set auto increment field if there is not primary key field");
				}

				if(table->primaryKey->name != settingField){
					throw Exception("Cant make non-primary key field auto increment");
				}

				table->primaryKey->autoIncrement = true;
				break;
			case ST_PRIMARY_KEY:
				if(table->primaryKey != nullptr){
					throw Exception(
							"Cant make field \"" + settingField +
							"\". There is primary key field \"" + table->primaryKey->name + "\""
							);
				}

				table->primaryKey = table->getColumn(settingField);
				table->primaryKey->primaryKey = true;
				break;
			default:
				throw Exception("Unknown setting type " + std::to_string(setting));
		}
	}

	if(table->primaryKey != nullptr && table->primaryKey->autoIncrement){
		table->autoIncrementId = bs->readSignedInt32();
	}

	table->rows = bs->readSignedInt32();

	return table;
}

bool Table::exists(const std::string& name) {
	return FS::fileExists(name + "/information.bin");
}

void Table::saveInformation() const {
	const std::string path = this->path() + "information.bin";

	auto bs = new binary::Stream();
	bs->writeUnsignedByte((uint8_t)this->columns->len());

	for(auto i = this->columns->first; i != nullptr; i = i->next){
		auto column = i->element;

		bs->writeByteString(column->name);
		bs->writeUnsignedByte(column->type);
	}

	auto settingsAmount = 0;
	if(this->primaryKey != nullptr){
		settingsAmount++;
		if(this->primaryKey->autoIncrement){
			settingsAmount++;
		}
	}

	bs->writeUnsignedByte(settingsAmount);

	if(this->primaryKey != nullptr) {
		bs->writeUnsignedByte(ST_PRIMARY_KEY);
		bs->writeByteString(this->primaryKey->name);

		if (this->primaryKey->autoIncrement) {
			bs->writeUnsignedByte(ST_AUTO_INCREMENT);
			bs->writeByteString(this->primaryKey->name);

			bs->writeUnsignedLong(this->autoIncrementId); // autoIncrementId
		}
	}

	bs->writeUnsignedLong(this->rows); // кол во строк

	std::ofstream info(path);
	if(!info.good()){
		info.close();
		throw Exception("Cant open file \"" + path + "\"");
	}

	info.clear();
	info << bs->buffer;

	info.close();

	delete bs;
}

void Table::createAdditionFiles() const {
	const std::string path = this->path();

	if(this->primaryKey != nullptr){
		std::ofstream primaryKeys(path + "primary_keys.bin");

		if(!primaryKeys.good()){
			primaryKeys.close();
			throw Exception("Cant open file \"" + path + "primary_keys.bin\"");
		}

		// файл пустой должен быть по началу
		primaryKeys << "";

		primaryKeys.close();
	}

	std::ofstream dataset(path + "dataset.bin");

	if(!dataset.good()){
		throw Exception("Cant open file \"" + path + "dataset.bin\"");
	}

	// файл пустой должен быть по началу
	dataset << "";

	dataset.close();
}

Row *Table::getBaseRow() const {
	auto row = new Row();
	for(auto i = this->columns->first; i != nullptr; i = i->next) {
		auto column = i->element;

		row->columns->push_back(column);
	}

	return row;
}

template list::List<db::Row>* Table::find<std::string>(Column *col, const std::function<bool(std::string)>& check);
template list::List<db::Row>* Table::find<int32_t>(Column *col, const std::function<bool(int32_t)>& check);
template bool Table::shouldAddRow<std::string>(parser::UserValueBaseToken* val, const std::function<bool(std::string)>& check);
template bool Table::shouldAddRow<int32_t>(parser::UserValueBaseToken* val, const std::function<bool(int32_t)>& check);