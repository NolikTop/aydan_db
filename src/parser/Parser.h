
#pragma once

#include <string>
#include "Token.h"
#include "Operation.h"
#include "UserValueToken.h"
#include <list/List.h>
#include <parser/type/TypeToken.h>

namespace parser {

	class Parser {
	public:
		static list::List<parser::type::TypeToken>* types;

		static void initTypes();

	    static void skipEmpty(std::string::iterator &it, std::string::iterator end, bool skipComma = true);

	    static std::string nextWord(std::string::iterator &it, std::string::iterator end, const std::function<bool(char)>& check = nullptr);
		static std::string nextWord(std::string::iterator &it, std::string::iterator end, int (*check)(int) = nullptr);
		static std::string nextKeyword(std::string::iterator &it, std::string::iterator end);
		static std::string nextNameValue(std::string::iterator &it, std::string::iterator end);
		static UserValueBaseToken* nextUserValue(std::string::iterator &it, std::string::iterator end);
        static UserValueToken<std::string>* nextStringVal(std::string::iterator &it, std::string::iterator end);
		static db::Column* readColumn(std::string::iterator &it, std::string::iterator end);

		static void parse(std::string query);

		static void runCreate(std::string &query, std::string::iterator &it);

		static void runDelete(std::string &query, std::string::iterator &it);
		static void runDrop(std::string &query, std::string::iterator &it);
		static void runInsert(std::string &query, std::string::iterator &it);
		static void runSelect(std::string &query, std::string::iterator &it);
		static void runUpdate(std::string &query, std::string::iterator &it);

	};

}
