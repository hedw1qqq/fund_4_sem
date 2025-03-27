#ifndef VECTOR_H
#define VECTOR_H

#include <algorithm>
#include <compare>
#include <initializer_list>
#include <limits>
#include <stdexcept>

#include "container.h"

namespace my_container {

template <typename T>
class Vector final : public Container<T> {
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
	pointer arr;
	size_type len;
	size_type cap;

   public:
	Vector() : arr(nullptr), len(0), cap(0) {}

	Vector(const Vector &other) : len(other.len), cap(other.cap) {
		arr = new T[cap];
		std::copy_n(other.arr, len, arr);
	}

	Vector(Vector &&other) noexcept : arr(other.arr), len(other.len), cap(other.cap) {
		other.arr = nullptr;
		other.len = 0;
		other.cap = 0;
	}

	explicit Vector(std::initializer_list<T> init) : len(init.size()), cap(init.size()) {
		arr = new T[cap];
		std::copy(init.begin(), init.end(), arr);
	}

	~Vector() override { delete[] arr; }

	Vector &operator=(const Vector &other) {
		if (this != &other) {
			Vector temp(other);
			swap(temp);
		}
		return *this;
	}

	Vector &operator=(Vector &&other) noexcept {
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

	reference front() { return arr[0]; }
	const_reference front() const { return arr[0]; }

	reference back() { return arr[len - 1]; }
	const_reference back() const { return arr[len - 1]; }

	pointer data() noexcept { return arr; }
	const_pointer data() const noexcept { return arr; }

	bool empty() const override { return len == 0; }
	size_type size() const override { return len; }
	size_type max_size() const override { return std::numeric_limits<size_type>::max() / sizeof(T); }
	size_type capacity() const { return cap; }

	void reserve(size_type new_cap) {
		if (new_cap > cap) {
			T *tmp = new T[new_cap];
			std::move(arr, arr + len, tmp);
			delete[] arr;
			arr = tmp;
			cap = new_cap;
		}
	}

	void shrink_to_fit() {
		if (cap > len) {
			T *tmp = (len > 0) ? new T[len] : nullptr;
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

	iterator insert(const_iterator pos, const T &value) {
		size_type index = pos - cbegin();
		if (len == cap) reserve(cap == 0 ? 1 : cap * 2);
		std::move_backward(begin() + index, end(), end() + 1);
		arr[index] = value;
		++len;
		return begin() + index;
	}

	iterator insert(const_iterator pos, T &&value) {
		size_type index = pos - cbegin();
		if (len == cap) reserve(cap == 0 ? 1 : cap * 2);
		std::move_backward(begin() + index, end(), end() + 1);
		arr[index] = std::move(value);
		++len;
		return begin() + index;
	}

	iterator insert(const_iterator pos, const size_type &count, const T &value) {
		size_type index = pos - cbegin();
		if (len + count > cap) reserve(len + count);
		std::move_backward(begin() + index, end(), end() + count);
		std::fill(begin() + index, begin() + index + count, value);
		len += count;
		return begin() + index;
	}

	iterator insert(const_iterator pos, std::initializer_list<T> ilist) {
		size_type index = pos - cbegin();
		size_type count = ilist.size();
		if (len + count > cap) reserve(len + count);
		std::move_backward(begin() + index, end(), end() + count);
		std::copy(ilist.begin(), ilist.end(), begin() + index);
		len += count;
		return begin() + index;
	}

	iterator erase(const_iterator pos) {
		size_type index = pos - cbegin();
		std::move(begin() + index + 1, end(), begin() + index);
		--len;
		return begin() + index;
	}

	iterator erase(const_iterator first, const_iterator last) {
		size_type start = first - cbegin();
		size_type endPos = last - cbegin();
		std::move(begin() + endPos, end(), begin() + start);
		len -= (endPos - start);
		return begin() + start;
	}

	void clear() noexcept { len = 0; }

	void push_back(const T &elem) { insert(end(), elem); }

	void push_back(T &&elem) { insert(end(), std::move(elem)); }

	void pop_back() { --len; }

	void resize(const size_type &new_size) { resize(new_size, T()); }
	void resize(const size_type &new_size, const T &value) {
		if (new_size > len) {
			if (new_size > cap) reserve(new_size);
			for (size_type i = len; i < new_size; ++i) {
				arr[i] = value;
			}
		}
		len = new_size;
	}

	bool operator==(const Container<T> &other) const override {
		auto otherVec = dynamic_cast<const Vector<T> *>(&other);
		if (!otherVec) return false;
		return *this == *otherVec;
	}
	bool operator!=(const Container<T> &other) const override { return !(*this == other); }

	bool operator==(const Vector &other) const { return *this <=> other == std::strong_ordering::equal; }
	bool operator!=(const Vector &other) const { return !(*this == other); }

	auto operator<=>(const Vector &other) const {
		return std::lexicographical_compare_three_way(arr, arr + len, other.arr, other.arr + other.len);
	}
	bool operator<(const Vector &other) const { return (*this <=> other) == std::strong_ordering::less; }
	bool operator<=(const Vector &other) const { return !(*this > other); }
	bool operator>(const Vector &other) const { return (*this <=> other) == std::strong_ordering::greater; }
	bool operator>=(const Vector &other) const { return !(*this < other); }

	void swap(Vector &other) noexcept {
		std::swap(arr, other.arr);
		std::swap(len, other.len);
		std::swap(cap, other.cap);
	}
};

}  // namespace my_container

#endif
