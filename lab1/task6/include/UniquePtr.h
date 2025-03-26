#ifndef UNIQUEPTR_H
#define UNIQUEPTR_H
#include <cstddef>
#include <utility>
namespace my_smart_ptr {
template <class T>
class UniquePtr {
   private:
    T* ptr{nullptr};
   public:
	using pointer = T*;
    UniquePtr() noexcept = default;

    explicit UniquePtr(T* p) noexcept : ptr{p} {}

    ~UniquePtr() { delete ptr; }

    UniquePtr(const UniquePtr&) = delete;

    UniquePtr& operator=(const UniquePtr&) = delete;

    UniquePtr(UniquePtr&& other) noexcept : ptr{other.ptr} {
        other.ptr = nullptr;
    }

    UniquePtr& operator=(UniquePtr&& other) noexcept {
        if (this != &other) {
            pointer temp = ptr;
            ptr = other.ptr;
            other.ptr = nullptr;
            delete temp;
        }
        return *this;
    }
    T* get() const noexcept { return ptr; }
    T& operator*() const { return *ptr; }
    T* operator->() const { return ptr; }
    explicit operator bool() const noexcept { return ptr != nullptr; }
    void reset(T* p = nullptr) noexcept {
        if (ptr != p) {
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
template <typename T>
class UniquePtr<T[]> {
   private:
    T* ptr{nullptr};
   public:
	using pointer = T*;
    UniquePtr() noexcept = default;
    explicit UniquePtr(T* p) noexcept : ptr{p} {}
    ~UniquePtr() { delete[] ptr; }
    UniquePtr(const UniquePtr&) = delete;
    UniquePtr& operator=(const UniquePtr&) = delete;
    UniquePtr(UniquePtr&& other) noexcept : ptr{other.ptr} {
        other.ptr = nullptr;
    }
    UniquePtr& operator=(UniquePtr&& other) noexcept {
        if (this != &other) {
            pointer temp = ptr;
            ptr = other.ptr;
            other.ptr = nullptr;
            delete[] temp;
        }
        return *this;
    }
    T* get() const noexcept { return ptr; }
    T& operator[](size_t index) const { return ptr[index]; }
    explicit operator bool() const noexcept { return ptr != nullptr; }
    void reset(T* p = nullptr) noexcept {
        if (ptr != p) {
            delete[] ptr;
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
#endif  // UNIQUEPTR_H