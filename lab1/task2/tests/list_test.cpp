#include <gtest/gtest.h>
#include "../include/List.h"
#include <stdexcept>
#include <algorithm>
#include <limits>
#include <string>
#include <vector>
#include <numeric>

using namespace my_container;

class ListTest : public ::testing::Test {
protected:
    List<int> list;
};

TEST_F(ListTest, DefaultConstructor) {
    EXPECT_TRUE(list.empty());
    EXPECT_EQ(list.size(), 0);
}

TEST_F(ListTest, InitializerListConstructor) {
    List<int> l {1, 2, 3};
    EXPECT_EQ(l.size(), 3);
    EXPECT_EQ(l.front(), 1);
    EXPECT_EQ(l.back(), 3);
}

TEST_F(ListTest, CopyConstructor) {
    List<int> l1 {1, 2, 3};
    List<int> l2(l1);
    EXPECT_EQ(l1, l2);
}

TEST_F(ListTest, MoveConstructor) {
    List<int> l1 {1, 2, 3};
    List<int> l2(std::move(l1));
    EXPECT_EQ(l2.size(), 3);

}

TEST_F(ListTest, CopyAssignmentOperator) {
    List<int> l1 {1, 2, 3};
    List<int> l2;
    l2 = l1;
    EXPECT_EQ(l1, l2);
}

TEST_F(ListTest, MoveAssignmentOperator) {
    List<int> l1 {1, 2, 3};
    List<int> l2;
    l2 = std::move(l1);
    EXPECT_EQ(l2.size(), 3);

}

TEST_F(ListTest, PushBack) {
    list.push_back(5);
    EXPECT_EQ(list.size(), 1);
    EXPECT_EQ(list.back(), 5);
}

TEST_F(ListTest, PushFront) {
    list.push_front(5);
    EXPECT_EQ(list.size(), 1);
    EXPECT_EQ(list.front(), 5);
}

TEST_F(ListTest, PopBack) {
    list.push_back(1);
    list.pop_back();
    EXPECT_TRUE(list.empty());
}

TEST_F(ListTest, PopFront) {
    list.push_front(1);
    list.pop_front();
    EXPECT_TRUE(list.empty());
}

TEST_F(ListTest, InsertErase) {
    list.push_back(1);
    list.push_back(3);
    auto it = list.insert(++list.begin(), 2);
    EXPECT_EQ(*it, 2);
    EXPECT_EQ(list.size(), 3);
    list.erase(it);
    EXPECT_EQ(list.size(), 2);
}

TEST_F(ListTest, ComparisonOperators) {
    List<int> l1 {1, 2, 3};
    List<int> l2 {1, 2, 3};
    List<int> l3 {4, 5, 6};
    EXPECT_EQ(l1, l2);
    EXPECT_NE(l1, l3);
    EXPECT_LT(l1, l3);
    EXPECT_LE(l1, l2);
    EXPECT_GT(l3, l1);
    EXPECT_GE(l3, l3);
}

TEST_F(ListTest, Swap) {
    List<int> l1 {1, 2, 3};
    List<int> l2 {4, 5};
    l1.swap(l2);
    EXPECT_EQ(l1.size(), 2);
    EXPECT_EQ(l2.size(), 3);
}

TEST_F(ListTest, Iterators) {
    list.push_back(1);
    list.push_back(2);
    list.push_back(3);
    auto it = list.begin();
    EXPECT_EQ(*it, 1);
    ++it;
    EXPECT_EQ(*it, 2);
    --it;
    EXPECT_EQ(*it, 1);
}
