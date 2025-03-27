#ifndef DEQUE_H
#define DEQUE_H

#include <iterator>
#include <stdexcept>

#include "List.h"

namespace my_container {

template <typename T>
class Deque : public List<T> {
   public:
	using List<T>::List;

	T &at(size_t pos) {
		if (pos >= this->size()) {
			throw std::out_of_range("Deque: index out of range");
		}
		return (*this)[pos];
	}

	const T &at(size_t pos) const {
		if (pos >= this->size()) {
			throw std::out_of_range("Deque: index out of range");
		}
		return (*this)[pos];
	}

	T &operator[](size_t pos) {
		auto iter = this->begin();
		for (size_t i = 0; i < pos; ++i) {
			++iter;
		}
		return *iter;
	}

	const T &operator[](size_t pos) const {
		auto iter = this->begin();
		for (size_t i = 0; i < pos; ++i) {
			++iter;
		}
		return *iter;
	}
};

}  // namespace my_container

#endif  // DEQUE_H
