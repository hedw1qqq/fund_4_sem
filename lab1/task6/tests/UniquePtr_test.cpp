#include <gtest/gtest.h>
#include "../include/UniquePtr.h"

using namespace my_smart_ptr;

class UniquePtr_Test : public ::testing::Test {};

TEST_F(UniquePtr_Test, DefaultConstructorTest) {
    UniquePtr<int> ptr;
    EXPECT_EQ(ptr.get(), nullptr);
    EXPECT_FALSE(ptr);
}

TEST_F(UniquePtr_Test, PointerConstructorTest) {
    int *rawPtr = new int(42);
    UniquePtr<int> ptr(rawPtr);
    EXPECT_EQ(ptr.get(), rawPtr);
    EXPECT_TRUE(ptr);
    EXPECT_EQ(*ptr, 42);
}

TEST_F(UniquePtr_Test, OperatorBoolTest) {
    UniquePtr<int> ptr(new int(100));
    EXPECT_TRUE(ptr);
    UniquePtr<int> empty;
    EXPECT_FALSE(empty);
}

TEST_F(UniquePtr_Test, OperatorArrowTest) {
    struct S { int a; };
    S* s = new S{55};
    UniquePtr<S> ptr(s);
    EXPECT_EQ(ptr->a, 55);
}

TEST_F(UniquePtr_Test, OperatorDereferenceTest) {
    int *rawPtr = new int(77);
    UniquePtr<int> ptr(rawPtr);
    EXPECT_EQ(*ptr, 77);
}

TEST_F(UniquePtr_Test, MoveConstructorTest) {
    int *rawPtr = new int(42);
    UniquePtr<int> original(rawPtr);
    UniquePtr<int> moved(std::move(original));
    EXPECT_EQ(moved.get(), rawPtr);
}

TEST_F(UniquePtr_Test, MoveAssignmentTest) {
    int *rawPtr1 = new int(42);
    int *rawPtr2 = new int(10);
    UniquePtr<int> ptr1(rawPtr1);
    UniquePtr<int> ptr2(rawPtr2);
    ptr1 = std::move(ptr2);
    EXPECT_EQ(*ptr1, 10);
}

TEST_F(UniquePtr_Test, ArraySpecializationTest) {
    UniquePtr<int[]> ptr(new int[5]{1, 2, 3, 4, 5});
    EXPECT_EQ(ptr[0], 1);
    EXPECT_EQ(ptr[4], 5);
}

TEST_F(UniquePtr_Test, ArrayOperatorTest) {
    UniquePtr<int[]> ptr(new int[3]{10, 20, 30});
    EXPECT_EQ(ptr[1], 20);
}

TEST_F(UniquePtr_Test, ResetMethodTest) {
    UniquePtr<int> ptr(new int(42));
    ptr.reset(new int(10));
    EXPECT_EQ(*ptr, 10);
}

TEST_F(UniquePtr_Test, ResetToNullTest) {
    UniquePtr<int> ptr(new int(55));
    ptr.reset();
    EXPECT_EQ(ptr.get(), nullptr);
}

TEST_F(UniquePtr_Test, ReleaseMethodTest) {
    UniquePtr<int> ptr(new int(42));
    int *releasedPtr = ptr.release();
    EXPECT_EQ(*releasedPtr, 42);
    EXPECT_EQ(ptr.get(), nullptr);
    delete releasedPtr;
}

TEST_F(UniquePtr_Test, SwapMethodTest) {
    UniquePtr<int> ptr1(new int(42));
    UniquePtr<int> ptr2(new int(10));
    ptr1.swap(ptr2);
    EXPECT_EQ(*ptr1, 10);
    EXPECT_EQ(*ptr2, 42);
}

TEST_F(UniquePtr_Test, MultipleResetTest) {
    UniquePtr<int> ptr(new int(5));
    ptr.reset(new int(15));
    EXPECT_EQ(*ptr, 15);
    ptr.reset(new int(25));
    EXPECT_EQ(*ptr, 25);
}

TEST_F(UniquePtr_Test, SwapArrayTest) {
    UniquePtr<int[]> arr1(new int[3]{1, 2, 3});
    UniquePtr<int[]> arr2(new int[3]{4, 5, 6});
    arr1.swap(arr2);
    EXPECT_EQ(arr1[0], 4);
    EXPECT_EQ(arr2[0], 1);
}
