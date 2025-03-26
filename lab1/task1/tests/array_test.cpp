#include <gtest/gtest.h>
#include "../include/array.h" // Убедитесь, что путь к вашему array.h верный
#include <string>
#include <stdexcept>

using namespace my_container;

class ArrayTest : public ::testing::Test {
protected:
    Array<int, 5> arr{1, 2, 3, 4, 5};
    Array<int, 5> emptyArr;
};

TEST_F(ArrayTest, Constructors) {
    Array<int, 5> defaultArr;
    Array<int, 5> copyArr(arr);
    Array<int, 5> moveArr(std::move(arr));
    EXPECT_EQ(copyArr[0], 1);
    EXPECT_EQ(moveArr[0], 1);
}

TEST_F(ArrayTest, AssignmentOperators) {
    Array<int, 5> a;
    a = arr;
    EXPECT_EQ(a[0], 1);
    Array<int, 5> b;
    b = std::move(a);
    EXPECT_EQ(b[0], 1);
}

TEST_F(ArrayTest, Accessors) {
    EXPECT_EQ(arr.at(0), 1);
    EXPECT_EQ(arr.front(), 1);
    EXPECT_EQ(arr.back(), 5);
    EXPECT_THROW(arr.at(10), std::out_of_range);
}

TEST_F(ArrayTest, Iterators) {
    EXPECT_EQ(*arr.begin(), 1);
    EXPECT_EQ(*(arr.end() - 1), 5);
    EXPECT_EQ(*arr.rbegin(), 5);
    EXPECT_EQ(*(arr.rend() - 1), 1);
}

TEST_F(ArrayTest, SizeMethods) {
    EXPECT_EQ(arr.size(), 5);
    EXPECT_EQ(arr.max_size(), 5);
    EXPECT_FALSE(arr.empty());

}

TEST_F(ArrayTest, FillAndSwap) {
    arr.fill(7);
    EXPECT_EQ(arr[0], 7);
    Array<int, 5> other{9, 8, 7, 6, 5};
    arr.swap(other);
    EXPECT_EQ(arr[0], 9);
    EXPECT_EQ(other[0], 7);
}

TEST_F(ArrayTest, Comparisons) {
    Array<int, 5> same{1, 2, 3, 4, 5};
    Array<int, 5> greater{2, 3, 4, 5, 6};
    EXPECT_EQ(arr, same);
    EXPECT_NE(arr, greater);
    EXPECT_LT(arr, greater);
    EXPECT_LE(arr, same);
    EXPECT_GT(greater, arr);
    EXPECT_GE(arr, same);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
