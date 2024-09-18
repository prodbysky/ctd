#pragma once

#include <cassert>
#include <cstddef>
#include <cstdlib>
#include <cstring>
#include <initializer_list>

template <class T> class DynArray {
public:
    DynArray() {
        elements = (T*) malloc(sizeof(T) * default_init_cap);
        cap      = default_init_cap;
        used     = 0;
    }
    ~DynArray() { free(elements); }
    DynArray(size_t cap) {
        elements  = (T*) malloc(sizeof(T) * cap);
        this->cap = cap;
        used      = 0;
    }
    DynArray(std::initializer_list<T> init_list) {
        elements = (T*) malloc(sizeof(T) * init_list.size());
        cap      = init_list.size() * 2;
        used     = 0;

        for (const T& e : init_list) {
            elements[used++] = e;
        }
    }
    DynArray(const DynArray<T>& other) {
        elements = (T*) malloc(sizeof(T) * other.cap);
        cap      = other.cap;
        used     = other.used;
        memcpy(elements, other.elements, other.used * sizeof(T));
        // std::copy(other.Begin(), other.Begin() + other.used, elements);
    }

    // Assignment operator
    DynArray& operator=(const DynArray<T>& other) {
        if (this != &other) {
            free(elements);
            elements = (T*) malloc(sizeof(T) * other.cap);
            cap      = other.cap;
            used     = other.used;
            memcpy(elements, other.elements, other.used * sizeof(T));
            // std::copy(other.Begin(), other.Begin() + other.used, elements);
        }
        return *this;
    }
    void Push(T element) {
        if (used + 1 == cap) {
            Reserve(cap * 2);
        }
        elements[used++] = element;
    }
    T Pop() {
        assert(used);
        return elements[used--];
    }
    bool Empty() const { return used == 0; }
    bool Full() const { return used == cap; };
    // Resize the array, (cap = size)
    void ReserveAbs(size_t size) {
        assert(size > cap);
        cap      = size;
        elements = (T*) realloc(elements, size * sizeof(T));
    }
    // Resize the array, (cap += size)
    void Reserve(size_t size) {
        cap      += size;
        elements  = (T*) realloc(elements, cap * sizeof(T));
    }

    T* Begin() const { return &elements[0]; }
    T* End() const { return &elements[used]; }
    const T* BeginConst() const { return &elements[0]; }
    const T* EndConst() const { return &elements[used]; }
    size_t Len() const { return used; }

    T At(size_t idx) const {
        assert(cap > idx);
        return elements[idx];
    }
    T& AtRef(size_t idx) {
        assert(cap > idx);
        return &elements[idx];
    }

private:
    T* elements;
    size_t cap;
    size_t used;

    static constexpr size_t default_init_cap = 16;
};
