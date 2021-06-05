#include <sstream>
#include <db/Column.h>
#include <db/Table.h>

#include <parser/type/Number.h>

#include <parser/type/StringToken.h>
#include <binary/Stream.h>
#include <db/Row.h>
#include "Operation.h"
#include "Parser.h"
#include "Exception.h"

using namespace parser;
using namespace parser::type;

list::List<parser::type::TypeToken>* Parser::types;

void Parser::initTypes() {
	Parser::types = new list::List<TypeToken>();

	Parser::types->push_back(new Number());
	Parser::types->push_back(new StringToken());
}

void Parser::skipEmpty(std::string::iterator &it, std::string::iterator end, bool skipComma) {
    for(; (isspace(*it) || (skipComma && *it == ',')) && it != end; it++); // скипаем до момента когда можно будет читать
    if(it == end){
        throw Exception("Empty value until the end");
    }
}

std::string Parser::nextWord(std::string::iterator &it, std::string::iterator end, const std::function<bool(char)>& check) {
	std::string res;
    skipEmpty(it, end);
	for(; check(*it) && it != end; it++){
		res += (char)std::tolower(*it);
	}

	return res;
}

// для функций типа isdigit
std::string Parser::nextWord(std::string::iterator &it, std::string::iterator end, int (*check)(int)) {
    auto castedCheck = [&check](char c){
        return (bool)check(c);
    };

    return nextWord(it, end, castedCheck);
}

std::string Parser::nextKeyword(std::string::iterator &it, std::string::iterator end) {
    return nextWord(it, end, isalpha);
}

// name value - тот же keyword, только может быть закрыт `вот так` (вообще лучше давать их вводить только внутри таких кавычек)
std::string Parser::nextNameValue(std::string::iterator &it,
                                  std::string::iterator end) {
	skipEmpty(it, end);
	auto word = nextWord(it, end, [](char c){
        return isalpha(c) || c == '*' || c == '`';
    });
    if(word.empty()){
    	char c = *it;
        throw Exception("Empty word in name value near \"" + std::string(1, c) + "\" symbol ");
    }

    if(word[0] == '`'){
        if(*word.rbegin() == '`'){
            word = word.substr(1, word.length() - 2);
        }else{
            throw Exception("Not closed name value \"" + word + "\"");
        }
    }

    return word;
}

UserValueBaseToken* Parser::nextUserValue(std::string::iterator &it,
                                  std::string::iterator end) {
    skipEmpty(it, end);

    // проверяем строка ли там вообще
    if(*it == '"'){
        return nextStringVal(it, end);
    }

    // bool или float или number/big unsigned number
    auto word = nextWord(it, end, [](char c){
        return isalpha(c) || isdigit(c) || c == '.';
    });
    if(word.empty()){
        throw Exception("Empty word in user value near \"" + std::to_string(*it) + "\" symbol");
    }

    if(word[0] == '"'){ // значит строка
        if(*word.rbegin() == '`'){
            word = word.substr(1, word.length() - 2);
        }else{
            throw Exception("Not closed name value \"" + word + "\"");
        }
    } // что-то иное

    if(word == "false" || word == "true"){
        throw Exception("Boolean values are not supported");
        //return new UserValueToken<bool>(db::CT_BOOL, word[0] != 'f');
    }

    std::stringstream ss(word);

    auto dotPosition = word.find('.');
    if(dotPosition != std::string::npos){ // float
    	throw Exception("Float values are not supported");
    }

    // значит число

    // проверяем влезет ли оно в int32_t
    int32_t try1;
    if(ss >> try1){
        return new UserValueToken<int32_t>(db::CT_NUMBER, try1);
    }

    // что-то непонятное, что мы не можем принять (слишком большое число или там дрянь)
    throw Exception("Unknown user value \"" + word + "\"");

}

UserValueToken<std::string>* Parser::nextStringVal(std::string::iterator &it, std::string::iterator end) {
    if(*it != '"'){
        throw Exception("Not valid string");
    }

    std::stringstream ss;

    bool escapeSymbol = false;
    for(; it != end; ++it){
        char c = *it;

        if(c == '\\'){
            escapeSymbol = !escapeSymbol;

            if(escapeSymbol){
                continue;
            }
        }

        if(c == '"' && !escapeSymbol){ // конец строки
            break;
        }

        ss << c;

        escapeSymbol = false;
    }

    auto word = ss.str();

    if(it == end){
        throw Exception("Wrong string (no end \" symbol): " + word);
    }

    return new UserValueToken<std::string>(db::CT_STRING, word);
}

db::Column *Parser::readColumn(std::string::iterator &it,
                               std::string::iterator end) {
	if(*it == ')') return nullptr;
	auto name = nextNameValue(it, end);

	auto stringIdType = nextKeyword(it, end);
	db::COLUMN_T type = db::CT_UNKNOWN;

	for(auto i = Parser::types->first; i != nullptr; i = i->next){
		auto t = i->element;
		if(t->getStringId() == stringIdType){
			type = t->getColumnType();
			break;
		}
	}

	if(type == db::CT_UNKNOWN){
		throw Exception("Unknown type \"" + stringIdType + "\"");
	}

	skipEmpty(it, end, false);

	if(*it == ',' || *it == ')'){
		return new db::Column(name, type);
	}

	auto nextWord = nextKeyword(it, end);
	if(nextWord == "primary"){
		nextWord = nextKeyword(it, end);

		if(nextWord != "key"){
			throw Exception(R"(After "primary" keyword should be "key")");
		}
	}else if(nextWord == "auto_increment"){
		throw Exception(R"("auto_increment" should be used after "primary key")");
	}else{
		throw Exception("Unknown keyword \"" + nextWord + "\"");
	}

	if(*it == ',' || *it == ')') {
		// это primary key, но не auto_increment
		return new db::Column(name, type, true);
	}

	// тогда наверно auto_increment
	nextWord = nextKeyword(it, end);
	if(nextWord != "auto_increment"){
		throw Exception("Unknown keyword \"" + nextWord + "\"");
	}

	if(*it == ',' || *it == ')') {
		return new db::Column(name, type, true, true);
	}

	throw Exception("Unknown data after column \"" + name + "\" data");
}

std::string Parser::parse(std::string query) {
	if(Parser::types == nullptr){
		initTypes();
	}

	auto it = query.begin();

	auto opRaw = nextKeyword(it, query.end());
	if(opRaw == "select"){
		return Parser::runSelect(query, it);
	}else if(opRaw == "insert"){
		return Parser::runInsert(query, it);
	}else if(opRaw == "update"){
		return Parser::runUpdate(query, it);
	}else if(opRaw == "delete"){
		return Parser::runDelete(query, it);
	}else if(opRaw == "create"){
		return Parser::runCreate(query, it);
	}else if(opRaw == "drop"){
		return Parser::runDrop(query, it);
	}else{
		throw Exception("Unknown operation \"" + opRaw + "\"");
	}
}

std::string Parser::runCreate(std::string &query,
                       std::string::iterator &it) {
    auto end = query.end();
	auto str = nextKeyword(it, end);

	if(str != "table"){
		throw Exception(R"(After "create" keyword you should use "table")");
		// raw string literal юзается потому что иначе тут будет много \"
	}

    auto tableName = nextNameValue(it, end);

	if(db::Table::exists(tableName)){
		throw Exception("Table \"" + tableName + "\" already exists");
	}

	// читаем структуру таблицы

	skipEmpty(it, end);
	if(*it != '('){
        throw Exception("No \"(\" symbol before table structure");
	}

	if(it++ == end){
	    throw Exception("Unexpected end");
	}

	auto* cols = new list::List<db::Column>();
	for(auto col = readColumn(it, end); col != nullptr; col = readColumn(it, end)){
		cols->push_back(col);
	}

	auto table = new db::Table(tableName, 0, cols, 0);
	table->createFiles();

	return "Successfully created table \"" + tableName + "\"";
}

std::string Parser::runDelete(std::string &query,
                       std::string::iterator &it) {
	auto str = nextKeyword(it, query.end());
	if(str != "from"){
		throw Exception(R"(After "delete" keyword you should use "from")");
	}

	throw Exception("Delete operation is not supported");

	return "Successfully deleted rows";
}

std::string Parser::runDrop(std::string &query,
                     std::string::iterator &it) {
	auto end = query.end();
	auto str = nextKeyword(it, end);

	if(str != "table"){
		throw Exception(R"(After "drop" keyword you should use "table")");
		// raw string literal юзается потому что иначе тут будет много \"
	}

	auto tableName = nextNameValue(it, end);

	if(!db::Table::exists(tableName)){
		throw Exception("Table \"" + tableName + "\" doesn't exists");
	}

	db::Table::drop(tableName);

	return "Table \"" + tableName + "\" successfully deleted";
}

std::string Parser::runInsert(std::string &query,
                       std::string::iterator &it) {
	auto end = query.end();
	auto str = nextKeyword(it, end);
	if(str != "into"){
		throw Exception(R"(After "insert" keyword you should use "into")");
		// raw string literal юзается потому что иначе тут будет много \"
	}

	auto tableName = nextNameValue(it, end);

	auto table = db::Table::open(tableName);

	str = nextKeyword(it, end);

	if(str == "values"){
		throw Exception(R"(After table name you should use "values" (you cant specify column names))");
	}

	skipEmpty(it, end);
	if(*it != '('){
		throw Exception("No \"(\" symbol before values");
	}

	if(it++ == end){
		throw Exception("Unexpected end");
	}

	auto row = table->getBaseRow();

	for(auto i = table->columns->first; i != nullptr && *it != ')'; i = i->next){
		auto col = i->element;

		UserValueBaseToken* val;
		if(col->autoIncrement){
			if(col->type != db::CT_NUMBER){
				throw Exception("Column type is not CT_NUMBER");
			}
			val = new UserValueToken<int32_t>(db::CT_NUMBER, table->autoIncrementId++);
		}else{
			val = nextUserValue(it, end);
		}

		row->values->push_back(val);
	}

	table->appendRow(row);

	std::stringstream res;

    res << "Successfully inserted row in table \"";
    res << tableName;
    res << "\"";
    res << "\n";

	res << "|";

	for(auto cols = row->columns->first; cols != nullptr; cols = cols->next){
		auto col = cols->element;

		res << col->toString() << " | ";
	}

	res << "\n";

    res << "\nRow:\n";

    res << row->toString();


	delete row;

	return res.str();
}

std::string Parser::runSelect(std::string &query,
                       std::string::iterator &it) {
	auto end = query.end();
	std::string str;

	str = nextNameValue(it, end);
	if(str != "*"){
		throw Exception("You can select only all columns using '*'");
	}

	if(it == end){
		throw Exception("Unexpected end");
	}

	auto tableName = nextNameValue(it, end);
	auto table = db::Table::open(tableName);

	str = nextKeyword(it, end);
	if(str != "where"){
		throw Exception("After table name should be keyword \"where\"");
	}

	auto conditionColName = nextNameValue(it, end);
	auto conditionCol = table->getColumn(conditionColName);

	skipEmpty(it, end);
	auto check = [](char c){
		return c == '>' || c == '<' || c == '=' || c == '!';
	};

	if(!check(*it)){
		throw Exception("Unknown operation");
	}

	str = nextWord(it, end, check);
	if(it == end){
		throw Exception("Unexpected empty or unknown operation");
	}

	skipEmpty(it, end);

	auto conditionVal = nextUserValue(it, end);
	if(conditionVal->type != conditionCol->type){
		throw Exception("Wrong type in condition value");
	}

	list::List<db::Row>* rows;

    if (str == "==") {
        if (conditionCol->type == db::CT_STRING) {
            auto val = (dynamic_cast<parser::UserValueToken<std::string> *>(conditionVal))->value;
            rows = table->find<std::string>(
                conditionCol,
                [&val](std::string value) {
                    return value == val;
                }
            );
        } else if (conditionCol->type == db::CT_NUMBER) {
            auto val = (dynamic_cast<parser::UserValueToken<int32_t> *>(conditionVal))->value;
            rows = table->find<int32_t>(
                    conditionCol,
                    [&val](int32_t value) {
                        return value == val;
                    }
            );
        } else {
            throw Exception("Unknown type");
        }
    }else if (str == "!=") {
        if(conditionCol->type == db::CT_STRING) {
            auto val = (dynamic_cast<parser::UserValueToken<std::string> *>(conditionVal))->value;
            rows = table->find<std::string>(
                conditionCol,
                [&val](std::string value) {
                    return value != val;
                }
            );
        }else if(conditionCol->type == db::CT_NUMBER) {
            auto val = (dynamic_cast<parser::UserValueToken<int32_t> *>(conditionVal))->value;
            rows = table->find<int32_t>(
                conditionCol,
                [&val](int32_t value){
                    return value != val;
                }
            );
        }else{
            throw Exception("Unknown type");
        }
    } else if (str == ">") {
        if(conditionCol->type != db::CT_NUMBER) {
            throw Exception("This operation supported only by numbers");
        }

        auto val = (dynamic_cast<parser::UserValueToken<int32_t> *>(conditionVal))->value;
        rows = table->find<int32_t>(
                conditionCol,
                [&val](int32_t value){
                    return value > val;
                }
        );
    } else if (str == "<") {
        if(conditionCol->type != db::CT_NUMBER) {
            throw Exception("This operation supported only by numbers");
        }

        auto val = (dynamic_cast<parser::UserValueToken<int32_t> *>(conditionVal))->value;
        rows = table->find<int32_t>(
                conditionCol,
                [&val](int32_t value){
                    return value < val;
                }
        );
    } else if (str == ">=") {
        if(conditionCol->type != db::CT_NUMBER) {
            throw Exception("This operation supported only by numbers");
        }

        auto val = (dynamic_cast<parser::UserValueToken<int32_t> *>(conditionVal))->value;
        rows = table->find<int32_t>(
                conditionCol,
                [&val](int32_t value){
                    return value >= val;
                }
        );
    } else if (str == "<=") {
        if(conditionCol->type != db::CT_NUMBER) {
            throw Exception("This operation supported only by numbers");
        }

        auto val = (dynamic_cast<parser::UserValueToken<int32_t> *>(conditionVal))->value;
        rows = table->find<int32_t>(
                conditionCol,
                [&val](int32_t value){
                    return value <= val;
                }
        );
    } else {
        throw Exception("Unknown operation \"" + str + "\"");
    }

    std::stringstream res;

    auto rowsAmount = rows->len();
    if(rowsAmount == 0){
        return "Empty set";
    }

    res << "|";

    for(auto cols = rows->first->element->columns->first; cols != nullptr; cols = cols->next){
        auto col = cols->element;

        res << col->toString() << " | ";
    }

    res << "\n";

    for(auto rElement = rows->first; rElement != nullptr; rElement = rElement->next){
        auto row = rElement->element;

        res << row->toString() << "\n";
    }

    res << rowsAmount;
    if(rowsAmount == 1){
        res << " row";
    }else{
        res << " rows";
    }
    res << " in set";

    return res.str();
}

std::string Parser::runUpdate(std::string &query,
                       std::string::iterator &it) {
    throw Exception("Update operation is not supported");

    return "Successfully updated rows";
}