#include <gtest/gtest.h>
#include "../include/vector.h"

using namespace my_container;

class VectorTest : public ::testing::Test {
protected:
    Vector<int> v;
};

TEST_F(VectorTest, ConstructorsAndAssignment) {
    Vector<int> v1{1, 2, 3};
    Vector<int> v2(v1);
    Vector<int> v3(std::move(v1));
    v = v2;
    EXPECT_EQ(v, v2);
    v = std::move(v3);
    EXPECT_EQ(v.size(), 3);
    Vector<int> v4;
    EXPECT_TRUE(v4.empty());
}

TEST_F(VectorTest, Modifiers) {
    v.insert(v.begin(), {1, 2, 3, 4});
    v.push_back(5);
    v.pop_back();
    v.insert(v.begin() + 2, 99);
    v.erase(v.begin() + 1);
    v.resize(10, 8);
    v.clear();
    EXPECT_TRUE(v.empty());
    v.shrink_to_fit();
}

TEST_F(VectorTest, CapacityAndAccess) {
    v.resize(5, 7);
    v.reserve(50);
    v.shrink_to_fit();
    EXPECT_EQ(v.at(3), 7);
    EXPECT_THROW(v.at(10), std::out_of_range);
    EXPECT_EQ(v.front(), v.back());
    EXPECT_GE(v.capacity(), 5);

}

TEST_F(VectorTest, SwapAndComparison) {
    Vector<int> v2{10, 20};
    v.swap(v2);
    EXPECT_EQ(v.size(), 2);
    EXPECT_NE(v, v2);
    v2.push_back(30);
    EXPECT_LT(v, v2);
    Vector<int> v3{10, 20};
    EXPECT_EQ(v, v3);
    v3.swap(v2);
    EXPECT_NE(v3, v2);
}

TEST_F(VectorTest, IteratorFunctions) {
    v.insert(v.begin(), {1, 2, 3, 4, 5});
    EXPECT_EQ(*v.begin(), 1);
    EXPECT_EQ(*(v.end() - 1), 5);
    EXPECT_EQ(*v.rbegin(), 5);
    EXPECT_EQ(*(v.rend() - 1), 1);
    EXPECT_EQ(v.cbegin(), v.begin());
    EXPECT_EQ(v.cend(), v.end());
}

TEST_F(VectorTest, EdgeCases) {
    v.resize(1, 42);
    v.pop_back();
    EXPECT_TRUE(v.empty());
    v.reserve(0);
    EXPECT_EQ(v.capacity(), 1);
    Vector<int> v2;
    v2.resize(100, 5);
    v2.erase(v2.begin(), v2.begin() + 50);
    EXPECT_EQ(v2.size(), 50);
    v2.insert(v2.begin(), {10, 20, 30});
    EXPECT_EQ(v2.front(), 10);
    EXPECT_EQ(v2.size(), 53);
    v2.clear();
    EXPECT_TRUE(v2.empty());
    v2.shrink_to_fit();
}

TEST_F(VectorTest, LargeScaleOperations) {
    Vector<int> v_large;
    v_large.resize(1000, 1);
    EXPECT_EQ(v_large.size(), 1000);
    v_large.resize(500);
    EXPECT_EQ(v_large.size(), 500);
    v_large.reserve(2000);
    EXPECT_GE(v_large.capacity(), 2000);
    v_large.shrink_to_fit();
}

TEST_F(VectorTest, AdvancedOperations) {
    v.insert(v.begin(), {1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
    auto it = v.erase(v.begin() + 5);
    EXPECT_EQ(*it, 7);
    v.insert(v.begin() + 2, {11, 12, 13});
    EXPECT_EQ(v.at(2), 11);
    v.resize(15, 99);
    EXPECT_EQ(v.at(14), 99);
    v.shrink_to_fit();
}

TEST_F(VectorTest, ExtremeCases) {
    Vector<int> v_empty;
    EXPECT_EQ(v_empty.size(), 0);
    v_empty.reserve(1000);
    EXPECT_GE(v_empty.capacity(), 1000);
    v_empty.resize(500);
    EXPECT_EQ(v_empty.size(), 500);
    v_empty.clear();
    EXPECT_TRUE(v_empty.empty());
    v_empty.shrink_to_fit();
}

TEST_F(VectorTest, StressTest) {
    Vector<int> v_stress;
    v_stress.resize(1000000, 42);
    EXPECT_EQ(v_stress.size(), 1000000);
    v_stress.resize(500000);
    EXPECT_EQ(v_stress.size(), 500000);
    v_stress.clear();
    EXPECT_TRUE(v_stress.empty());
    v_stress.shrink_to_fit();
}

TEST_F(VectorTest, DataFunction) {
	v.insert(v.begin(), {5, 10, 15});
	int* ptr = v.data();
	EXPECT_EQ(ptr[0], 5);
	ptr[1] = 20;
	EXPECT_EQ(v[1], 20);
}

TEST_F(VectorTest, ArrayIndexOperator) {
	v.insert(v.begin(), {100, 200, 300});
	EXPECT_EQ(v[0], 100);
	v[0] = 150;
	EXPECT_EQ(v.front(), 150);
	EXPECT_EQ(v[2], 300);
}

TEST_F(VectorTest, MaxSizeFunction) {
	// Проверяем, что max_size() возвращает значение, отличное от 0
	EXPECT_GT(v.max_size(), 0);
}

TEST_F(VectorTest, ExplicitComparisonOperators) {
	Vector<int> v1{1, 2, 3};
	Vector<int> v2{1, 2, 4};
	EXPECT_NE(v1, v2);
	EXPECT_LT(v1, v2);
	EXPECT_LE(v1, v2);
	EXPECT_GT(v2, v1);
	EXPECT_GE(v2, v1);
}

TEST_F(VectorTest, RvalueInsertAndPushBack) {
	int a = 50;
	v.push_back(std::move(a));
	EXPECT_EQ(v.back(), 50);
	v.insert(v.begin(), std::move(60));
	EXPECT_EQ(v.front(), 60);
}
