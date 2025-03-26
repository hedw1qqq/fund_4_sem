//
// Created by ivglu on 13.03.2025.
//

#ifndef LAB1_ARRAY_H
#define LAB1_ARRAY_H
#include "Container.h"
#include <compare>
#include <iterator>
#include <algorithm>
#include <stdexcept>
#include <utility>
#include <initializer_list>

namespace my_container {
    template<typename T, std::size_t N>
    class Array final : public Container<T> {
    private:
        T elems[N]{};

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

        Array() = default;

        ~Array() = default;

        explicit Array(std::initializer_list<T> init) {
            size_type count = std::min(init.size(), N);
            std::copy_n(init.begin(), count, elems);
            if (count < N) {
                std::fill_n(elems + count, N - count, T{});
            }
        }

        Array(const Array &other) {
            std::copy_n(other.elems, N, elems);
        }

        Array(Array &&other) noexcept {
            std::move(other.begin(), other.end(), elems);
        }

        Array &operator=(const Array &other) {
            if (this != &other) {
                std::copy_n(other.elems, N, elems);
            }
            return *this;
        }

        Array &operator=(Array &&other) noexcept {
            if (this != &other) {
                std::move(other.begin(), other.end(), elems);
            }
            return *this;
        }

        reference at(size_type pos) {
            if (pos >= N) {
                throw std::out_of_range("Array index out of range: " + std::to_string(pos) +
                                      ", max allowed: " + std::to_string(N - 1));
            }
            return elems[pos];
        }

        const_reference at(size_type pos) const {
            if (pos >= N) {
                throw std::out_of_range("Array index out of range: " + std::to_string(pos) +
                                      ", max allowed: " + std::to_string(N - 1));
            }
            return elems[pos];
        }

        reference operator[](size_type pos) { return elems[pos]; }

        const_reference operator[](size_type pos) const { return elems[pos]; }

        reference front() { return elems[0]; }

        const_reference front() const { return elems[0]; }

        reference back() { return elems[N - 1]; }

        const_reference back() const { return elems[N - 1]; }

        pointer data() noexcept { return elems; }

        const_pointer data() const noexcept { return elems; }

        iterator begin() { return elems; }

        const_iterator cbegin() const { return elems; }

        iterator end() { return elems + N; }

        const_iterator cend() const { return elems + N; }

        reverse_iterator rbegin() { return reverse_iterator(end()); }

        const_reverse_iterator crbegin() const { return const_reverse_iterator(cend()); }

        reverse_iterator rend() { return reverse_iterator(begin()); }

        const_reverse_iterator crend() const { return const_reverse_iterator(cbegin()); }

        bool empty() const override { return N == 0; }

        size_type size() const override { return N; }

        size_type max_size() const override { return N; }

        void fill(const T &val) { std::fill_n(elems, N, val); }

        void swap(Array &other) noexcept { std::swap_ranges(begin(), end(), other.begin()); }

        bool operator==(const Array &other) const {
            return (*this <=> other) == std::strong_ordering::equal;
        }

        bool operator!=(const Array &other) const { return !(*this == other); }

        bool operator<(const Array &other) const {
            return (*this <=> other) == std::strong_ordering::less;
        }

        bool operator<=(const Array &other) const {
            return !(*this > other);
        }

        bool operator>(const Array &other) const {
            return (*this <=> other) == std::strong_ordering::greater;
        }

        bool operator>=(const Array &other) const {
            return !(*this < other);
        }

        auto operator<=>(const Array &rhs) const {
            return std::lexicographical_compare_three_way(cbegin(), cend(), rhs.cbegin(), rhs.cend());
        }
    };
} // namespace my_container

#endif  // LAB1_ARRAY_H
