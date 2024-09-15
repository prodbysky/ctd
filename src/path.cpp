#include "path.hpp"

#include "config.hpp"

#include <raymath.h>
#include <vector>

Path::Path(const char* file_name) {
    FILE* file = fopen(file_name, "rb");

    size_t n = fgetc(file);
    points   = std::vector<Vector2>();

    for (; n > 0; n--) {
        size_t x = (fgetc(file) * TileSize) + TileSize / 2.0;
        size_t y = (fgetc(file) * TileSize) + TileSize / 2.0;
        points.push_back({static_cast<float>(x), static_cast<float>(y)});
    }
    fclose(file);
}

void Path::Push(Vector2 point) {
    points.emplace_back(Vector2Add(Vector2Multiply(point, {TileSize, TileSize}),
                                   {TileSize / 2.0, TileSize / 2.0}));
}

void Path::Draw() const {
    for (size_t i = 0; i < points.size(); i++) {
        if (i + 1 != points.size()) {
            DrawLineV(points[i], points[i + 1], WHITE);
        }
    }
}

const Vector2& Path::Beginning() const { return points[0]; }

const std::vector<Vector2>& Path::GetPoints() const { return points; }
