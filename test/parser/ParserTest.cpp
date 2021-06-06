#include <gtest/gtest.h>
#include <parser/Parser.h>
using namespace parser;

TEST(Parser, nextUserValue){
	std::string query = R"(1 -4 "test")";
	auto it = query.begin();
	auto end = query.end();

	Parser::iterator = it;
	Parser::begin = query.begin();

	auto one = Parser::nextUserValue(end, "number 1");
	EXPECT_EQ(one->toString(), "(number) 1");

	auto negativeFour = Parser::nextUserValue(end, "number -4");
	EXPECT_EQ(negativeFour->toString(), "(number) -4");

	auto testStr = Parser::nextUserValue(end, R"("test" string)");
	EXPECT_EQ(testStr->toString(), "(string) test");

}