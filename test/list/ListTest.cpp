#include <gtest/gtest.h>
#include <list/List.h>

using namespace list;

List<int>* getSomeList(){
	auto l = new List<int>();
	l->push_back(new int(1));
	l->push_back(new int(2));
	l->push_back(new int(3));
	l->push_back(new int(4));
	l->push_back(new int(5));
	l->push_back(new int(6));
	l->push_back(new int(7));
	l->push_back(new int(8));
	l->push_back(new int(9));

	return l;
}

TEST(List, push){
	auto l = getSomeList();

	l->push_back(new int(10));
	auto newElement = *l->at(9);

	EXPECT_EQ(newElement, 10);

	l->push_front(new int(0));
	newElement = *l->at(0);

	EXPECT_EQ(newElement, 0);
}

TEST(List, clear){
	auto l = getSomeList();

	l->clear();

	EXPECT_EQ(l->first, nullptr);
	EXPECT_EQ(l->last, nullptr);
}

TEST(List, set){
	auto l = getSomeList();

	l->set(3, new int(100));

	auto updatedElement = l->at(3);
	EXPECT_EQ(updatedElement, 100);
}