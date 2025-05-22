

#pragma once
#include <functional>
#include <memory>
#include <stack>

template <typename Key, typename Value, typename Comparator = std::less<Key> >
class BSTree {
   public:
	struct Node {
		Key key;
		Value value;
		Node *left_;
		Node *right_;

		Node(const Key &key, const Value &value) : key(key), value(value), left_(nullptr), right_(nullptr) {}
	};
	using CallBack = std::function<void(Node *node)>;

   private:
	size_t size_{};
	Node *root_{};
	Comparator comparator_;
	Node *copy_nodes_recursive(Node *other_node) {
		if (other_node == nullptr) {
			return nullptr;
		}
		Node *new_node = new Node(other_node->key, other_node->value);
		new_node->left_ = copy_nodes_recursive(other_node->left_);
		new_node->right_ = copy_nodes_recursive(other_node->right_);
		return new_node;
	}
	void preorder(Node *node, CallBack call_back) {
		if (node == nullptr) {
			return;
		}
		call_back(node);
		preorder(node->left_, call_back);
		preorder(node->right_, call_back);
	}

	void inorder(Node *node, CallBack call_back) {
		if (node == nullptr) {
			return;
		}
		inorder(node->left_, call_back);
		call_back(node);
		inorder(node->right_, call_back);
	}

	void postorder(Node *node, CallBack call_back) {
		if (node == nullptr) {
			return;
		}
		postorder(node->left_, call_back);
		postorder(node->right_, call_back);
		call_back(node);
	}

   public:
	BSTree() = default;

	explicit BSTree(const Key &key, const Value &value) {
		root_ = new Node(key, value);
		size_ = 1;
	}
	BSTree(const BSTree &other)
	    : size_(other.size_), root_(copy_nodes_recursive(other.root_)), comparator_(other.comparator_) {}
	BSTree(BSTree &&other) noexcept {
		root_ = other.root_;
		size_ = other.size_;
		comparator_ = std::move(other.comparator_);
	}
	BSTree &operator=(const BSTree &other) {
		if (this != &other) {
			clear();
			root_ = copy_nodes_recursive(other.root_);
			size_ = other.size_;
			comparator_ = other.comparator_;
		}
		return *this;
	}

	BSTree &operator=(BSTree &&other) noexcept {
		if (this == &other) {
			return *this;
		}
		clear();

		root_ = other.root_;
		size_ = other.size_;
		comparator_ = std::move(other.comparator_);

		other.root_ = nullptr;
		other.size_ = 0;
		return *this;
	}

	virtual ~BSTree() { clear(); }

	[[nodiscard]] size_t size() const { return size_; }

	virtual void insert(const Key &key, const Value &value) {
		if (root_ == nullptr) {
			root_ = new Node(key, value);
			size_ = 1;
			return;
		}
		Node *current = root_;
		Node *parent = nullptr;

		while (current != nullptr) {
			parent = current;
			if (comparator_(key, current->key)) {
				current = current->left_;
			} else if (comparator_(current->key, key)) {
				current = current->right_;
			} else {
				current->value = value;
				return;
			}
		}

		Node *newNode = new Node(key, value);
		if (comparator_(key, parent->key)) {
			parent->left_ = newNode;
		} else {
			parent->right_ = newNode;
		}
		size_ += 1;
	}

	Value *find(const Key &key) const {
		Node *current = root_;
		while (current != nullptr) {
			if (comparator_(key, current->key)) {
				current = current->left_;
			} else if (comparator_(current->key, key)) {
				current = current->right_;
			} else {
				return &(current->value);
			}
		}
		return nullptr;
	}

	bool contains(const Key &key) const { return find(key) != nullptr; }

	virtual void remove(const Key &key) {
		Node *parent = nullptr;
		Node *current = root_;

		while (current != nullptr) {
			if (comparator_(key, current->key)) {
				parent = current;
				current = current->left_;

			} else if (comparator_(current->key, key)) {
				parent = current;
				current = current->right_;
			} else {
				break;
			}
		}

		if (current == nullptr) {
			return;
		}

		if (current->left_ == nullptr) {
			Node *child_to_promote = current->right_;

			if (parent == nullptr) {
				root_ = child_to_promote;
			} else {
				if (parent->left_ == current) {
					parent->left_ = child_to_promote;
				} else {
					parent->right_ = child_to_promote;
				}
			}
			delete current;
		}

		else if (current->right_ == nullptr) {
			Node *child_to_promote = current->left_;

			if (parent == nullptr) {
				root_ = child_to_promote;
			} else {
				if (parent->left_ == current) {
					parent->left_ = child_to_promote;
				} else {
					parent->right_ = child_to_promote;
				}
			}
			delete current;
		}

		else {
			Node *successor_parent = current;
			Node *successor = current->right_;

			while (successor->left_ != nullptr) {
				successor_parent = successor;
				successor = successor->left_;
			}

			current->key = successor->key;
			current->value = successor->value;

			if (successor_parent == current) {
				successor_parent->right_ = successor->right_;
			} else {
				successor_parent->left_ = successor->right_;
			}
			delete successor;
		}

		size_--;
	}

	void preorder_user(CallBack user_cb) { preorder(root_, user_cb); }
	void inorder_user(CallBack user_cb) { inorder(root_, user_cb); }
	void postorder_user(CallBack user_cb) { postorder(root_, user_cb); }
	class iterator {
	   private:
		std::stack<Node *> s_;

		void push_all_left_(Node *node) {
			while (node) {
				s_.push(node);
				node = node->left_;
			}
		}

	   public:
		using iterator_category = std::forward_iterator_tag;
		using value_type = Node *;
		using difference_type = std::ptrdiff_t;
		using pointer = Node **;
		using reference = Node *&;

		explicit iterator(Node *root_node = nullptr) {
			if (root_node) {
				push_all_left_(root_node);
			}
		}

		~iterator() = default;

		value_type operator*() const { return s_.top(); }

		Node *operator->() const { return s_.top(); }

		iterator &operator++() {
			Node *current_top = s_.top();
			s_.pop();
			if (current_top->right_) {
				push_all_left_(current_top->right_);
			}
			return *this;
		}

		iterator operator++(int) {
			iterator temp = *this;
			++(*this);
			return temp;
		}

		bool operator==(const iterator &other) const {
			if (s_.empty() && other.s_.empty()) {
				return true;
			}
			if (s_.empty() || other.s_.empty()) {
				return false;
			}
			return s_.top() == other.s_.top();
		}

		bool operator!=(const iterator &other) const { return !(*this == other); }
	};
	class const_iterator {
	   private:
		std::stack<const Node *> s_;

		void push_all_left_(const Node *node) {
			while (node) {
				s_.push(node);
				node = node->left_;
			}
		}

	   public:
		using iterator_category = std::forward_iterator_tag;
		using value_type = const Node *;
		using difference_type = std::ptrdiff_t;
		using pointer = const Node **;
		using reference = const Node *&;

		explicit const_iterator(const Node *root_node = nullptr) {
			if (root_node) {
				push_all_left_(root_node);
			}
		}

		~const_iterator() = default;

		value_type operator*() const { return s_.top(); }

		const Node *operator->() const { return s_.top(); }

		const_iterator &operator++() {
			if (s_.empty()) {
				return *this;
			}
			const Node *current_top = s_.top();
			s_.pop();
			if (current_top->right_) {
				push_all_left_(current_top->right_);
			}
			return *this;
		}

		const_iterator operator++(int) {
			const_iterator temp = *this;
			++(*this);
			return temp;
		}

		bool operator==(const const_iterator &other) const {
			if (s_.empty() && other.s_.empty()) {
				return true;
			}
			if (s_.empty() || other.s_.empty()) {
				return false;
			}
			return s_.top() == other.s_.top();
		}

		bool operator!=(const const_iterator &other) const { return !(*this == other); }
	};
	iterator begin() noexcept { return iterator(root_); }
	const_iterator cbegin() const noexcept { return const_iterator(root_); }

	iterator end() noexcept { return iterator(); }
	const_iterator cend() const noexcept { return const_iterator(); }

	void clear() {
		if (root_ == nullptr) {
			size_ = 0;
			return;
		}
		postorder(root_, [](Node *node) { delete node; });
		root_ = nullptr;
		size_ = 0;
	}
};
