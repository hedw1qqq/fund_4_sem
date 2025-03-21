#include <gtest/gtest.h>
#include "../include/Array.h"
#include <string>
#include <stdexcept>

TEST(ArrayTest, ConstructorDefault) {
    my_container::Array<int, 5> arr;
    EXPECT_EQ(arr.size(), 5);
    EXPECT_FALSE(arr.empty());

    for (int i = 0; i < 5; ++i) {
        EXPECT_EQ(arr[i], 0);
    }
}

TEST(ArrayTest, ConstructorInitializerList) {
    my_container::Array<int, 5> arr = {1, 2, 3, 4, 5};
    EXPECT_EQ(arr.size(), 5);

    for (int i = 0; i < 5; ++i) {
        EXPECT_EQ(arr[i], i + 1);
    }

    // Test initializer list shorter than array size
    my_container::Array<int, 5> arr2 = {10, 20, 30};
    EXPECT_EQ(arr2.size(), 5);
    EXPECT_EQ(arr2[0], 10);
    EXPECT_EQ(arr2[1], 20);
    EXPECT_EQ(arr2[2], 30);
    EXPECT_EQ(arr2[3], 0);
    EXPECT_EQ(arr2[4], 0);

    // Test initializer list longer than array size
    my_container::Array<int, 3> arr3 = {100, 200, 300, 400, 500};
    EXPECT_EQ(arr3.size(), 3);
    EXPECT_EQ(arr3[0], 100);
    EXPECT_EQ(arr3[1], 200);
    EXPECT_EQ(arr3[2], 300);
}

TEST(ArrayTest, CopyConstructor) {
    my_container::Array<int, 5> arr1 = {1, 2, 3, 4, 5};
    my_container::Array<int, 5> arr2(arr1);

    EXPECT_EQ(arr1.size(), arr2.size());

    for (int i = 0; i < 5; ++i) {
        EXPECT_EQ(arr1[i], arr2[i]);
    }
}

TEST(ArrayTest, MoveConstructor) {
    my_container::Array<int, 5> arr1 = {1, 2, 3, 4, 5};
    my_container::Array<int, 5> arr2(std::move(arr1));

    EXPECT_EQ(arr2.size(), 5);

    for (int i = 0; i < 5; ++i) {
        EXPECT_EQ(arr2[i], i + 1);
    }
}

TEST(ArrayTest, CopyAssignment) {
    my_container::Array<int, 5> arr1 = {1, 2, 3, 4, 5};
    my_container::Array<int, 5> arr2;

    arr2 = arr1;

    EXPECT_EQ(arr1.size(), arr2.size());

    for (int i = 0; i < 5; ++i) {
        EXPECT_EQ(arr1[i], arr2[i]);
    }
}

TEST(ArrayTest, MoveAssignment) {
    my_container::Array<int, 5> arr1 = {1, 2, 3, 4, 5};
    my_container::Array<int, 5> arr2;

    arr2 = std::move(arr1);

    EXPECT_EQ(arr2.size(), 5);

    for (int i = 0; i < 5; ++i) {
        EXPECT_EQ(arr2[i], i + 1);
    }
}

TEST(ArrayTest, At) {
    my_container::Array<int, 5> arr = {10, 20, 30, 40, 50};

    EXPECT_EQ(arr.at(0), 10);
    EXPECT_EQ(arr.at(4), 50);

    EXPECT_THROW(arr.at(5), std::out_of_range);
}

TEST(ArrayTest, ConstAt) {
    const my_container::Array<int, 5> arr = {10, 20, 30, 40, 50};

    EXPECT_EQ(arr.at(0), 10);
    EXPECT_EQ(arr.at(4), 50);

    EXPECT_THROW(arr.at(5), std::out_of_range);
}

TEST(ArrayTest, IndexOperator) {
    my_container::Array<int, 5> arr = {10, 20, 30, 40, 50};

    EXPECT_EQ(arr[0], 10);
    EXPECT_EQ(arr[4], 50);

    arr[2] = 100;
    EXPECT_EQ(arr[2], 100);
}

TEST(ArrayTest, ConstIndexOperator) {
    const my_container::Array<int, 5> arr = {10, 20, 30, 40, 50};

    EXPECT_EQ(arr[0], 10);
    EXPECT_EQ(arr[4], 50);
}

