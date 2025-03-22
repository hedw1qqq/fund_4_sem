//
// Created by ivglu on 21.03.2025.
//

#ifndef LIST_H
#define LIST_H
#include <utility>
namespace my_smart_ptr {
template <class T>
class UniquePtr {
   private:
	T* ptr;

   public:
	UniquePtr() { ptr = nullptr; }

	explicit UniquePtr(T* p) : ptr(p) {}

	~UniquePtr() { delete ptr; }

	UniquePtr(const UniquePtr&) = delete;

	UniquePtr& operator=(const UniquePtr&) = delete;

	UniquePtr(UniquePtr&& p) noexcept {
		ptr = p.ptr;
		p.ptr = nullptr;
	}

	UniquePtr& operator=(UniquePtr&& other) noexcept {
		if (this != &other) {
			ptr = other.ptr;
			other.ptr = nullptr;
		}
		return *this;
	}

	T* get() const { return ptr; }

	T& operator*() const { return *ptr; }

	T* operator->() const { return ptr; }

	explicit operator bool() const { return ptr != nullptr; }

	void reset(T* p) {
		if (p != nullptr) {
			delete ptr;
			ptr = p;
		}
	}

	T* release() noexcept {
		T* oldPtr = ptr;
		ptr = nullptr;
		return oldPtr;
	}

	void swap(UniquePtr& other) noexcept { std::swap(ptr, other.ptr); }
};
}  // namespace my_smart_ptr

#endif  // LIST_H