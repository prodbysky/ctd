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

    const Vector2& Beginning() const;
};
