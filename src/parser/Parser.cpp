#include <sstream>
#include <db/Column.h>
#include "Operation.h"
#include "Parser.h"
#include "Exception.h"

using namespace parser;

void Parser::skipEmpty(std::string::iterator &it, std::string::iterator end) {
    for(; isspace(*it) || *it == ',' && it != end; it++); // скипаем до момента когда можно будет читать
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
    auto word = nextWord(it, end, [](char c){
        return isalpha(c) || c == '`';
    });
    if(word.empty()){
        throw Exception("Empty word in name value near \"" + std::to_string(*it) + "\" symbol ");
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

Token* Parser::nextUserValue(std::string::iterator &it,
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
        return new UserValueToken<bool>(UT_BOOL, word[0] != 'f');
    }

    std::stringstream ss(word);

    auto dotPosition = word.find('.');
    if(dotPosition != std::string::npos){ // float
        auto lastDotPosition = word.rfind('.');

        // значит у нас более одной точки, а значит тут какой то бред
        if(lastDotPosition != dotPosition){
            throw Exception("Wrong float value " + word);
        }

        float val;

        if(!(ss >> val)){
            throw Exception("Couldn't parse float value " + word);
        }

        return new UserValueToken<float>(UT_FLOAT, val);
    }

    // значит число

    // проверяем влезет ли оно в int64_t
    int64_t try1;
    if(ss >> try1){
        return new UserValueToken<int64_t>(UT_NUMBER, try1);
    }

    // если не влезло (или там дрянь), то пробуем в uint64_t
    uint64_t try2;
    if(ss >> try2){
        return new UserValueToken<uint64_t>(UT_UNSIGNED_BIG_NUMBER, try2);
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

    return new UserValueToken<std::string>(UT_STR, word);
}

void Parser::parse(std::string query) {
	auto it = query.begin();

	auto opRaw = nextKeyword(it, query.end());
	if(opRaw == "select"){
		Parser::runSelect(query, it);
	}else if(opRaw == "insert"){
		Parser::runInsert(query, it);
	}else if(opRaw == "update"){
		Parser::runUpdate(query, it);
	}else if(opRaw == "delete"){
		Parser::runDelete(query, it);
	}else if(opRaw == "create"){
		Parser::runCreate(query, it);
	}else if(opRaw == "drop"){
		Parser::runDrop(query, it);
	}else{
		throw Exception("Unknown operation \"" + opRaw + "\"");
	}
}

void Parser::runCreate(std::string &query,
                       std::string::iterator &it) {
    auto end = query.end();
	auto str = nextKeyword(it, end);

	if(str != "table"){
		throw Exception(R"(After "create" keyword you should use "table")");
		// raw string literal юзается потому что иначе тут будет много \"
	}

    auto tableName = nextNameValue(it, end);

	//todo проверять существует ли уже эта таблица

	// читаем структуру таблицы

	skipEmpty(it, end);
	if(*it != '('){
        throw Exception("No \"(\" symbol before table structure");
	}

	if(it++ == end){
	    throw Exception("Unexpected end");
	}

	auto* cols = new list::List<db::Column>();

	//todo сделать db::Column

}

void Parser::runDelete(std::string &query,
                       std::string::iterator &it) {
	auto str = nextKeyword(it, query.end());
	if(str != "from"){
		throw Exception(R"(After "delete" keyword you should use "from")");
		// raw string literal юзается потому что иначе тут будет много \"
	}
}

void Parser::runDrop(std::string &query,
                     std::string::iterator &it) {

}

void Parser::runInsert(std::string &query,
                       std::string::iterator &it) {
	auto str = nextKeyword(it, query.end());
	if(str != "into"){
		throw Exception(R"(After "insert" keyword you should use "into")");
		// raw string literal юзается потому что иначе тут будет много \"
	}
}

void Parser::runSelect(std::string &query,
                       std::string::iterator &it) {

}

void Parser::runUpdate(std::string &query,
                       std::string::iterator &it) {

}
