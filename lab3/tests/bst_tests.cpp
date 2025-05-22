#include <algorithm>
#include <string>
#include <vector>

#include "../include/BSTTree.h"
#include "../include/MyMap.h"
#include "gtest/gtest.h"

class MyMapTest : public ::testing::Test {
   protected:
	MyMap<int, std::string, BSTree> map_int_str;
	MyMap<int, int, BSTree> map_int_int;
};

TEST_F(MyMapTest, MapCopyMoveOperations) {
	map_int_str.insert(1, "one");
	map_int_str.insert(2, "two");

	MyMap<int, std::string, BSTree> map_copy_ctor(map_int_str);
	EXPECT_EQ(map_copy_ctor.size(), 2);
	map_copy_ctor.insert(1, "new_one_copy");
	EXPECT_EQ(*(map_int_str.find(1)), "one");

	MyMap<int, std::string, BSTree> map_copy_assign;
	map_copy_assign = map_int_str;
	EXPECT_EQ(map_copy_assign.size(), 2);

	MyMap<int, std::string, BSTree> map_move_ctor(std::move(map_copy_ctor));
	EXPECT_EQ(map_move_ctor.size(), 2);
	ASSERT_NE(map_move_ctor.find(1), nullptr);
	EXPECT_EQ(*(map_move_ctor.find(1)), "new_one_copy");
	EXPECT_EQ(map_copy_ctor.size(), 0);

	MyMap<int, std::string, BSTree> map_move_assign;
	map_move_assign = std::move(map_copy_assign);
	EXPECT_EQ(map_move_assign.size(), 2);
	EXPECT_EQ(map_copy_assign.size(), 0);
}
TEST(BSTreeDirectTest, ConstructorWithInitialNode) {
	BSTree<int, std::string> tree(42, "answer");
	EXPECT_EQ(tree.size(), 1);
	EXPECT_TRUE(tree.contains(42));
	std::string* val = tree.find(42);
	ASSERT_NE(val, nullptr);
	EXPECT_EQ(*val, "answer");
}
TEST_F(MyMapTest, EmptyMapOperations) {
	EXPECT_EQ(map_int_str.size(), 0);
	EXPECT_TRUE(map_int_str.begin() == map_int_str.end());
	map_int_str.clear();
	EXPECT_EQ(map_int_str.size(), 0);
	EXPECT_FALSE(map_int_str.contains(1));
	EXPECT_EQ(map_int_str.find(1), nullptr);
	map_int_str.remove(1);
	EXPECT_EQ(map_int_str.size(), 0);
}

TEST_F(MyMapTest, InsertFindContainsUpdate) {
	map_int_str.insert(10, "apple");
	EXPECT_EQ(map_int_str.size(), 1);
	EXPECT_TRUE(map_int_str.contains(10));
	EXPECT_FALSE(map_int_str.contains(5));

	std::string* val_ptr = map_int_str.find(10);
	ASSERT_NE(val_ptr, nullptr);
	EXPECT_EQ(*val_ptr, "apple");
	EXPECT_EQ(map_int_str.find(5), nullptr);

	map_int_str.insert(5, "banana");
	EXPECT_EQ(map_int_str.size(), 2);
	EXPECT_TRUE(map_int_str.contains(5));

	map_int_str.insert(10, "apricot");
	EXPECT_EQ(map_int_str.size(), 2);
	val_ptr = map_int_str.find(10);
	ASSERT_NE(val_ptr, nullptr);
	EXPECT_EQ(*val_ptr, "apricot");
}

TEST_F(MyMapTest, ClearNonEmptyMap) {
	map_int_str.insert(10, "a");
	map_int_str.insert(20, "b");
	map_int_str.clear();
	EXPECT_EQ(map_int_str.size(), 0);
	EXPECT_FALSE(map_int_str.contains(10));
	EXPECT_TRUE(map_int_str.begin() == map_int_str.end());
}

TEST_F(MyMapTest, RemoveAndIteratorOrder) {
	map_int_int.insert(50, 500);
	map_int_int.insert(30, 300);
	map_int_int.insert(70, 700);
	map_int_int.insert(20, 200);
	map_int_int.insert(40, 400);
	map_int_int.insert(60, 600);
	map_int_int.insert(80, 800);

	EXPECT_EQ(map_int_int.size(), 7);

	map_int_int.remove(20);
	EXPECT_EQ(map_int_int.size(), 6);
	EXPECT_FALSE(map_int_int.contains(20));

	map_int_int.remove(30);
	EXPECT_EQ(map_int_int.size(), 5);
	EXPECT_FALSE(map_int_int.contains(30));
	EXPECT_TRUE(map_int_int.contains(40));

	map_int_int.remove(50);
	EXPECT_EQ(map_int_int.size(), 4);
	EXPECT_FALSE(map_int_int.contains(50));
	EXPECT_TRUE(map_int_int.contains(60));

	std::cout << "After removing 50, checking key 60:" << std::endl;
	int* val60 = map_int_int.find(60);
	ASSERT_NE(val60, nullptr);
	EXPECT_EQ(*val60, 600);

	std::vector<int> actual_keys;
	for (auto it = map_int_int.begin(); it != map_int_int.end(); ++it) {
		actual_keys.push_back((*it)->key);
	}
	std::vector<int> expected_keys = {40, 60, 70, 80};
	EXPECT_EQ(actual_keys, expected_keys);
	EXPECT_TRUE(std::is_sorted(actual_keys.begin(), actual_keys.end()));

	map_int_int.remove(60);
	EXPECT_EQ(map_int_int.size(), 3);
	EXPECT_FALSE(map_int_int.contains(60));

	actual_keys.clear();
	for (auto it = map_int_int.begin(); it != map_int_int.end(); ++it) {
		actual_keys.push_back((*it)->key);
	}
	EXPECT_TRUE(std::is_sorted(actual_keys.begin(), actual_keys.end()));
	EXPECT_EQ(actual_keys.size(), 3);
}

TEST_F(MyMapTest, IteratorOperationsAndOrder) {
	map_int_int.insert(20, 2);
	map_int_int.insert(10, 1);
	map_int_int.insert(30, 3);

	std::vector<int> keys_ordered;
	std::vector<int> values_ordered;
	for (auto it = map_int_int.begin(); it != map_int_int.end(); ++it) {
		keys_ordered.push_back((*it)->key);
		values_ordered.push_back((*it)->value);
	}
	std::vector<int> expected_keys = {10, 20, 30};
	std::vector<int> expected_values = {1, 2, 3};
	EXPECT_EQ(keys_ordered, expected_keys);
	EXPECT_EQ(values_ordered, expected_values);

	keys_ordered.clear();
	for (auto it = map_int_int.cbegin(); it != map_int_int.cend(); ++it) {
		keys_ordered.push_back((*it)->key);
	}
	EXPECT_EQ(keys_ordered, expected_keys);
}

TEST_F(MyMapTest, RemoveUntilEmptyAndCheckIterators) {
	map_int_int.insert(3, 30);
	map_int_int.insert(1, 10);
	map_int_int.insert(2, 20);
	map_int_int.insert(4, 40);

	map_int_int.remove(1);
	map_int_int.remove(3);
	map_int_int.remove(4);
	map_int_int.remove(2);

	EXPECT_EQ(map_int_int.size(), 0);
	EXPECT_TRUE(map_int_int.begin() == map_int_int.end());
}
TEST_F(MyMapTest, ConstIteratorOperations) {
	map_int_int.insert(20, 200);
	map_int_int.insert(10, 100);
	map_int_int.insert(30, 300);

	const auto& const_map = map_int_int;

	std::vector<int> keys_ordered;
	std::vector<int> values_ordered;

	for (auto it = const_map.cbegin(); it != const_map.cend(); ++it) {
		keys_ordered.push_back((*it)->key);
		values_ordered.push_back((*it)->value);
	}

	std::vector<int> expected_keys = {10, 20, 30};
	std::vector<int> expected_values = {100, 200, 300};
	EXPECT_EQ(keys_ordered, expected_keys);
	EXPECT_EQ(values_ordered, expected_values);

	keys_ordered.clear();
	values_ordered.clear();
	for (auto it = map_int_int.cbegin(); it != map_int_int.cend(); ++it) {
		keys_ordered.push_back((*it)->key);
		values_ordered.push_back((*it)->value);
	}
	EXPECT_EQ(keys_ordered, expected_keys);
	EXPECT_EQ(values_ordered, expected_values);
}

TEST_F(MyMapTest, ConstIteratorEmptyMap) {
	const auto& const_map = map_int_int;
	EXPECT_TRUE(const_map.cbegin() == const_map.cend());
	EXPECT_TRUE(const_map.cbegin() == const_map.cend());
}

TEST_F(MyMapTest, ConstIteratorOneElement) {
	map_int_int.insert(10, 100);
	const auto& const_map = map_int_int;

	auto it = const_map.cbegin();
	ASSERT_NE(it, const_map.cend());
	EXPECT_EQ((*it)->key, 10);
	EXPECT_EQ((*it)->value, 100);
	++it;
	EXPECT_EQ(it, const_map.cend());
}

TEST_F(MyMapTest, ConstFind) {
	map_int_str.insert(10, "apple");
	map_int_str.insert(5, "banana");

	const auto& const_map_str = map_int_str;

	const std::string* val_ptr = const_map_str.find(10);
	ASSERT_NE(val_ptr, nullptr);
	EXPECT_EQ(*val_ptr, "apple");

	EXPECT_EQ(const_map_str.find(100), nullptr);
}

TEST_F(MyMapTest, ConstContains) {
	map_int_str.insert(10, "apple");
	const auto& const_map_str = map_int_str;
	EXPECT_TRUE(const_map_str.contains(10));
	EXPECT_FALSE(const_map_str.contains(50));
}

TEST_F(MyMapTest, ConstSize) {
	map_int_str.insert(10, "apple");
	const auto& const_map_str = map_int_str;
	EXPECT_EQ(const_map_str.size(), 1);
}
TEST_F(MyMapTest, RemoveSpecificParentLinks) {
	map_int_int.insert(10, 100);
	map_int_int.insert(5, 50);
	map_int_int.insert(15, 150);
	map_int_int.insert(12, 120);
	map_int_int.insert(17, 170);

	map_int_int.clear();
	map_int_int.insert(10, 100);
	map_int_int.insert(15, 150);
	map_int_int.insert(17, 170);

	map_int_int.remove(15);
	EXPECT_EQ(map_int_int.size(), 2);
	EXPECT_FALSE(map_int_int.contains(15));
	EXPECT_TRUE(map_int_int.contains(17));

	auto it = map_int_int.begin();
	ASSERT_NE(it, map_int_int.end());
	ASSERT_NE((*it)->right_, nullptr);
	EXPECT_EQ((*it)->right_->key, 17);

	map_int_int.clear();
	map_int_int.insert(10, 100);
	map_int_int.insert(15, 150);
	map_int_int.insert(12, 120);

	map_int_int.remove(15);
	EXPECT_EQ(map_int_int.size(), 2);
	EXPECT_FALSE(map_int_int.contains(15));
	EXPECT_TRUE(map_int_int.contains(12));

	it = map_int_int.begin();
	ASSERT_NE(it, map_int_int.end());
	ASSERT_NE((*it)->right_, nullptr);
	EXPECT_EQ((*it)->right_->key, 12);
}

TEST_F(MyMapTest, IteratorSpecificCoverage) {
	map_int_int.insert(10, 100);
	map_int_int.insert(20, 200);

	auto it1_begin = map_int_int.begin();
	auto it2_begin = map_int_int.begin();
	auto it1_second = map_int_int.begin();
	++it1_second;

	EXPECT_TRUE(it1_begin == it2_begin);
	EXPECT_FALSE(it1_begin == it1_second);
	EXPECT_TRUE(it1_begin != it1_second);

	const auto& const_map = map_int_int;
	auto cit1_begin = const_map.cbegin();
	auto cit2_begin = const_map.cbegin();
	auto cit1_second = const_map.cbegin();
	++cit1_second;

	EXPECT_TRUE(cit1_begin == cit2_begin);
	EXPECT_FALSE(cit1_begin == cit1_second);
	EXPECT_TRUE(cit1_begin != cit1_second);
	auto cit1 = map_int_int.cbegin();
	cit1++;
	EXPECT_EQ(cit1->key, 20);
	auto it3_begin = map_int_int.begin();
	it3_begin++;
	EXPECT_EQ(it3_begin->key, 20);
}

TEST_F(MyMapTest, RemoveElementInRightSubtree) {
	map_int_int.insert(10, 100);
	map_int_int.insert(5, 50);
	map_int_int.insert(20, 200);
	map_int_int.insert(15, 150);
	map_int_int.insert(25, 250);

	map_int_int.remove(20);
	EXPECT_EQ(map_int_int.size(), 4);
	EXPECT_FALSE(map_int_int.contains(20));
	EXPECT_TRUE(map_int_int.contains(15));
	EXPECT_TRUE(map_int_int.contains(25));
}
TEST_F(MyMapTest, PreorderNodeTraversalViaMyMap) {
	map_int_int.insert(50, 500);
	map_int_int.insert(30, 300);
	map_int_int.insert(70, 700);
	map_int_int.insert(20, 200);
	map_int_int.insert(40, 400);
	map_int_int.insert(60, 600);
	map_int_int.insert(80, 800);

	std::vector<int> visited_keys;
	map_int_int.preorder([&visited_keys](BSTree<int, int>::Node* n) {
		if (n) {
			visited_keys.push_back(n->key);
		}
	});

	std::vector<int> expected_keys = {50, 30, 20, 40, 70, 60, 80};
	EXPECT_EQ(visited_keys, expected_keys);
}

TEST_F(MyMapTest, InorderNodeTraversalViaMyMap) {
	map_int_int.insert(50, 500);
	map_int_int.insert(30, 300);
	map_int_int.insert(70, 700);
	map_int_int.insert(20, 200);
	map_int_int.insert(40, 400);
	map_int_int.insert(60, 600);
	map_int_int.insert(80, 800);

	std::vector<int> visited_keys;
	map_int_int.inorder([&visited_keys](BSTree<int, int>::Node* n) {
		if (n) {
			visited_keys.push_back(n->key);
		}
	});

	std::vector<int> expected_keys = {20, 30, 40, 50, 60, 70, 80};
	EXPECT_EQ(visited_keys, expected_keys);
}

TEST_F(MyMapTest, PostorderNodeTraversalViaMyMap) {
	map_int_int.insert(50, 500);
	map_int_int.insert(30, 300);
	map_int_int.insert(70, 700);
	map_int_int.insert(20, 200);
	map_int_int.insert(40, 400);
	map_int_int.insert(60, 600);
	map_int_int.insert(80, 800);

	std::vector<int> visited_keys;
	map_int_int.postorder([&visited_keys](BSTree<int, int>::Node* n) {
		if (n) {
			visited_keys.push_back(n->key);
		}
	});

	std::vector<int> expected_keys = {20, 40, 30, 60, 80, 70, 50};
	EXPECT_EQ(visited_keys, expected_keys);
}
int main(int argc, char** argv) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}