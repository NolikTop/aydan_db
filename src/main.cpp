#include <iostream>
#include <string>
#include <parser/Parser.h>
#include <binary/Exception.h>
#include <db/Exception.h>
#include <list/Exception.h>
#include <parser/Exception.h>
#include <exception>

using namespace std;

#define RESET   "\033[0m"
#define BLACK   "\033[30m"      /* Black */
#define RED     "\033[31m"      /* Red */
#define GREEN   "\033[32m"      /* Green */
#define YELLOW  "\033[33m"      /* Yellow */
#define BLUE    "\033[34m"      /* Blue */
#define MAGENTA "\033[35m"      /* Magenta */
#define CYAN    "\033[36m"      /* Cyan */
#define WHITE   "\033[37m"      /* White */
#define BOLDBLACK   "\033[1m\033[30m"      /* Bold Black */
#define BOLDRED     "\033[1m\033[31m"      /* Bold Red */
#define BOLDGREEN   "\033[1m\033[32m"      /* Bold Green */
#define BOLDYELLOW  "\033[1m\033[33m"      /* Bold Yellow */
#define BOLDBLUE    "\033[1m\033[34m"      /* Bold Blue */
#define BOLDMAGENTA "\033[1m\033[35m"      /* Bold Magenta */
#define BOLDCYAN    "\033[1m\033[36m"      /* Bold Cyan */
#define BOLDWHITE   "\033[1m\033[37m"      /* Bold White */

#define AYDANDB_CATCH_EXCEPTIONS 1

int main(){
	cout << "Welcome to the Aydan_db" << endl;
	cout << "Type 'exit' to exit the program" << endl;
	string command;
	do {
		if(!command.empty()){
#if AYDANDB_CATCH_EXCEPTIONS
			try {
#endif
				cout << parser::Parser::parse(command);
#if AYDANDB_CATCH_EXCEPTIONS
			}catch(binary::Exception &e){
				cout << RED << "binary::Exception: " << e.what();
			}catch(db::Exception &e){
				cout << RED << "db::Exception: " << e.what();
			}catch(list::Exception &e){
				cout << RED << "list::Exception: " << e.what();
			}catch(parser::Exception &e){
				cout << RED << "parser::Exception: " << e.what();
			}catch(std::exception &e){
				cout << RED << "std::Exception: " << e.what();
			}
			cout << RESET << endl;
#endif
		}
		cout << "aydan_db> ";
		getline(cin, command);
	}while(command != "exit");
}