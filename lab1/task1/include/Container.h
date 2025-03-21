#ifndef LAB1_CONTAINER_H
#define LAB1_CONTAINER_H

#include <cstddef>

template <typename T>
class Container {
public:
	using value_type = T;
	using reference = T&;
	using const_reference = const T&;
	using pointer = T*;
	using const_pointer = const T*;
	using size_type = std::size_t;
	using difference_type = std::ptrdiff_t;

	Container() = default;
	Container(const Container<T>& other) = default;
	Container<T>& operator=(const Container<T>& other) = default;
	virtual ~Container() = default;

	virtual bool operator==(const Container<T>& other) const = 0;
	virtual bool operator!=(const Container<T>& other) const = 0;

	virtual size_type size() const = 0;
	virtual size_type max_size() const = 0;
	virtual bool empty() const = 0;
};

#endif  // LAB1_CONTAINER_H