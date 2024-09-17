#include "path.hpp"

#include "config.hpp"
#include "dyn_array.hpp"

#include <cstdio>
#include <iostream>
#include <raymath.h>

Path::Path(const char* file_name) {
    FILE* file = fopen(file_name, "rb");

    size_t n = fgetc(file);
    points   = DynArray<Vector2>();

    for (; n > 0; n--) {
        size_t x = (fgetc(file) * TileSize) + TileSize / 2.0;
        size_t y = (fgetc(file) * TileSize) + TileSize / 2.0;
        points.Push({static_cast<float>(x), static_cast<float>(y)});
    }
    fclose(file);
}

void Path::Push(Vector2 point) {
    points.Push(Vector2Add(Vector2Multiply(point, {TileSize, TileSize}),
                           {TileSize / 2.0, TileSize / 2.0}));
}

void Path::Draw() const {
    for (size_t i = 0; i < points.Len(); i++) {
        if (i + 1 != points.Len()) {
            DrawLineV(points.At(i), points.At(i + 1), WHITE);
        }
    }
}

Vector2 Path::Beginning() const { return *points.Begin(); }

const DynArray<Vector2>& Path::GetPoints() const { return points; }

void Path::Save(const char* out, size_t n) {
    FILE* file = fopen(out, "wb");
    fputc(n, file);

    for (int i = 0; i < n; i++) {
        const Vector2& point = points.At(i);
        uint8_t x            = (point.x - (TileSize / 2.0)) / TileSize;
        uint8_t y            = (point.y - (TileSize / 2.0)) / TileSize;
        fputc(x, file);
        fputc(y, file);
    }
    fclose(file);
}
