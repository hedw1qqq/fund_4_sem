#ifndef LIST_H
#define LIST_H
#include <algorithm>
#include <iterator>
#include <stdexcept>

#include "Container.h"

namespace my_container {
template <typename T>
struct Node {
	T data;
	Node *prev;
	Node *next;

	explicit Node(const T &value, Node *prev = nullptr, Node *next = nullptr) : data(value), prev(prev), next(next) {}

	explicit Node(Node *prev = nullptr, Node *next = nullptr) : data(T()), prev(prev), next(next) {}
};

template <typename T>
class List : public Container<T> {
   private:
	Node<T> *head;
	Node<T> *tail;
	size_t count;

   public:
	class iterator {
	   private:
		friend class List<T>;
		Node<T> *current;

	   public:
		using iterator_category = std::bidirectional_iterator_tag;
		using value_type = T;
		using difference_type = std::ptrdiff_t;
		using pointer = T *;
		using reference = T &;

		explicit iterator(Node<T> *node) : current(node) {}

		T &operator*() { return current->data; }

		iterator &operator++() {
			current = current->next;
			return *this;
		}

		iterator operator++(int) {
			iterator temp(*this);
			current = current->next;
			return temp;
		}

		iterator &operator--() {
			current = current->prev;
			return *this;
		}

		iterator operator--(int) {
			iterator temp(*this);
			current = current->prev;
			return temp;
		}

		bool operator==(const iterator &other) const { return current == other.current; }
		bool operator!=(const iterator &other) const { return current != other.current; }
	};

	class const_iterator {
	   private:
		friend class List<T>;
		 const Node<T> *current;

	   public:
		using iterator_category = std::bidirectional_iterator_tag;
		using value_type = T;
		using difference_type = std::ptrdiff_t;
		using pointer = const T *;
		using reference = const T &;

		explicit const_iterator(const Node<T> *node) : current(node) {}

		const T &operator*() const { return current->data; }

		const_iterator &operator++() {
			current = current->next;
			return *this;
		}

		const_iterator operator++(int) {
			const_iterator temp(*this);
			current = current->next;
			return temp;
		}

		const_iterator &operator--() {
			current = current->prev;
			return *this;
		}

		const_iterator operator--(int) {
			const_iterator temp(*this);
			current = current->prev;
			return temp;
		}

		bool operator==(const const_iterator &other) const { return current == other.current; }
		bool operator!=(const const_iterator &other) const { return current != other.current; }
	};

	using reverse_iterator = std::reverse_iterator<iterator>;
	using const_reverse_iterator = std::reverse_iterator<const_iterator>;

	List() : count(0) {
		head = new Node<T>();
		tail = new Node<T>(nullptr, nullptr);
		head->next = tail;
		tail->prev = head;
	}

	List(const List &other) : List() {
		for (const auto &elem : other) {
			push_back(elem);
		}
	}

  List(List &&other) noexcept : head(nullptr), tail(nullptr), count(0) {
	  if (other.head) {
	    head = other.head;
	    tail = other.tail;
	    count = other.count;

	    other.head = new Node<T>();
	    other.tail = new Node<T>(nullptr, nullptr);
	    other.head->next = other.tail;
	    other.tail->prev = other.head;
	    other.count = 0;
	  }
	}


	explicit List(std::initializer_list<T> init) : List() {
		for (const auto &elem : init) {
			push_back(elem);
		}
	}

	~List() override {
		clear();
		delete head;
		delete tail;
	}

	List &operator=(const List &other) {
		if (this != &other) {
			clear();
			for (const auto &elem : other) {
				push_back(elem);
			}
		}
		return *this;
	}

	List &operator=(List &&other) noexcept {
		if (this != &other) {
			swap(other);
		}
		return *this;
	}

	T &front() { return head->next->data; }

	const T &front() const { return head->next->data; }

	T &back() { return tail->prev->data; }

	const T &back() const { return tail->prev->data; }

	bool empty() const override { return count == 0; }
	size_t size() const override { return count; }
	size_t max_size() const override { return SIZE_MAX / sizeof(Node<T>); }

	iterator begin() { return iterator(head->next); }

	const_iterator begin() const { return const_iterator(head->next); }

	const_iterator cbegin() const { return const_iterator(head->next); }

	iterator end() { return iterator(tail); }

	const_iterator end() const { return const_iterator(tail); }

	const_iterator cend() const { return const_iterator(tail); }

	reverse_iterator rbegin() { return reverse_iterator(end()); }

	reverse_iterator rend() { return reverse_iterator(begin()); }

	const_reverse_iterator crbegin() const { return const_reverse_iterator(cend()); }

	const_reverse_iterator crend() const { return const_reverse_iterator(cbegin()); }

	void clear() { erase(begin(), end()); }

	void swap(List &other) noexcept {
		std::swap(head, other.head);
		std::swap(tail, other.tail);
		std::swap(count, other.count);
	}

	void resize(size_t new_size, const T &value) {
		if (new_size < count) {
			while (count > new_size) {
				pop_back();
			}
		} else {
			while (count < new_size) {
				push_back(value);
			}
		}
	}
  void resize(size_t new_size){resize(new_size, T{});}
	void push_back(const T &value) { insert(end(), value); }

	void push_back(T &&value) { insert(end(), std::move(value)); }

	void push_front(const T &value) { insert(begin(), value); }

	void push_front(T &&value) { insert(begin(), std::move(value)); }

	void pop_back() { erase(--end()); }

	void pop_front() { erase(begin()); }

	iterator insert(iterator pos, const T &value) {
		Node<T> *pos_node = pos.current;
		Node<T> *newNode = new Node<T>(value, pos_node->prev, pos_node);
		pos_node->prev->next = newNode;
		pos_node->prev = newNode;
		++count;
		return iterator(newNode);
	}

	iterator insert(iterator pos, T &&value) {
		Node<T> *pos_node = pos.current;
		Node<T> *newNode = new Node<T>(std::move(value), pos_node->prev, pos_node);
		pos_node->prev->next = newNode;
		pos_node->prev = newNode;
		++count;
		return iterator(newNode);
	}
  iterator insert(iterator pos, std::initializer_list<T> ilist) {
	  iterator lastInserted = pos;
	  for (const T &value : ilist) {
	    lastInserted = insert(lastInserted, value);
	    ++lastInserted;
	  }
	  return lastInserted;
	}

	iterator erase(iterator pos) {
		Node<T> *toDelete = pos.current;
		iterator ret(toDelete->next);
		toDelete->prev->next = toDelete->next;
		toDelete->next->prev = toDelete->prev;
		delete toDelete;
		--count;
		return ret;
	}

	iterator erase(const_iterator pos) { return erase(iterator(const_cast<Node<T> *>(pos.current))); }

	iterator erase(iterator first, iterator last) {
		while (first != last) {
			first = erase(first);
		}
		return last;
	}

	iterator erase(const_iterator first, const_iterator last) {
		return erase(iterator(const_cast<Node<T> *>(first.current)), iterator(const_cast<Node<T> *>(last.current)));
	}

	bool operator==(const Container<T> &other) const override {
		auto *otherList = dynamic_cast<const List<T> *>(&other);
		if (!otherList) {
			return false;
		}
		if (this->size() != otherList->size()) {
			return false;
		}
		return std::equal(this->begin(), this->end(), otherList->begin());
	}

	bool operator!=(const Container<T> &other) const override { return !(*this == other); }

	bool operator==(const List &other) const { return ((*this <=> other) == std::strong_ordering::equal); }

	bool operator!=(const List &other) const { return !(*this == other); }

	bool operator<(const List &other) const { return ((*this <=> other) == std::strong_ordering::less); }

	bool operator<=(const List &other) const { return (!(*this > other)); }

	bool operator>(const List &other) const { return ((*this <=> other) == std::strong_ordering::greater); }

	bool operator>=(const List &other) const { return (!(*this < other)); }

	auto operator<=>(const List &other) const {
		return std::lexicographical_compare_three_way(this->cbegin(), this->cend(), other.cbegin(), other.cend());
	}
};
}  // namespace my_container

#endif  // LIST_H
