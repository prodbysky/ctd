#pragma once

#include <cassert>
#include <cstddef>
#include <cstdlib>
#include <initializer_list>
template <class T> class DynArray {
public:
    DynArray<T>() {
        elements = malloc(sizeof(T) * default_init_cap);
        cap      = default_init_cap;
        used     = 0;
    }
    DynArray<T>(size_t cap) {
        elements  = malloc(sizeof(T) * cap);
        this->cap = cap;
        used      = 0;
    }
    DynArray<T>(std::initializer_list<T> elements) {
        elements = malloc(sizeof(T) * elements.size());
        cap      = elements.size() * 2;
        used     = 0;

        for (const T e : elements) {
            elements[used++] = e;
        }
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
    bool Empty() { return used == 0; }
    bool Full() { return used == cap; };
    // Resize the array, (cap = size)
    void ReserveAbs(size_t size) {
        assert(size > cap);
        cap      = size;
        elements = realloc(elements, size);
    }
    // Resize the array, (cap += size)
    void Reserve(size_t size) {
        cap      += size;
        elements  = realloc(elements, cap);
    }

    T* Begin() { return &elements[0]; }
    T* End() { return &elements[cap]; }
    size_t Len() { return cap; }

    T At(size_t idx) {
        assert(cap < idx);
        return elements[idx];
    }
    T& AtRef(size_t idx) {
        assert(cap < idx);
        return &elements[idx];
    }

private:
    T* elements;
    size_t cap;
    size_t used;

    static constexpr size_t default_init_cap = 16;
};
