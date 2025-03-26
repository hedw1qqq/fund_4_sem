#include <gtest/gtest.h>
#include "../include/List.h"
#include <stdexcept>
#include <algorithm>
#include <limits>
#include <string>
#include <vector>
#include <numeric> // Для std::accumulate

using my_container::List;

class ListTest : public ::testing::Test {
protected:
    List<int> listInt;
    List<std::string> listStr;
};

TEST_F(ListTest, ConstructorAndBasicProps) {
    EXPECT_TRUE(listInt.empty());
    EXPECT_EQ(listInt.size(), 0u);
    EXPECT_GT(listInt.max_size(), 0u);
    EXPECT_EQ(listInt.begin(), listInt.end());
    EXPECT_THROW(listInt.front(), std::out_of_range);
    EXPECT_THROW(listInt.back(), std::out_of_range);
    List<int> ilist({1, 2, 3});
    EXPECT_EQ(ilist.size(), 3u);
    EXPECT_EQ(ilist.front(), 1);
    EXPECT_EQ(ilist.back(), 3);
    const List<int>& cilist = ilist;
    EXPECT_EQ(*cilist.begin(), 1); // Test begin() const
    EXPECT_NE(cilist.begin(), cilist.end()); // Test end() const
}

TEST_F(ListTest, PushAndPop) {
    listInt.push_back(10);
    listInt.push_front(5);
    listInt.push_back(20);
    EXPECT_EQ(listInt.size(), 3u);
    EXPECT_EQ(listInt.front(), 5);
    EXPECT_EQ(listInt.back(), 20);
    listInt.pop_front();
    EXPECT_EQ(listInt.front(), 10);
    listInt.pop_back();
    EXPECT_EQ(listInt.back(), 10);
    listInt.pop_back();
    EXPECT_TRUE(listInt.empty());
    EXPECT_THROW(listInt.pop_front(), std::out_of_range);
    EXPECT_THROW(listInt.pop_back(), std::out_of_range);
}

TEST_F(ListTest, RvaluePush) {
    std::string s1 = "hello";
    std::string s2 = "world";
    std::string s3 = "start";
    listStr.push_back(std::move(s1));
    listStr.push_front(std::move(s3));
    listStr.push_back(std::move(s2)); // {"start", "hello", "world"}
    EXPECT_EQ(listStr.size(), 3u);
    EXPECT_EQ(listStr.front(), "start");
    EXPECT_EQ(listStr.back(), "world");
}


TEST_F(ListTest, CopyAndMoveCtor) {
    List<int> source {1, 2, 3};
    List<int> copy(source);
    EXPECT_EQ(copy.size(), 3u);
    EXPECT_EQ(source.size(), 3u);
    List<int> moved(std::move(source));
    EXPECT_EQ(moved.size(), 3u);
}

TEST_F(ListTest, CopyAndMoveAssign) {
    List<int> source {10, 20};
    List<int> copyAssign;
    copyAssign.push_back(1);
    copyAssign = source;
    EXPECT_EQ(copyAssign.size(), 2u);
    EXPECT_EQ(source.size(), 2u);
    List<int> moveAssign;
    moveAssign.push_back(2);
    moveAssign = std::move(source);
    EXPECT_EQ(moveAssign.size(), 2u);
    copyAssign = copyAssign;
    EXPECT_EQ(copyAssign.size(), 2u);
}

TEST_F(ListTest, ClearAndSwap) {
    List<int> a {1, 2};
    List<int> b {3, 4, 5};
    a.clear();
    EXPECT_TRUE(a.empty());
    a.swap(b);
    EXPECT_EQ(a.size(), 3u);
    EXPECT_TRUE(b.empty());
}

TEST_F(ListTest, Resize) {
    listInt.resize(3, 7);
    EXPECT_EQ(listInt.size(), 3u);
    listInt.resize(1);
    EXPECT_EQ(listInt.size(), 1u);
    EXPECT_EQ(listInt.front(), 7);
    listInt.resize(2);
    EXPECT_EQ(listInt.back(), 0);
    listInt.resize(0);
    EXPECT_TRUE(listInt.empty());
}

TEST_F(ListTest, IteratorsAndAccumulate) {
    List<int> l {10, 20, 30};
    auto it = l.begin();
    *it = 15;
    ++it;
    EXPECT_EQ(*it, 20);
    const List<int>& cl = l;
    EXPECT_EQ(*cl.cbegin(), 15);
    int sum = std::accumulate(l.cbegin(), l.cend(), 0);
    EXPECT_EQ(sum, 15 + 20 + 30);
    std::vector<int> reversed;
    for(auto rit = l.crbegin(); rit != l.crend(); ++rit) { reversed.push_back(*rit); }
    EXPECT_EQ(reversed.size(), 3u);
    EXPECT_EQ(reversed[0], 30);
}

TEST_F(ListTest, Insert) {
    listInt.insert(listInt.begin(), 10);
    auto it = listInt.begin();
    listInt.insert(++it, 30);
    listInt.insert(listInt.begin(), 5);
    EXPECT_EQ(listInt.size(), 3u);
    int temp = 20;
    auto inserted_it = listInt.insert(std::next(listInt.begin(), 2), temp);
    EXPECT_EQ(*inserted_it, 20);
    std::string s = "world";
    listStr.insert(listStr.end(), "hello");
    listStr.insert(listStr.end(), std::move(s)); // Test rvalue insert
    EXPECT_EQ(listStr.back(), "world");
}

TEST_F(ListTest, EraseAndConstErase) {
    List<int> l {10, 20, 30, 40, 50};
    auto it = l.erase(l.begin()); // erase(iterator)
    EXPECT_EQ(*it, 20);
    auto cit_erase = l.cbegin(); // const_iterator
    ++cit_erase; // points to 30
    it = l.erase(cit_erase); // erase(const_iterator)
    EXPECT_EQ(*it, 40); // {20, 40, 50}
    EXPECT_EQ(l.size(), 3u);
    auto cit_first = l.cbegin();
    auto cit_last = l.cend();
    it = l.erase(cit_first, cit_last); // erase(const_iterator, const_iterator)
    EXPECT_TRUE(l.empty());
    EXPECT_EQ(it, l.end());
    EXPECT_THROW(l.erase(l.cbegin()), std::out_of_range);
}

TEST_F(ListTest, Comparison) {
    List<int> a {1, 2};
    List<int> b {1, 2};
    List<int> c {1, 3};
    EXPECT_TRUE(a == b);
    EXPECT_TRUE(a != c);
    EXPECT_TRUE(a < c);
    EXPECT_TRUE(c > a);
    EXPECT_TRUE(a <= b);
    EXPECT_TRUE(a >= b);
    Container<int>* pA = &a;
    Container<int>* pB = &b;
    EXPECT_TRUE(*pA == *pB); // Base comparison
}
TEST_F(ListTest, InsertEdgeCases) {
	listInt.insert(listInt.end(), 100);
	EXPECT_EQ(listInt.size(), 1u);
	EXPECT_EQ(listInt.front(), 100);

	listInt.insert(listInt.begin(), 200);
	EXPECT_EQ(listInt.size(), 2u);
	EXPECT_EQ(listInt.front(), 200);
	EXPECT_EQ(listInt.back(), 100);

	listInt.insert(listInt.begin(), {1, 2, 3});
	EXPECT_EQ(listInt.size(), 5u);
	EXPECT_EQ(listInt.front(), 1);
}

TEST_F(ListTest, EraseEdgeCases) {
	List<int> l {1, 2, 3, 4, 5};
	auto it = l.erase(l.begin(), l.end());
	EXPECT_TRUE(l.empty());
	EXPECT_EQ(it, l.end());

	EXPECT_THROW(l.erase(l.begin()), std::out_of_range);
}

TEST_F(ListTest, ReverseIterator) {
	List<int> l {1, 2, 3, 4};
	auto rit = l.rbegin();
	EXPECT_EQ(*rit, 4);
	++rit;
	EXPECT_EQ(*rit, 3);
	++rit;
	EXPECT_EQ(*rit, 2);
}

TEST_F(ListTest, SwapWithSelf) {
	List<int> l {10, 20, 30};
	l.swap(l);
	EXPECT_EQ(l.size(), 3u);
	EXPECT_EQ(l.front(), 10);
	EXPECT_EQ(l.back(), 30);
}
int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}