#include "Table.h"
#include "Exception.h"

#include <utility>

using namespace db;

Table::Table(std::string name) :
name(std::move(name)), rows(0),
columns(new list::List<Column>()),
primaryKey(nullptr) {}

Table::Table(std::string name, size_t rows, list::List<Column>* columns) :
name(std::move(name)), rows(rows),
columns(columns)
{
	for(auto i = columns->first; i != nullptr; i = i->next){
		if(i->element->primaryKey){
			this->primaryKey = i->element;
			return;
		}
	}

	throw Exception("Couldn't find primaryKey column");
}

void Table::addColumn(Column *c, bool isPrimaryKey, bool isAutoIncrement) {
	c->primaryKey = isPrimaryKey;
	c->autoIncrement = isAutoIncrement;
	this->columns->push_back(c);
	if(isPrimaryKey){
		if(this->primaryKey != nullptr){
			throw Exception("Can't add column \"" + c->name + "\": there is another primary key column \"" + this->primaryKey->name + "\"");
		}

		this->primaryKey = c;
	}
}