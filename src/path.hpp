#pragma once

#include "dyn_array.hpp"

#include <cstdio>

struct Vector2;

class Path {
    DynArray<Vector2> points;

public:
    Path() = default;
    Path(const char* file_name);
    void Push(Vector2 point);
    void Draw() const;
    const DynArray<Vector2>& GetPoints() const;
    void Save(const char* out, size_t n);
    Vector2 Beginning() const;
};
