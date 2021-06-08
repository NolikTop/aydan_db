#include <sstream>
#include <db/Column.h>
#include <db/Table.h>

#include <parser/type/Number.h>

#include <parser/type/StringToken.h>
#include <binary/Stream.h>
#include <db/Row.h>
#include "Parser.h"
#include "Exception.h"
#include <utils/Colors.h>
#include <iomanip>

using namespace parser;
using namespace parser::type;

list::List<parser::type::TypeToken>* Parser::types;
std::string::iterator Parser::iterator;
std::string::iterator Parser::begin;

void Parser::initTypes() {
	Parser::types = new list::List<TypeToken>();

	Parser::types->push_back(new Number());
	Parser::types->push_back(new StringToken());
}

void Parser::skipEmpty(std::string::iterator end, std::string expected, bool skipComma, bool skipCloseParenthesis) {
	auto& it = Parser::iterator;
    for(; (isspace(*it) || (skipCloseParenthesis && *it == ')') || (skipComma && *it == ',')) && it != end; it++); // скипаем до момента когда можно будет читать
    if(it == end){
    	if(!expected.empty()){
		    throw Exception("Empty value until the end. Expected " + expected);
    	}else{
		    throw Exception("Empty value until the end");
    	}
    }
}

std::string Parser::nextWord(std::string::iterator end, const std::function<bool(char)>& check, std::string expected) {
	auto& it = Parser::iterator;

	std::string res;
	if(expected.empty()){
		expected = "word";
	}
    skipEmpty(end, expected);
	for(; check(*it) && it != end; it++){
		res += (char)std::tolower(*it);
	}

	return res;
}

// для функций типа isdigit
std::string Parser::nextWord(std::string::iterator end, int (*check)(int), std::string expected) {
	auto& it = Parser::iterator;

	auto castedCheck = [&check](char c){
        return (bool)check(c);
    };

    return nextWord(end, castedCheck, expected);
}

std::string Parser::nextKeyword(std::string::iterator end, std::string expected) {
	auto& it = Parser::iterator;

	if(expected.empty()){
		expected = "keyword";
	}
    return nextWord(end,
        [](char c){
            return isalpha(c) || c == '_';
        }, expected);
}

// name value - тот же keyword, только может быть закрыт `вот так` (вообще лучше давать их вводить только внутри таких кавычек)
std::string Parser::nextNameValue(std::string::iterator end, std::string expected) {
	auto& it = Parser::iterator;

	if(expected.empty()){
		expected = "name value";
	}
	skipEmpty(end, expected);
	auto word = nextWord(end, [](char c){
        return isalpha(c) || c == '*' || c == '`';
    }, expected);
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

UserValueBaseToken* Parser::nextUserValue(std::string::iterator end, std::string expected) {
	auto& it = Parser::iterator;

	if(expected.empty()){
		expected = "user value";
	}
    skipEmpty(end, expected);

    // проверяем строка ли там вообще
    if(*it == '"'){
        return nextStringVal(end, expected);
    }

    // bool или float или number/big unsigned number
    auto word = nextWord(end, [](char c){
        return isalpha(c) || isdigit(c) || c == '.' || c == '-';
    }, expected);
    if(word.empty()){
        throw Exception("Empty word in user value near \"" + std::string(1, *it) + "\" symbol");
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
        return new UserValueToken<int32_t>(db::CT_NUMBER, try1, it);
    }

    // что-то непонятное, что мы не можем принять (слишком большое число или там дрянь)
    throw Exception("Unknown user value \"" + word + "\"");

}

UserValueToken<std::string>* Parser::nextStringVal(std::string::iterator end, std::string expected) {
	auto& it = Parser::iterator;

	if(*it != '"'){
        throw Exception("Not valid string");
    }

    std::stringstream ss;

    bool escapeSymbol = false;
    for(++it; it != end; ++it){
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

    it++;

    return new UserValueToken<std::string>(db::CT_STRING, word, it);
}

db::Column *Parser::readColumn(std::string::iterator end) {
	auto& it = Parser::iterator;

	if(*it == ')') return nullptr;
	auto name = nextNameValue(end, "column name");

	auto stringIdType = nextKeyword(end, "column type");
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

	skipEmpty(end, "end of column (\")\" or \",\") or \"primary\" keyword", false, false);

	if(*it == ',' || *it == ')'){
		return new db::Column(name, type);
	}

	auto nextWord = nextKeyword(end, "\"primary\" keyword");
	if(nextWord == "primary"){
		nextWord = nextKeyword(end, "\"key\" keyword");

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
	nextWord = nextKeyword(end, "\"auto_increment\" keyword");
	if(nextWord != "auto_increment"){
		throw Exception("Unknown keyword \"" + nextWord + "\"");
	}

	if(type != db::CT_NUMBER){
		throw Exception("Only number type can be auto incremented");
	}

	skipEmpty(end, "useless space before end", false, false);

	if(*it == ',' || *it == ')') {
		return new db::Column(name, type, true, true);
	}

	throw Exception("Unknown data after column \"" + name + "\" data. Data code=" + std::to_string(*it) + " Data=\"" + *it + "\"");
}

std::string Parser::parse(std::string query) {
	if(Parser::types == nullptr){
		initTypes();
	}

	auto it = query.begin();

	Parser::iterator = it;
	Parser::begin = query.begin();

	auto opRaw = nextKeyword(query.end(), "operation name");
	std::string res;

	if(opRaw == "select"){
		return Parser::runSelect(query);
	}else if(opRaw == "insert"){
		return Parser::runInsert(query);
	}else if(opRaw == "update"){
		return Parser::runUpdate(query);
	}else if(opRaw == "delete"){
		return Parser::runDelete(query);
	}else if(opRaw == "create"){
		return Parser::runCreate(query);
	}else if(opRaw == "drop") {
		return Parser::runDrop(query);
	}else if(opRaw == "describe"){
		return Parser::runDescribe(query);
	}else{
		throw Exception("Unknown operation \"" + opRaw + "\"");
	}
}

std::string Parser::runCreate(std::string &query) {
	auto& it = Parser::iterator;
	auto end = query.end();
	auto str = nextKeyword(end, "\"table\" keyword");

	if(str != "table"){
		throw Exception(R"(After "create" keyword you should use "table")");
		// raw string literal юзается потому что иначе тут будет много \"
	}

    auto tableName = nextNameValue(end, "table name");

	if(db::Table::exists(tableName)){
		throw Exception("Table \"" + tableName + "\" already exists");
	}

	// читаем структуру таблицы

	skipEmpty(end, "skip until \"(\" symbol (after that should be table structure)");
	if(*it != '('){
        throw Exception("No \"(\" symbol before table structure");
	}

	if(it++ == end){
	    throw Exception("Unexpected end");
	}

	auto* cols = new list::List<db::Column>();
	for(auto col = readColumn(end); col != nullptr; col = readColumn(end)){
		cols->push_back(col);
	}

	auto table = new db::Table(tableName, 0, cols, 0);
	table->createFiles();

	return "Successfully created table \"" + tableName + "\"";
}

std::string Parser::runDelete(std::string &query) {
	auto& it = Parser::iterator;

	auto str = nextKeyword(query.end(), "\"from\" keyword");
	if(str != "from"){
		throw Exception(R"(After "delete" keyword you should use "from")");
	}

	throw Exception("Delete operation is not supported");

	return "Successfully deleted rows";
}

std::string Parser::runDrop(std::string &query) {
	auto& it = Parser::iterator;

	auto end = query.end();
	auto str = nextKeyword(end, "\"table\" keyword");

	if(str != "table"){
		throw Exception(R"(After "drop" keyword you should use "table")");
		// raw string literal юзается потому что иначе тут будет много \"
	}

	auto tableName = nextNameValue(end, "table name");

	if(!db::Table::exists(tableName)){
		throw Exception("Table \"" + tableName + "\" doesn't exists");
	}

	db::Table::drop(tableName);

	return "Table \"" + tableName + "\" successfully deleted";
}

std::string Parser::runInsert(std::string &query) {
	auto& it = Parser::iterator;

	auto end = query.end();
	auto str = nextKeyword(end, "\"into\" keyword");
	if(str != "into"){
		throw Exception(R"(After "insert" keyword you should use "into")");
		// raw string literal юзается потому что иначе тут будет много \"
	}

	auto tableName = nextNameValue(end, "table name");

	auto table = db::Table::open(tableName);

	str = nextKeyword(end, "\"values\" keyword");

	if(str != "values"){
		throw Exception(R"(After table name you should use "values" (you cant specify column names))");
	}

	skipEmpty(end, "skip until \"(\" symbol (after that should be values)");
	if(*it != '('){
		throw Exception("No \"(\" symbol before values");
	}

	if(it++ == end){
		throw Exception("Unexpected end");
	}

	auto row = table->getBaseRow();



	for(auto i = table->columns->first; i != nullptr/* && *it != ')'*/; i = i->next){
		auto col = i->element;

		UserValueBaseToken* val;
		if(col->autoIncrement){
			if(col->type != db::CT_NUMBER){
				throw Exception("Column type is not CT_NUMBER");
			}
			val = new UserValueToken<int32_t>(db::CT_NUMBER, table->autoIncrementId++, it);
		}else{
			val = nextUserValue(end, "value for \"" + col->toString() + "\" column");
		}

		row->values->push_back(val);
	}
	if(*it != ')'){
		skipEmpty(end, "Useless space before excess value", false, false);
		throw Exception("Excess value at the end");
	}

	table->appendRow(row);

	std::stringstream res;

    res << "Successfully inserted row in table \"";
    res << GREEN << tableName << RESET;
    res << "\"" << std::endl;

	//region расчет maxWidthCol
	auto colIndex = 0;
	for(auto cols = row->columns->first; cols != nullptr; cols = cols->next, colIndex++){
		auto col = cols->element;

		col->maxWidthCol = std::max(
				(int)row->values->at(colIndex)->toColoredString().length(),
				(int)col->toString().length()
				);
	}
	//endregion

	std::ios init(NULL);
	init.copyfmt(res);

	res << "\n| ";

	for(auto cols = row->columns->first; cols != nullptr; cols = cols->next){
		auto col = cols->element;

		res << std::setw(col->maxWidthCol) << std::left << col->toString();
		res.copyfmt(init);
		res << " | ";
	}

	res << "\n";
    res << row->toString();

	delete row;
	return res.str();
}

std::string Parser::runSelect(std::string &query) {
	auto& it = Parser::iterator;

	auto end = query.end();
	std::string str;

	str = nextNameValue(end, "columns names (but it does not supported, so use \"*\" keyword)");
	if(str != "*"){
		throw Exception("You can select only all columns using '*'");
	}

	str = nextKeyword(end, "\"from\" keyword");
	if(str != "from"){
		throw Exception(R"(After "select *" you should use "from" keyword)");
	}

	auto tableName = nextNameValue(end, "table name");
	auto table = db::Table::open(tableName);

	list::List<db::Row>* rows;

	if(it == end || std::next(it) == end){
		if(table->primaryKey->type == db::CT_NUMBER) {
			rows = table->find<int32_t>(
				table->primaryKey,
				[](int32_t value) {
					return true;
				}
			);
		}else{
			rows = table->find<std::string>(
				table->primaryKey,
				[](std::string value) {
					return true;
				}
			);
		}
	}else{
		str = nextKeyword(end, "\"where\" keyword");
		if (str != "where") {
			throw Exception("After table name should be keyword \"where\"");
		}

		auto conditionColName = nextNameValue(end, "condition column");
		auto conditionCol = table->getColumn(conditionColName);

		skipEmpty(end, "operation");
		auto check = [](char c) {
			return c == '>' || c == '<' || c == '=' || c == '!';
		};

		if (!check(*it)) {
			throw Exception("Unknown operation");
		}

		str = nextWord(end, check);
		if (it == end) {
			throw Exception("Expected condition value");
		}

		skipEmpty(end, "empty space before condition value");

		auto conditionVal = nextUserValue(end, "condition value");
		if (conditionVal->type != conditionCol->type) {
			throw Exception("Wrong type in condition value");
		}

		if (str == "==" || str == "=") {
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
		} else if (str == "!=") {
			if (conditionCol->type == db::CT_STRING) {
				auto val = (dynamic_cast<parser::UserValueToken<std::string> *>(conditionVal))->value;
				rows = table->find<std::string>(
						conditionCol,
						[&val](std::string value) {
							return value != val;
						}
				);
			} else if (conditionCol->type == db::CT_NUMBER) {
				auto val = (dynamic_cast<parser::UserValueToken<int32_t> *>(conditionVal))->value;
				rows = table->find<int32_t>(
						conditionCol,
						[&val](int32_t value) {
							return value != val;
						}
				);
			} else {
				throw Exception("Unknown type");
			}
		} else if (str == ">") {
			if (conditionCol->type != db::CT_NUMBER) {
				throw Exception("This operation supported only by numbers");
			}

			auto val = (dynamic_cast<parser::UserValueToken<int32_t> *>(conditionVal))->value;
			rows = table->find<int32_t>(
					conditionCol,
					[&val](int32_t value) {
						return value > val;
					}
			);
		} else if (str == "<") {
			if (conditionCol->type != db::CT_NUMBER) {
				throw Exception("This operation supported only by numbers");
			}

			auto val = (dynamic_cast<parser::UserValueToken<int32_t> *>(conditionVal))->value;
			rows = table->find<int32_t>(
					conditionCol,
					[&val](int32_t value) {
						return value < val;
					}
			);
		} else if (str == ">=") {
			if (conditionCol->type != db::CT_NUMBER) {
				throw Exception("This operation supported only by numbers");
			}

			auto val = (dynamic_cast<parser::UserValueToken<int32_t> *>(conditionVal))->value;
			rows = table->find<int32_t>(
					conditionCol,
					[&val](int32_t value) {
						return value >= val;
					}
			);
		} else if (str == "<=") {
			if (conditionCol->type != db::CT_NUMBER) {
				throw Exception("This operation supported only by numbers");
			}

			auto val = (dynamic_cast<parser::UserValueToken<int32_t> *>(conditionVal))->value;
			rows = table->find<int32_t>(
					conditionCol,
					[&val](int32_t value) {
						return value <= val;
					}
			);
		} else {
			throw Exception("Unknown operation \"" + str + "\"");
		}

	}

    std::stringstream res;

    auto rowsAmount = rows->len();
    if(rowsAmount == 0){
        return "Empty set";
    }

    //region расчет maxWidthCol
	auto colIndex = 0;
	for(auto cols = rows->first->element->columns->first; cols != nullptr; cols = cols->next, colIndex++){
		auto col = cols->element;

		col->maxWidthCol = col->toString().length();

		for(auto rElement = rows->first; rElement != nullptr; rElement = rElement->next) {
			auto row = rElement->element;
			col->maxWidthCol = std::max((int)row->values->at(colIndex)->toColoredString().length(), col->maxWidthCol);
		}
	}
	//endregion

    res << "| ";

	std::ios init(NULL);
	init.copyfmt(res);

    for(auto cols = rows->first->element->columns->first; cols != nullptr; cols = cols->next){
        auto col = cols->element;

        res << std::setw(col->maxWidthCol) << std::left << col->toString();
	    res.copyfmt(init);
	    res << " | ";
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

std::string Parser::runUpdate(std::string &query) {
    throw Exception("Update operation is not supported");

    return "Successfully updated rows";
}

std::string Parser::runDescribe(std::string &query) {
	auto& it = Parser::iterator;

	auto end = query.end();

	auto tableName = nextNameValue(end, "table name");

	auto table = db::Table::open(tableName);

	std::stringstream res;

	res << "Table name: " << GREEN << table->name << RESET << " | Rows: " << YELLOW << table->rows << RESET << " | Auto increment id: " << MAGENTA << table->autoIncrementId << RESET;
	res << "\nColumns:";
	for(auto cElement = table->columns->first; cElement != nullptr; cElement = cElement->next){
		auto c = cElement->element;
		res << "\n";
		res << c->toString();
		if(c->primaryKey){
			res << BOLDYELLOW << " [PRIMARY KEY]";
			if(c->autoIncrement){
				res << BOLDGREEN << " {AUTO_INCREMENT}";
			}
		}
		res << RESET;
	}

	return res.str();
}
