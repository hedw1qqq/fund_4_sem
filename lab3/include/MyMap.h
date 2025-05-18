//
// Created by ivglu on 18.05.2025.
//

#pragma once
#include <functional>
template <typename Key, typename Value, template <typename, typename, typename> class TreeType,
          typename Comparator = std::less<Key>>
class MyMap {
    TreeType<Key, Value, Comparator> tree_;

public:
    using iterator = typename TreeType<Key, Value, Comparator>::iterator;
    using const_iterator = typename TreeType<Key, Value, Comparator>::const_iterator;

    void insert(const Key& key, const Value& value) { tree_.insert(key, value); }

    void remove(const Key& key) { tree_.remove(key); }

    Value* find(const Key& key) { return tree_.find(key); }
    const Value* find(const Key& key) const { return tree_.find(key); }

    bool contains(const Key& key) const { return tree_.contains(key); }
    void clear() { tree_.clear(); }
    [[nodiscard]] size_t size() const { return tree_.size(); }

    iterator begin() noexcept { return tree_.begin(); }
    const_iterator cbegin() const noexcept { return tree_.cbegin(); }

    iterator end() noexcept { return tree_.end(); }
    const_iterator cend() const noexcept { return tree_.cend(); }

    using CallBack = typename TreeType<Key, Value, Comparator>::CallBack;


    void inorder(CallBack cb)  { tree_.inorder_user(cb); }
    void preorder(CallBack cb)  { tree_.preorder_user(cb); }
    void postorder(CallBack cb)  { tree_.postorder_user(cb); }
};