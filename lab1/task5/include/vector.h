//
// Created by ivglu on 21.03.2025.
//
#ifndef LIST_H
#define LIST_H
#include <algorithm>
#include <compare>
#include <limits>
#include <memory>
#include <stdexcept>

#include "container.h"

namespace my_container {
template <typename T>
class vector final : public Container<T> {
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
	vector() {
		len = 0;
		cap = 0;
		arr = nullptr;
	}

	vector(const vector &other) {
		len = other.len;
		cap = other.cap;
		arr = new T[cap];
		std::copy_n(other.arr, len, arr);
	}

	~vector() override {
		std::destroy(begin(), end());
		delete[] arr;
	}

	vector(vector &&other) noexcept {
		len = other.len;
		cap = other.cap;
		arr = other.arr;
		other.arr = nullptr;
		other.len = 0;
		other.cap = 0;
	}

	explicit vector(std::initializer_list<T> init) {
		len = init.size();
		cap = len;
		arr = new T[cap];
		std::copy(init.begin(), init.end(), arr);
	}

	vector &operator=(const vector &other) {
		if (this != &other) {
			delete[] arr;
			len = other.len;
			cap = other.cap;
			arr = new T[cap];
			std::copy_n(other.arr, len, arr);
		}
		return *this;
	}
	vector &operator=(vector &&other) noexcept {
		if (this != &other) {
			delete[] arr;
			arr = other.arr;
			len = other.len;
			cap = other.cap;
			other.arr = nullptr;
			other.len = 0;
			other.cap = 0;
		}
		return *this;
	}

	reference operator[](size_type index) { return arr[index]; }

	const_reference operator[](size_type index) const { return arr[index]; }

	reference at(size_type index) {
		if (index >= len) throw std::out_of_range("index out of range");
		return arr[index];
	}

	const_reference at(size_type index) const {
		if (index >= len) throw std::out_of_range("index out of range");
		return arr[index];
	}

	// Добавлены методы front(), back() и data()
	reference front() {
		if (empty()) throw std::out_of_range("vector is empty");
		return arr[0];
	}

	const_reference front() const {
		if (empty()) throw std::out_of_range("vector is empty");
		return arr[0];
	}

	reference back() {
		if (empty()) throw std::out_of_range("vector is empty");
		return arr[len - 1];
	}

	const_reference back() const {
		if (empty()) throw std::out_of_range("vector is empty");
		return arr[len - 1];
	}

	pointer data() noexcept { return arr; }

	const_pointer data() const noexcept { return arr; }

	bool empty() const override { return len == 0; }

	size_type size() const override { return len; }

	// Исправлен метод max_size()
	size_type max_size() const override { return std::numeric_limits<size_type>::max() / sizeof(T); }

	size_type capacity() const { return cap; }

	// Добавлен метод shrink_to_fit()
	void shrink_to_fit() {
		if (cap > len) {
			T* tmp = len ? new T[len] : nullptr;
			std::move(arr, arr + len, tmp);
			delete[] arr;
			arr = tmp;
			cap = len;
		}
	}

	iterator begin() { return arr; }

	const_iterator begin() const { return arr; }

	const_iterator cbegin() const { return arr; }

	iterator end() { return arr + len; }

	const_iterator end() const { return arr + len; }

	const_iterator cend() const { return arr + len; }

	reverse_iterator rbegin() { return reverse_iterator(end()); }

	const_reverse_iterator rbegin() const { return const_reverse_iterator(end()); }

	const_reverse_iterator crbegin() const { return const_reverse_iterator(cend()); }

	reverse_iterator rend() { return reverse_iterator(begin()); }

	const_reverse_iterator rend() const { return const_reverse_iterator(begin()); }

	const_reverse_iterator crend() const { return const_reverse_iterator(cbegin()); }

	void reserve(size_type new_cap) {
		if (new_cap > cap) {
			T *tmp = new T[new_cap];
			std::move(arr, arr + len, tmp);
			delete[] arr;
			arr = tmp;
			cap = new_cap;
		}
	}

	void push_back(const T &elem) {
		if (len == cap) {
			reserve(cap == 0 ? 1 : cap * 2);
		}
		arr[len++] = elem;
	}

	void push_back(T &&elem) {
		if (len == cap) {
			reserve(cap == 0 ? 1 : cap * 2);
		}
		arr[len++] = std::move(elem);
	}

	void pop_back() {
		--len;
	}

	void resize(const size_type &new_size) { resize(new_size, T()); }

	void resize(const size_type &new_size, const T &value) {
		if (new_size < len) {
			for (size_type i = new_size; i < len; ++i) {
				arr[i].~T();
			}
		} else if (new_size > len) {
			if (new_size > cap) {
				reserve(new_size);
			}
			for (size_type i = len; i < new_size; ++i) {
				new (&arr[i]) T(value);
			}
		}
		len = new_size;
	}

	void swap(vector &other) noexcept {
		std::swap(arr, other.arr);
		std::swap(len, other.len);
		std::swap(cap, other.cap);
	}

	iterator insert(const_iterator pos, const T &value) {
		size_type index = pos - cbegin();
		if (len == cap) {
			reserve(cap == 0 ? 1 : cap * 2);
		}
		std::move_backward(begin() + index, end(), end() + 1);
		arr[index] = value;
		++len;
		return begin() + index;
	}

	iterator insert(const_iterator pos, T &&value) {
		size_type index = pos - cbegin();
		if (len == cap) {
			reserve(cap == 0 ? 1 : cap * 2);
		}
		std::move_backward(begin() + index, end(), end() + 1);
		arr[index] = std::move(value);
		++len;
		return begin() + index;
	}

	iterator insert(const_iterator pos, const size_type &count, const T &value) {
		size_type index = pos - cbegin();
		if (len + count > cap) {
			reserve(len + count);
		}
		std::move_backward(begin() + index, end(), end() + count);
		std::fill(begin() + index, begin() + index + count, value);
		len += count;
		return begin() + index;
	}

	iterator insert(const_iterator pos, std::initializer_list<T> ilist) {
		size_type index = pos - cbegin();
		size_type count = ilist.size();
		if (len + count > cap) {
			reserve(len + count);
		}
		std::move_backward(begin() + index, end(), end() + count);
		std::copy(ilist.begin(), ilist.end(), begin() + index);
		len += count;
		return begin() + index;
	}

	iterator erase(const_iterator pos) {
		size_type index = pos - cbegin();
		std::move(begin() + index + 1, end(), begin() + index);
		arr[--len].~T();
		return begin() + index;
	}

	iterator erase(const_iterator first, const_iterator last) {
		size_type start = first - cbegin();
		size_type endPos = last - cbegin();
		std::move(begin() + endPos, end(), begin() + start);
		len -= (endPos - start);
		return begin() + start;
	}

	void clear() noexcept {
		std::destroy(arr, arr + len);
		len = 0;
	}



	bool operator==(const vector &other) const {
		return (*this <=> other) == std::strong_ordering::equal;
	}

	bool operator!=(const vector &other) const { return !(*this == other); }

	auto operator<=>(const vector &other) const {
		return std::lexicographical_compare_three_way(arr, arr + len, other.arr, other.arr + other.len);
	}

	bool operator<(const vector &other) const { return (*this <=> other) == std::strong_ordering::less; }

	bool operator<=(const vector &other) const { return !(*this > other); }

	bool operator>(const vector &other) const { return (*this <=> other) == std::strong_ordering::greater; }

	bool operator>=(const vector &other) const {return !(*this < other); }
};
}  // namespace my_container

#endif  // LIST_H