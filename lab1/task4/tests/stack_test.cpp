#include <gtest/gtest.h>
#include "../include/stack.h"

using namespace my_container;

class StackTest : public ::testing::Test {
protected:
    Stack<std::string> stack;
    Stack<std::string> filledStack;

    void SetUp() override {
        filledStack.push("one");
        filledStack.push("two");
        filledStack.push("three");
    }
};

TEST_F(StackTest, DefaultConstructor) {
    EXPECT_TRUE(stack.empty());
    EXPECT_EQ(stack.size(), 0);
}

TEST_F(StackTest, CopyConstructor) {
    Stack<std::string> copyStack(filledStack);
    EXPECT_EQ(copyStack, filledStack);
}

TEST_F(StackTest, MoveConstructor) {
    Stack<std::string> movedStack(std::move(filledStack));
    EXPECT_EQ(movedStack.size(), 3);
    EXPECT_TRUE(filledStack.empty());
}

TEST_F(StackTest, InitializerListConstructor) {
    Stack<std::string> ilStack({"one", "two", "three"});
    EXPECT_EQ(ilStack.size(), 3);
    EXPECT_EQ(ilStack.top(), "three");
}

TEST_F(StackTest, CopyAssignment) {
    Stack<std::string> copyStack;
    copyStack = filledStack;
    EXPECT_EQ(copyStack, filledStack);
}

TEST_F(StackTest, MoveAssignment) {
    Stack<std::string> movedStack;
    movedStack = std::move(filledStack);
    EXPECT_EQ(movedStack.size(), 3);
    EXPECT_TRUE(filledStack.empty());
}

TEST_F(StackTest, TopMethod) {
    EXPECT_EQ(filledStack.top(), "three");
    filledStack.pop();
    EXPECT_EQ(filledStack.top(), "two");
}


TEST_F(StackTest, EmptyMethod) {
    EXPECT_TRUE(stack.empty());
    stack.push("hello");
    EXPECT_FALSE(stack.empty());
}

TEST_F(StackTest, SizeMethod) {
    EXPECT_EQ(stack.size(), 0);
    stack.push("one");
    EXPECT_EQ(stack.size(), 1);
    stack.push("two");
    EXPECT_EQ(stack.size(), 2);
}

TEST_F(StackTest, PushPop) {
    stack.push("one");
    EXPECT_EQ(stack.top(), "one");
    stack.push("two");
    EXPECT_EQ(stack.top(), "two");
    stack.pop();
    EXPECT_EQ(stack.top(), "one");
}



TEST_F(StackTest, Swap) {
    Stack<std::string> otherStack({"four", "five"});
    filledStack.swap(otherStack);
    EXPECT_EQ(filledStack.size(), 2);
    EXPECT_EQ(filledStack.top(), "five");
    EXPECT_EQ(otherStack.size(), 3);
    EXPECT_EQ(otherStack.top(), "three");
}

TEST_F(StackTest, ComparisonOperators) {
    Stack<std::string> identicalStack({"one", "two", "three"});
    Stack<std::string> smallerStack({"one", "two"});
    Stack<std::string> differentStack({"one", "two", "four"});

    EXPECT_EQ(filledStack, identicalStack);
    EXPECT_NE(filledStack, smallerStack);
    EXPECT_LT(smallerStack, filledStack);
    EXPECT_LE(smallerStack, filledStack);
    EXPECT_GT(filledStack, smallerStack);
    EXPECT_GE(filledStack, smallerStack);
    EXPECT_GT(filledStack, differentStack);
}
