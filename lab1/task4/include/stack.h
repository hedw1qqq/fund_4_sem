#ifndef STACK_H
#define STACK_H

#include <initializer_list>
#include <utility>

#include "../include/deque.h"

namespace my_container {

template <typename T>
class Stack {
   public:
	using value_type = T;
	using container_type = Deque<T>;
	using size_type = size_t;
	using reference = T&;
	using const_reference = const T&;

   private:
	container_type container_;

   public:
	Stack() = default;

	Stack(const Stack& other) : container_(other.container_) {}

	Stack(Stack&& other) noexcept : container_(std::move(other.container_)) {}

	explicit Stack(std::initializer_list<T> init) : container_(init) {}

	~Stack() = default;

	Stack& operator=(const Stack& other) {
		if (this != &other) {
			container_ = other.container_;
		}
		return *this;
	}

	Stack& operator=(Stack&& other) noexcept {
		if (this != &other) {
			container_ = std::move(other.container_);
		}
		return *this;
	}

	reference top() { return container_.back(); }
	const_reference top() const { return container_.back(); }

	bool empty() const { return container_.empty(); }

	size_type size() const { return container_.size(); }

	void push(const T& value) { container_.push_back(value); }

	void push(T&& value) { container_.push_back(std::move(value)); }

	void pop() { container_.pop_back(); }
	void swap(Stack& other) noexcept { container_.swap(other.container_); }

	friend bool operator==(const Stack& lhs, const Stack& rhs) { return lhs.container_ == rhs.container_; }

	friend bool operator!=(const Stack& lhs, const Stack& rhs) { return !(lhs == rhs); }

	friend bool operator<(const Stack& lhs, const Stack& rhs) { return lhs.container_ < rhs.container_; }

	friend bool operator<=(const Stack& lhs, const Stack& rhs) { return lhs.container_ <= rhs.container_; }

	friend bool operator>(const Stack& lhs, const Stack& rhs) { return lhs.container_ > rhs.container_; }

	friend bool operator>=(const Stack& lhs, const Stack& rhs) { return lhs.container_ >= rhs.container_; }

	auto operator<=>(const Stack& other) const { return container_ <=> other.container_; }
};

}  // namespace my_container

#endif  // STACK_H