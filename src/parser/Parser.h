
#pragma once

#include <string>
#include "Token.h"
#include "Operation.h"
#include <list/List.h>

namespace parser {

	class Parser {
	public:

		static std::string nextKeyword(std::string::iterator &it, std::string::iterator end);
		static void parse(std::string query);

		static void runCreate(std::string &query, std::string::iterator &it);
		static void runDelete(std::string &query, std::string::iterator &it);
		static void runDrop(std::string &query, std::string::iterator &it);
		static void runInsert(std::string &query, std::string::iterator &it);
		static void runSelect(std::string &query, std::string::iterator &it);
		static void runUpdate(std::string &query, std::string::iterator &it);

	};

}
