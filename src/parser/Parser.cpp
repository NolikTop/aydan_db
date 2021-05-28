#include "Operation.h"
#include "Parser.h"
#include "Exception.h"

using namespace parser;

std::string Parser::nextKeyword(std::string::iterator &it, std::string::iterator end) {
	std::string res;
	for(; isalpha(*it) && it != end; it++){
		res += (char)std::tolower(*it);
	}

	return res;
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
	auto str = nextKeyword(it, query.end());
	if(str != "table"){
		throw Exception(R"(After "create" keyword you should use "table")");
		// raw string literal юзается потому что иначе тут будет много \"
	}
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
