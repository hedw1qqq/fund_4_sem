#include <gtest/gtest.h>
#include "Vector.h"
#include <string>

using namespace my_container;

class VectorTest : public ::testing::Test {
protected:
    Vector<int> emptyVector;
    Vector<int> singleElementVector{5};
    Vector<int> multiElementVector{1, 2, 3, 4, 5};
    Vector<std::string> stringVector{"hello", "world"};
};

// Constructor Tests
TEST_F(VectorTest, DefaultConstructor) {
    EXPECT_EQ(0, emptyVector.size());
    EXPECT_TRUE(emptyVector.empty());
    EXPECT_EQ(0, emptyVector.capacity());
}

TEST_F(VectorTest, InitializerListConstructor) {
    EXPECT_EQ(5, multiElementVector.size());
    EXPECT_FALSE(multiElementVector.empty());
    EXPECT_EQ(5, multiElementVector.capacity());

    EXPECT_EQ(1, multiElementVector[0]);
    EXPECT_EQ(5, multiElementVector[4]);
}

TEST_F(VectorTest, CopyConstructor) {
    Vector<int> copy(multiElementVector);

    EXPECT_EQ(multiElementVector.size(), copy.size());
    EXPECT_EQ(multiElementVector.capacity(), copy.capacity());

    for (size_t i = 0; i < multiElementVector.size(); ++i) {
        EXPECT_EQ(multiElementVector[i], copy[i]);
    }

    // Verify deep copy
    copy[0] = 100;
    EXPECT_NE(copy[0], multiElementVector[0]);
}

TEST_F(VectorTest, MoveConstructor) {
    Vector<int> original{1, 2, 3, 4, 5};
    Vector<int> moved(std::move(original));

    EXPECT_EQ(5, moved.size());
    EXPECT_EQ(0, original.size());
    EXPECT_EQ(0, original.capacity());
    EXPECT_EQ(1, moved[0]);
    EXPECT_EQ(5, moved[4]);
}

// Assignment Operator Tests
TEST_F(VectorTest, CopyAssignmentOperator) {
    Vector<int> copy;
    copy = multiElementVector;

    EXPECT_EQ(multiElementVector.size(), copy.size());

    for (size_t i = 0; i < multiElementVector.size(); ++i) {
        EXPECT_EQ(multiElementVector[i], copy[i]);
    }

    // Self-assignment
    multiElementVector = multiElementVector;
    EXPECT_EQ(5, multiElementVector.size());
    EXPECT_EQ(1, multiElementVector[0]);
}

TEST_F(VectorTest, MoveAssignmentOperator) {
    Vector<int> original{1, 2, 3, 4, 5};
    Vector<int> moved;
    moved = std::move(original);

    EXPECT_EQ(5, moved.size());
    EXPECT_EQ(0, original.size());
    EXPECT_EQ(1, moved[0]);
    EXPECT_EQ(5, moved[4]);
}

// Element Access Tests
TEST_F(VectorTest, OperatorBrackets) {
    EXPECT_EQ(1, multiElementVector[0]);
    EXPECT_EQ(5, multiElementVector[4]);

    multiElementVector[0] = 10;
    EXPECT_EQ(10, multiElementVector[0]);
}

TEST_F(VectorTest, AtMethod) {
    EXPECT_EQ(1, multiElementVector.at(0));
    EXPECT_EQ(5, multiElementVector.at(4));

    multiElementVector.at(0) = 10;
    EXPECT_EQ(10, multiElementVector.at(0));

    EXPECT_THROW(multiElementVector.at(5), std::out_of_range);
}

TEST_F(VectorTest, FrontAndBack) {
    EXPECT_EQ(1, multiElementVector.front());
    EXPECT_EQ(5, multiElementVector.back());

    multiElementVector.front() = 10;
    multiElementVector.back() = 50;

    EXPECT_EQ(10, multiElementVector.front());
    EXPECT_EQ(50, multiElementVector.back());

    EXPECT_THROW(emptyVector.front(), std::out_of_range);
    EXPECT_THROW(emptyVector.back(), std::out_of_range);
}

TEST_F(VectorTest, Data) {
    EXPECT_NE(nullptr, multiElementVector.data());
    EXPECT_EQ(nullptr, emptyVector.data());

    int* data = multiElementVector.data();
    EXPECT_EQ(1, data[0]);
    EXPECT_EQ(5, data[4]);
}

// Capacity Tests
TEST_F(VectorTest, EmptySizeCapacity) {
    EXPECT_TRUE(emptyVector.empty());
    EXPECT_FALSE(multiElementVector.empty());

    EXPECT_EQ(0, emptyVector.size());
    EXPECT_EQ(5, multiElementVector.size());

    EXPECT_EQ(0, emptyVector.capacity());
    EXPECT_EQ(5, multiElementVector.capacity());
}

TEST_F(VectorTest, Reserve) {
    Vector<int> vec{1, 2, 3};
    EXPECT_EQ(3, vec.capacity());

    vec.reserve(10);
    EXPECT_EQ(10, vec.capacity());
    EXPECT_EQ(3, vec.size());
    EXPECT_EQ(1, vec[0]);
    EXPECT_EQ(2, vec[1]);
    EXPECT_EQ(3, vec[2]);

    vec.reserve(2); // Should not shrink capacity
    EXPECT_EQ(10, vec.capacity());
}

TEST_F(VectorTest, ShrinkToFit) {
    Vector<int> vec{1, 2, 3};
    vec.reserve(10);
    EXPECT_EQ(10, vec.capacity());

    vec.shrink_to_fit();
    EXPECT_EQ(3, vec.capacity());
    EXPECT_EQ(3, vec.size());

    // Test shrinking empty vector
    Vector<int> empty;
    empty.reserve(5);
    EXPECT_EQ(5, empty.capacity());

    empty.shrink_to_fit();
    EXPECT_EQ(0, empty.capacity());
}

// Modifier Tests
TEST_F(VectorTest, Clear) {
    multiElementVector.clear();
    EXPECT_EQ(0, multiElementVector.size());
    EXPECT_TRUE(multiElementVector.empty());
    EXPECT_EQ(5, multiElementVector.capacity()); // Capacity shouldn't change
}

TEST_F(VectorTest, PushBack) {
    Vector<int> vec;
    vec.push_back(1);
    EXPECT_EQ(1, vec.size());
    EXPECT_EQ(1, vec[0]);

    vec.push_back(2);
    EXPECT_EQ(2, vec.size());
    EXPECT_EQ(2, vec[1]);

    // Test capacity growth
    Vector<int> growVec;
    EXPECT_EQ(0, growVec.capacity());

    growVec.push_back(1);
    EXPECT_EQ(1, growVec.capacity());

    growVec.push_back(2);
    EXPECT_EQ(2, growVec.capacity());

    growVec.push_back(3);
    EXPECT_EQ(4, growVec.capacity());
}

TEST_F(VectorTest, PopBack) {
    multiElementVector.pop_back();
    EXPECT_EQ(4, multiElementVector.size());
    EXPECT_EQ(4, multiElementVector.back());

    EXPECT_THROW(emptyVector.pop_back(), std::out_of_range);
}

TEST_F(VectorTest, Insert) {
    multiElementVector.insert(2, 10);
    EXPECT_EQ(6, multiElementVector.size());
    EXPECT_EQ(10, multiElementVector[2]);
    EXPECT_EQ(3, multiElementVector[3]);

    EXPECT_THROW(multiElementVector.insert(7, 20), std::out_of_range);

    // Insert at beginning
    multiElementVector.insert(0, 0);
    EXPECT_EQ(7, multiElementVector.size());
    EXPECT_EQ(0, multiElementVector[0]);

    // Insert at end
    multiElementVector.insert(7, 7);
    EXPECT_EQ(8, multiElementVector.size());
    EXPECT_EQ(7, multiElementVector[7]);
}

TEST_F(VectorTest, Erase) {
    multiElementVector.erase(2);
    EXPECT_EQ(4, multiElementVector.size());
    EXPECT_EQ(4, multiElementVector[2]);

    EXPECT_THROW(multiElementVector.erase(4), std::out_of_range);
}

TEST_F(VectorTest, Resize) {
    // Resize larger
    emptyVector.resize(3);
    EXPECT_EQ(3, emptyVector.size());
    EXPECT_EQ(0, emptyVector[0]); // Default value

    // Resize with value
    emptyVector.resize(5, 10);
    EXPECT_EQ(5, emptyVector.size());
    EXPECT_EQ(10, emptyVector[3]);
    EXPECT_EQ(10, emptyVector[4]);

    // Resize smaller
    multiElementVector.resize(3);
    EXPECT_EQ(3, multiElementVector.size());
    EXPECT_EQ(1, multiElementVector[0]);
    EXPECT_EQ(3, multiElementVector[2]);
}

TEST_F(VectorTest, Swap) {
    Vector<int> vec1{1, 2, 3};
    Vector<int> vec2{4, 5, 6, 7};

    vec1.swap(vec2);

    EXPECT_EQ(4, vec1.size());
    EXPECT_EQ(3, vec2.size());

    EXPECT_EQ(4, vec1[0]);
    EXPECT_EQ(1, vec2[0]);
}

// Comparison Operator Tests
TEST_F(VectorTest, EqualityOperators) {
    Vector<int> vec1{1, 2, 3};
    Vector<int> vec2{1, 2, 3};
    Vector<int> vec3{1, 2, 4};
    Vector<int> vec4{1, 2};

    EXPECT_TRUE(vec1 == vec2);
    EXPECT_FALSE(vec1 == vec3);
    EXPECT_FALSE(vec1 == vec4);

    EXPECT_FALSE(vec1 != vec2);
    EXPECT_TRUE(vec1 != vec3);
    EXPECT_TRUE(vec1 != vec4);
}

TEST_F(VectorTest, ComparisonOperators) {
    Vector<int> vec1{1, 2, 3};
    Vector<int> vec2{1, 2, 4};
    Vector<int> vec3{1, 2, 3, 4};

    EXPECT_TRUE(vec1 < vec2);
    EXPECT_TRUE(vec1 <= vec2);
    EXPECT_FALSE(vec1 > vec2);
    EXPECT_FALSE(vec1 >= vec2);

    EXPECT_TRUE(vec1 < vec3);
    EXPECT_TRUE(vec1 <= vec3);
    EXPECT_FALSE(vec1 > vec3);
    EXPECT_FALSE(vec1 >= vec3);

    EXPECT_TRUE(vec1 <= vec1);
    EXPECT_TRUE(vec1 >= vec1);
}

TEST_F(VectorTest, ContainerComparison) {
    Vector<int> vec1{1, 2, 3};
    Vector<int> vec2{1, 2, 3};
    Vector<int> vec3{1, 2, 4};

    Container<int>& container1 = vec1;
    Container<int>& container2 = vec2;
    Container<int>& container3 = vec3;

    EXPECT_TRUE(container1 == container2);
    EXPECT_FALSE(container1 == container3);

    EXPECT_FALSE(container1 != container2);
    EXPECT_TRUE(container1 != container3);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}