#include <iostream>
#include <string>
#include <parser/Parser.h>
#include <binary/Exception.h>
#include <db/Exception.h>
#include <list/Exception.h>
#include <parser/Exception.h>
#include <db/FS.h>
#include <exception>
#include <utils/Colors.h>
#include <sstream>

#ifdef __APPLE__
#include <Availability.h> // for deployment target to support pre-catalina targets without std::fs
#endif
#if ((defined(_MSVC_LANG) && _MSVC_LANG >= 201703L) || (defined(__cplusplus) && __cplusplus >= 201703L)) && defined(__has_include)
#if __has_include(<filesystem>) && (!defined(__MAC_OS_X_VERSION_MIN_REQUIRED) || __MAC_OS_X_VERSION_MIN_REQUIRED >= 101500)
#define GHC_USE_STD_FS
#include <filesystem>
namespace fs = std::filesystem;
#endif
#endif
#ifndef GHC_USE_STD_FS
#include <ghc/filesystem.hpp>
#include <sstream>

namespace fs = ghc::filesystem;
#endif

#define AYDANDB_CATCH_EXCEPTIONS 1

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
#define WINDOWS 1
#else
#define WINDOWS 0
#endif

bool dir_exists(){
	return fs::exists(db::dbPath);
}

int main(){
	std::cout << "Welcome to the " << BOLDMAGENTA << "Aydan_db" << RESET << std::endl;
#if WINDOWS
	if(db::dbPath.find('/') != std::string::npos){
		std::cout << RED << "Please change database path in src/db/FS.h. Is must not contain \"/\" symbols (because of Windows)";
		std::cout << std::endl << YELLOW << "Current path is \"" << db::dbPath << "\"";
		return 0;
	}
#endif

	if(dir_exists()){
		std::cout << "Using " << GREEN << "\"" << db::dbPath << "\"" << RESET << " as " << CYAN << "database" << RESET << " directory" << std::endl;
		std::cout << "You can change it in " << GREEN << "src/db/FS.h" << RESET << std::endl;
	}else{
		std::cout << RED << "Can't find dir \"" << db::dbPath << "\" (it must be used as database directory)" << std::endl;
		std::cout << "You can change it in src/db/FS.h" << std::endl;
		return 0;
	}

	std::cout << "Type " << GREEN << "'exit'" << RESET << " to exit the program" << std::endl;
	std::string command;
	std::cout << BOLDMAGENTA << "aydan_db" << YELLOW << "> " << RESET;
	do {
		std::stringstream error;
		if(!command.empty()){
#if AYDANDB_CATCH_EXCEPTIONS
			try {
#endif
				std::cout << parser::Parser::parse(command);
#if AYDANDB_CATCH_EXCEPTIONS
			}catch(binary::Exception &e){
				error << RED << "binary::Exception: " << RESET << e.what();
			}catch(db::Exception &e){
				error << RED << "db::Exception: " << RESET << e.what();
			}catch(list::Exception &e){
				error << RED << "list::Exception: " << RESET << e.what();
			}catch(parser::Exception &e) {
				error << RED << "parser::Exception: " << RESET << e.what();
			}catch(fs::filesystem_error &e) {
				error << RED << "fs::filesystem_error: " << RESET << e.what();
			}catch(std::exception &e){
				error << RED << "std::Exception: " << RESET << e.what();
			}
#endif
			if(!error.str().empty()){ // eof() адекватно не работает
				std::cout << command << std::endl;
				auto errIndex = distance(parser::Parser::begin, parser::Parser::iterator);
				std::cout << std::string(errIndex, ' ') << BOLDCYAN << '^' << RESET << std::endl;
				std::cout << error.str();
			}
			std::cout << std::endl;
			std::cout << BOLDMAGENTA << "aydan_db" << YELLOW << "> " << RESET;
		}
		getline(std::cin, command);
	}while(command != "exit");
}