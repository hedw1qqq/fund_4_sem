//
// Created by ivglu on 21.03.2025.
//

#ifndef LIST_H
#define LIST_H
#include "Container.h"
#include <algorithm>
#include <memory>
#include <stdexcept>
#include <compare>

namespace my_container {
template <typename T>
class Vector final : public Container<T> {
   private:
	T *arr{};

   public:
	using value_type = T;
	using reference = T &;
	using const_reference = const T &;
	using pointer = T *;
	using const_pointer = const T *;
	using iterator = T *;
	using const_iterator = const T *;
	using reverse_iterator = std::reverse_iterator<iterator>;
	using const_reverse_iterator = std::reverse_iterator<const_iterator>;
	using size_type = std::size_t;
	using difference_type = std::ptrdiff_t;

   private:
	size_type len;
	size_type cap;

   public:
	Vector() {
		this->len = 0;
		this->cap = 0;
		this->arr = nullptr;
	}

	Vector(const Vector &other) {
		this->len = other.len;
		this->cap = other.cap;
		this->arr = new T[this->cap];
		std::copy_n(other.arr, this->len, this->arr);
	}

	~Vector() override {
		delete[] this->arr;
		this->len = 0;
		this->cap = 0;
	}

	Vector(Vector &&other) noexcept {
		this->len = other.len;
		this->cap = other.cap;
		this->arr = other.arr;
		other.arr = nullptr;
		other.len = 0;
		other.cap = 0;
	}

	Vector(std::initializer_list<T> init) {
		this->len = init.size();
		this->cap = this->len;
		this->arr = new T[this->cap];
		std::copy(init.begin(), init.end(), this->arr);
	}

	Vector &operator=(const Vector &other) {
		if (this != &other) {
			delete[] this->arr;
			this->len = other.len;
			this->cap = other.cap;
			this->arr = new T[this->cap];
			std::copy_n(other.arr, this->len, this->arr);
		}
		return *this;
	}

	Vector &operator=(Vector &&other) noexcept {
		if (this != &other) {
			delete[] this->arr;
			this->len = other.len;
			this->cap = other.cap;
			this->arr = other.arr;
			other.arr = nullptr;
			other.len = 0;
			other.cap = 0;
		}
		return *this;
	}

	reference operator[](size_type index) { return this->arr[index]; }

	const_reference operator[](size_type index) const { return this->arr[index]; }

	reference at(size_type index) {
		if (index >= this->len) {
			throw std::out_of_range("index out of range");
		}
		return this->arr[index];
	}
	const_reference at(size_type index) const {
		if (index >= this->len) {
			throw std::out_of_range("index out of range");
		}
		return this->arr[index];
	}

	bool empty() const override { return this->len == 0; }

	reference front() {
		if (empty()) {
			throw std::out_of_range("vector is empty");
		}

		return this->arr[0];
	}
	const_reference front() const {
		if (empty()) {
			throw std::out_of_range("vector is empty");
		}

		return this->arr[0];
	}

	reference back() {
		if (empty()) {
			throw std::out_of_range("vector is empty");
		}
		return this->arr[this->len - 1];
	}

	const_reference back() const {
		if (empty()) {
			throw std::out_of_range("vector is empty");
		}
		return this->arr[this->len - 1];
	}

	T *data() { return this->arr; }
	const T *data() const { return this->arr; }

	size_type size() const override { return this->len; }

	size_type max_size() const override {
		return this->len;
	}

	size_type capacity() const { return this->cap; }

	void reserve(size_type new_cap) {
		if (new_cap > this->cap) {
			T *tmp = new T[new_cap];

			std::move(this->arr, this->arr + this->len, tmp);

			delete[] this->arr;

			this->arr = tmp;
			this->cap = new_cap;
		}
	}

	void shrink_to_fit() {
		if (this->len < this->cap) {
			if (this->len == 0) {
				delete[] this->arr;
				this->arr = nullptr;
				this->cap = 0;
			} else {
				T *tmp = new T[this->len];

				std::move(this->arr, this->arr + this->len, tmp);

				delete[] this->arr;

				this->arr = tmp;
				this->cap = this->len;
			}
		}
	}
	void clear() noexcept {
		std::destroy(this->arr, this->arr + this->len);

		this->len = 0;
	}
	void push_back(const T &elem) {
		if (this->len == this->cap) {
			size_type new_cap = this->cap == 0 ? 1 : this->cap * 2;
			T *tmp = new T[new_cap];
			std::move(this->arr, this->arr + this->len, tmp);
			delete[] this->arr;
			this->arr = tmp;
			this->cap = new_cap;
		}
		this->arr[this->len++] = elem;
	}

	void push_back(T &&elem) {
		if (this->len == this->cap) {
			size_type new_cap = this->cap == 0 ? 1 : this->cap * 2;
			T *tmp = new T[new_cap];
			std::move(this->arr, this->arr + this->len, tmp);
			delete[] this->arr;
			this->arr = tmp;
			this->cap = new_cap;
		}
		this->arr[this->len++] = std::move(elem);
	}

	void pop_back() {
		if (empty()) {
			throw std::out_of_range("vector is empty");
		}
		this->len--;
		this->arr[this->len].~T();
	}

	void insert(size_type index, const T &elem) {
		if (index > this->len) {
			throw std::out_of_range("index out of range");
		}

		if (this->len == this->cap) {
			size_type new_cap = this->cap == 0 ? 1 : this->cap * 2;
			T *tmp = new T[new_cap];


			std::move(this->arr, this->arr + index, tmp);

			tmp[index] = elem;

			std::move(this->arr + index, this->arr + this->len, tmp + index + 1);

			delete[] this->arr;
			this->arr = tmp;
			this->cap = new_cap;
		} else {

			for (size_type i = this->len; i > index; --i) {
				this->arr[i] = std::move(this->arr[i - 1]);
			}


			this->arr[index] = elem;
		}

		this->len++;
	}

	void erase(size_type index) {
		if (index >= this->len) {
			throw std::out_of_range("index out of range");
		}


		for (size_type i = index; i < this->len - 1; ++i) {
			this->arr[i] = std::move(this->arr[i + 1]);
		}


		this->arr[--this->len].~T();
	}

	void resize(size_type new_size) {
		resize(new_size, T());
	}

	void resize(size_type new_size, const T &value) {
		if (new_size > this->len) {
			if (new_size > this->cap) {
				reserve(new_size);
			}

			std::fill(this->arr + this->len, this->arr + new_size, value);
		} else if (new_size < this->len) {
			std::destroy(this->arr + new_size, this->arr + this->len);
		}

		this->len = new_size;
	}

	void swap(Vector &other) noexcept {
		std::swap(this->arr, other.arr);
		std::swap(this->len, other.len);
		std::swap(this->cap, other.cap);
	}


	bool operator==(const Vector &other) const {
		if (this->len != other.len) {
			return false;
		}

		for (size_type i = 0; i < this->len; ++i) {
			if (!(this->arr[i] == other.arr[i])) {
				return false;
			}
		}

		return true;
	}

	bool operator!=(const Vector &other) const {
		return !(*this == other);
	}

	bool operator<(const Vector &other) const {
		return std::lexicographical_compare(
			this->arr, this->arr + this->len,
			other.arr, other.arr + other.len
		);
	}

	bool operator<=(const Vector &other) const {
		return !(other < *this);
	}

	bool operator>(const Vector &other) const {
		return other < *this;
	}

	bool operator>=(const Vector &other) const {
		return !(*this < other);
	}


	bool operator==(const Container<T> &other) const override {

		const Vector *otherVector = dynamic_cast<const Vector *>(&other);
		if (otherVector == nullptr) {
			return false;
		}

		return *this == *otherVector;
	}

	bool operator!=(const Container<T> &other) const override {
		return !(*this == other);
	}


	auto operator<=>(const Vector &other) const {
		return std::lexicographical_compare_three_way(
			this->arr, this->arr + this->len,
			other.arr, other.arr + other.len
		);
	}
};
}  // namespace my_container

#endif  // LIST_H