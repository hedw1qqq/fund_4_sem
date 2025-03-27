#include <gtest/gtest.h>

#include <stdexcept>
#include <vector>

#include "../include/deque.h"

using namespace my_container;

class DequeTest : public ::testing::Test {
   protected:
	Deque<int> d{1,2,3,4,5};

};

TEST_F(DequeTest, DefaultConstructor) {
	Deque<int> localDeque;
	EXPECT_TRUE(localDeque.empty());
	EXPECT_EQ(localDeque.size(), 0u);
}

TEST(DequeConstructorTest, InitializerListConstructor) {
	Deque<int> dlist({10, 20, 30});
	EXPECT_EQ(dlist.size(), 3u);
	EXPECT_EQ(dlist.front(), 10);
	EXPECT_EQ(dlist.back(), 30);
	EXPECT_EQ(dlist[1], 20);
}

TEST_F(DequeTest, CopyConstructor) {
	Deque<int> copyD(d);
	EXPECT_EQ(copyD.size(), d.size());
	auto it1 = d.begin();
	auto it2 = copyD.begin();
	while (it1 != d.end() && it2 != copyD.end()) {
		EXPECT_EQ(*it1, *it2);
		++it1;
		++it2;
	}
}

TEST_F(DequeTest, MoveConstructor) {
	Deque<int> temp = d;
	Deque<int> movedDeque(std::move(temp));
	EXPECT_EQ(movedDeque.size(), d.size());
}

TEST_F(DequeTest, CopyAssignmentOperator) {
	Deque<int> copyD;
	copyD = d;
	EXPECT_EQ(copyD.size(), d.size());
	auto it1 = d.begin();
	auto it2 = copyD.begin();
	while (it1 != d.end() && it2 != copyD.end()) {
		EXPECT_EQ(*it1, *it2);
		++it1;
		++it2;
	}
}

TEST_F(DequeTest, MoveAssignmentOperator) {
	Deque<int> temp = d;
	Deque<int> movedDeque;
	movedDeque = std::move(temp);
	EXPECT_EQ(movedDeque.size(), d.size());

}

TEST_F(DequeTest, SwapMethod) {
	Deque<int> other({100, 200});
	size_t originalSize = d.size();
	d.swap(other);
	EXPECT_EQ(d.size(), 2u);
	EXPECT_EQ(other.size(), originalSize);
	EXPECT_EQ(d.front(), 100);
	EXPECT_EQ(other.front(), 1);
}

TEST_F(DequeTest, AccessMethods) {
	EXPECT_EQ(d.front(), 1);
	EXPECT_EQ(d.back(), 5);
	EXPECT_EQ(d.at(2), 3);
	EXPECT_EQ(d[2], 3);
	EXPECT_THROW(d.at(10), std::out_of_range);
}

TEST_F(DequeTest, IteratorTraversal) {
	std::vector<int> vec;
	for (auto it = d.begin(); it != d.end(); ++it) {
		vec.push_back(*it);
	}
	std::vector<int> expected = {1, 2, 3, 4, 5};
	EXPECT_EQ(vec, expected);
}

TEST_F(DequeTest, ReverseIteratorTraversal) {
	std::vector<int> vec;
	for (auto it = d.rbegin(); it != d.rend(); ++it) {
		vec.push_back(*it);
	}
	std::vector<int> expected = {5, 4, 3, 2, 1};
	EXPECT_EQ(vec, expected);
}

TEST_F(DequeTest, SizeAndEmpty) {
	Deque<int> localDeque;
	EXPECT_TRUE(localDeque.empty());
	localDeque.push_back(42);
	EXPECT_FALSE(localDeque.empty());
	EXPECT_EQ(localDeque.size(), 1u);
}

TEST_F(DequeTest, PushAndPopOperations) {
	Deque<int> localDeque;
	localDeque.push_front(10);
	EXPECT_EQ(localDeque.front(), 10);
	localDeque.push_back(20);
	EXPECT_EQ(localDeque.back(), 20);
	localDeque.pop_front();
	EXPECT_EQ(localDeque.front(), 20);
	localDeque.push_front(30);
	localDeque.pop_back();
	EXPECT_EQ(localDeque.back(), 30);
}

TEST_F(DequeTest, InsertAndErase) {
	auto it = d.begin();
	std::advance(it, 2);
	it = d.insert(it, 99);
	EXPECT_EQ(*it, 99);
	EXPECT_EQ(d.size(), 6u);
	auto eraseIt = d.erase(it);
	EXPECT_EQ(d.size(), 5u);
	EXPECT_EQ(*eraseIt, 3);
}

TEST_F(DequeTest, ClearMethod) {
	d.clear();
	EXPECT_TRUE(d.empty());
	EXPECT_EQ(d.size(), 0u);
}

TEST_F(DequeTest, ResizeMethod) {
	d.resize(7, 100);
	EXPECT_EQ(d.size(), 7u);
	EXPECT_EQ(d.back(), 100);
	d.resize(3);
	EXPECT_EQ(d.size(), 3u);
	EXPECT_EQ(d.back(), 3);
}

TEST_F(DequeTest, ComparisonOperators) {
	Deque<int> a({1, 2, 3});
	Deque<int> b({1, 2, 3});
	Deque<int> c({1, 2, 4});
	EXPECT_EQ(a, b);
	EXPECT_NE(a, c);
	EXPECT_LT(a, c);
	EXPECT_LE(a, b);
	EXPECT_GT(c, a);
	EXPECT_GE(b, a);
}

int main(int argc, char **argv) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
