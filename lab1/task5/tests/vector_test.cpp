#include <gtest/gtest.h>
#include "vector.h"
#include <string>
#include <array>

using namespace my_container;

class VectorTest : public ::testing::Test {
protected:
    vector<int> v;
};

// --- Тесты конструкторов ---
TEST_F(VectorTest, DefaultConstructor) {
    EXPECT_TRUE(v.empty());
    EXPECT_EQ(v.size(), 0);
    EXPECT_EQ(v.capacity(), 0);
}

TEST_F(VectorTest, InitializerListConstructor) {
    vector<int> v1 {1, 2, 3};
    EXPECT_EQ(v1.size(), 3);
    EXPECT_EQ(v1[0], 1);
    EXPECT_EQ(v1[1], 2);
    EXPECT_EQ(v1[2], 3);
}

TEST_F(VectorTest, CopyConstructor) {
    vector<int> v1 {1, 2, 3};
    vector<int> v2(v1);
    EXPECT_EQ(v2.size(), 3);
    EXPECT_EQ(v2, v1);
}

TEST_F(VectorTest, MoveConstructor) {
    vector<int> v1 {1, 2, 3};
    vector<int> v2(std::move(v1));
    EXPECT_EQ(v2.size(), 3);
    EXPECT_EQ(v2[0], 1);
    EXPECT_EQ(v2[1], 2);
    EXPECT_EQ(v2[2], 3);
}

// --- Операторы ---
TEST_F(VectorTest, CopyAssignment) {
    vector<int> v1 {1, 2, 3};
    vector<int> v2;
    v2 = v1;
    EXPECT_EQ(v2, v1);
}

TEST_F(VectorTest, MoveAssignment) {
    vector<int> v1 {1, 2, 3};
    vector<int> v2;
    v2 = std::move(v1);
    EXPECT_EQ(v2.size(), 3);
    EXPECT_EQ(v2[0], 1);
    EXPECT_EQ(v2[1], 2);
    EXPECT_EQ(v2[2], 3);
}

TEST_F(VectorTest, Swap) {
    vector<int> v1 {1, 2, 3};
    vector<int> v2 {4, 5};
    v1.swap(v2);
    EXPECT_EQ(v1.size(), 2);
    EXPECT_EQ(v2.size(), 3);
    EXPECT_EQ(v1[0], 4);
    EXPECT_EQ(v2[0], 1);
}

TEST_F(VectorTest, SubscriptOperator) {
    v.push_back(10);
    EXPECT_EQ(v[0], 10);
    v[0] = 20;
    EXPECT_EQ(v[0], 20);
}

// --- Методы доступа ---
TEST_F(VectorTest, AtOutOfRange) {
    EXPECT_THROW(v.at(0), std::out_of_range);
}

TEST_F(VectorTest, AtInRange) {
    v.push_back(10);
    EXPECT_EQ(v.at(0), 10);
}

TEST_F(VectorTest, ConstAt) {
    const vector<int> const_v {1, 2, 3};
    EXPECT_EQ(const_v.at(1), 2);
    EXPECT_THROW(const_v.at(3), std::out_of_range);
}

TEST_F(VectorTest, FrontBack) {
    v.push_back(10);
    v.push_back(20);
    EXPECT_EQ(v.front(), 10);
    EXPECT_EQ(v.back(), 20);
}

TEST_F(VectorTest, ConstFrontBack) {
    const vector<int> const_v {10, 20};
    EXPECT_EQ(const_v.front(), 10);
    EXPECT_EQ(const_v.back(), 20);
}

TEST_F(VectorTest, Data) {
    for (int i = 0; i < 5; ++i) {
        v.push_back(i + 1);
    }
    int* dataPtr = v.data();
    for (size_t i = 0; i < v.size(); ++i) {
        EXPECT_EQ(dataPtr[i], v[i]);
    }
}

TEST_F(VectorTest, ConstData) {
    const vector<int> const_v {1, 2, 3};
    const int* dataPtr = const_v.data();
    for (size_t i = 0; i < const_v.size(); ++i) {
        EXPECT_EQ(dataPtr[i], const_v[i]);
    }
}

TEST_F(VectorTest, MaxSize) {
    EXPECT_NE(v.max_size(), 0);
}

// --- Методы управления памятью ---
TEST_F(VectorTest, Reserve) {
    v.reserve(100);
    EXPECT_GE(v.capacity(), 100);
}

TEST_F(VectorTest, ShrinkToFit) {
    v.reserve(100);
    v.push_back(1);
    v.push_back(2);
    v.shrink_to_fit();
    EXPECT_EQ(v.capacity(), v.size());
}

// --- Итераторы ---
TEST_F(VectorTest, BeginEnd) {
    v.push_back(1);
    v.push_back(2);
    int sum = 0;
    for (auto it = v.begin(); it != v.end(); ++it) {
        sum += *it;
    }
    EXPECT_EQ(sum, 3);
}

TEST_F(VectorTest, ConstBeginEnd) {
    const vector<int> const_v {1, 2, 3};
    int sum = 0;
    for (auto it = const_v.begin(); it != const_v.end(); ++it) {
        sum += *it;
    }
    EXPECT_EQ(sum, 6);
}

TEST_F(VectorTest, CBeginCEnd) {
    vector<int> v1 {1, 2, 3};
    int sum = 0;
    for (auto it = v1.cbegin(); it != v1.cend(); ++it) {
        sum += *it;
    }
    EXPECT_EQ(sum, 6);
}

TEST_F(VectorTest, RBeginREnd) {
    v.push_back(1);
    v.push_back(2);
    v.push_back(3);
    int sum = 0;
    for (auto it = v.rbegin(); it != v.rend(); ++it) {
        sum += *it;
    }
    EXPECT_EQ(sum, 6);
}

TEST_F(VectorTest, ConstRBeginREnd) {
    const vector<int> const_v {1, 2, 3};
    int sum = 0;
    for (auto it = const_v.rbegin(); it != const_v.rend(); ++it) {
        sum += *it;
    }
    EXPECT_EQ(sum, 6);
}

TEST_F(VectorTest, CRBeginCREnd) {
    vector<int> v1 {1, 2, 3};
    int sum = 0;
    for (auto it = v1.crbegin(); it != v1.crend(); ++it) {
        sum += *it;
    }
    EXPECT_EQ(sum, 6);
}


// --- Методы модификации контейнера ---
TEST_F(VectorTest, PushBack) {
    v.push_back(5);
    EXPECT_EQ(v.size(), 1);
    EXPECT_EQ(v[0], 5);
}

TEST_F(VectorTest, MovePushBack) {
    int val = 5;
    v.push_back(std::move(val));
    EXPECT_EQ(v.size(), 1);
    EXPECT_EQ(v[0], 5);
}

TEST_F(VectorTest, PopBack) {
    v.push_back(10);
    v.pop_back();
    EXPECT_TRUE(v.empty());
}

TEST_F(VectorTest, Insert) {
    v.push_back(1);
    v.push_back(3);
    v.insert(v.begin() + 1, 2);
    EXPECT_EQ(v[1], 2);
}

TEST_F(VectorTest, MoveInsert) {
    v.push_back(1);
    v.push_back(3);
    int val = 2;
    v.insert(v.begin() + 1, std::move(val));
    EXPECT_EQ(v[1], 2);
}

TEST_F(VectorTest, InsertCount) {
    v.push_back(1);
    v.push_back(4);
    v.insert(v.begin() + 1, 2, 2);
    EXPECT_EQ(v.size(), 4);
    EXPECT_EQ(v[1], 2);
    EXPECT_EQ(v[2], 2);
    EXPECT_EQ(v[3], 4);
}

TEST_F(VectorTest, InsertInitializerList) {
    v.push_back(1);
    v.push_back(5);
    v.insert(v.begin() + 1, {2, 3, 4});
    EXPECT_EQ(v.size(), 5);
    EXPECT_EQ(v[1], 2);
    EXPECT_EQ(v[2], 3);
    EXPECT_EQ(v[3], 4);
    EXPECT_EQ(v[4], 5);
}

TEST_F(VectorTest, Erase) {
    v.push_back(1);
    v.push_back(2);
    v.push_back(3);
    v.erase(v.begin() + 1);
    EXPECT_EQ(v.size(), 2);
    EXPECT_EQ(v[1], 3);
}

TEST_F(VectorTest, EraseRange) {
    v.push_back(1);
    v.push_back(2);
    v.push_back(3);
    v.push_back(4);
    v.erase(v.begin() + 1, v.begin() + 3);
    EXPECT_EQ(v.size(), 2);
    EXPECT_EQ(v[0], 1);
    EXPECT_EQ(v[1], 4);
}

TEST_F(VectorTest, Clear) {
    v.push_back(1);
    v.push_back(2);
    v.clear();
    EXPECT_TRUE(v.empty());
    EXPECT_EQ(v.size(), 0);
}

TEST_F(VectorTest, ResizeSmaller) {
    v.push_back(1);
    v.push_back(2);
    v.push_back(3);
    v.resize(2);
    EXPECT_EQ(v.size(), 2);
    EXPECT_EQ(v[0], 1);
    EXPECT_EQ(v[1], 2);
}

TEST_F(VectorTest, ResizeLarger) {
    v.resize(3);
    EXPECT_EQ(v.size(), 3);
}

TEST_F(VectorTest, ResizeLargerWithValue) {
    v.resize(3, 10);
    EXPECT_EQ(v.size(), 3);
    EXPECT_EQ(v[0], 10);
    EXPECT_EQ(v[1], 10);
    EXPECT_EQ(v[2], 10);
}


// --- Сравнение ---
TEST_F(VectorTest, EqualityOperator) {
    vector<int> v1 {1, 2, 3};
    vector<int> v2 {1, 2, 3};
    EXPECT_EQ(v1, v2);
}

TEST_F(VectorTest, InequalityOperator) {
    vector<int> v1 {1, 2, 3};
    vector<int> v2 {1, 2, 4};
    EXPECT_NE(v1, v2);
}

TEST_F(VectorTest, LessThanOperator) {
    vector<int> v1{1, 2};
    vector<int> v2 {1, 2, 3};
    EXPECT_LT(v1, v2);
    EXPECT_LE(v1, v2);
}

TEST_F(VectorTest, GreaterThanOperator) {
    vector<int> v1{1, 2};
    vector<int> v2 {1, 2, 3};
    EXPECT_GT(v2, v1);
    EXPECT_GE(v2, v1);
}