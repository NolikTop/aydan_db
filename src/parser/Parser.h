
#pragma once

#include <string>
#include "Token.h"
#include "Operation.h"
#include "UserValueToken.h"
#include <list/List.h>
#include <db/Column.h>
#include <parser/type/TypeToken.h>

namespace parser {

	class Parser {
	public:
		static list::List<parser::type::TypeToken>* types;
		static std::string::iterator iterator;
		static std::string::iterator begin;

		static void initTypes();

		static void skipEmpty(std::string::iterator end, std::string expected, bool skipComma = true, bool skipCloseParenthesis = true);

	    static std::string nextWord(std::string::iterator end, const std::function<bool(char)>& check = nullptr, std::string expected = "");
		static std::string nextWord(std::string::iterator end, int (*check)(int) = nullptr, std::string expected = "");
		static std::string nextKeyword(std::string::iterator end, std::string expected);
		static std::string nextNameValue(std::string::iterator end, std::string expected);
		static UserValueBaseToken* nextUserValue(std::string::iterator end, std::string expected);
        static UserValueToken<std::string>* nextStringVal(std::string::iterator end, std::string expected);
		static db::Column* readColumn(std::string::iterator end);

		static std::string parse(std::string query);

		static std::string runCreate(std::string &query);

		static std::string runDelete(std::string &query);
		static std::string runDrop(std::string &query);
		static std::string runInsert(std::string &query);
		static std::string runSelect(std::string &query);
		static std::string runUpdate(std::string &query);
		static std::string runDescribe(std::string &query);

	};

}
