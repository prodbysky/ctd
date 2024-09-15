#pragma once

#include <cstdio>
#include <vector>

struct Vector2;

class Path {
    std::vector<Vector2> points;

public:
    Path() = default;
    Path(const char* file_name);
    void Push(Vector2 point);
    void Draw() const;
    const std::vector<Vector2>& GetPoints() const;
    void Save(const char* out, size_t n);

    const Vector2& Beginning() const;
};
