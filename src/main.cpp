#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <iostream>
#include <memory>
#include <raylib.h>
#include <raymath.h>
#include <vector>

constexpr Vector2 WindowSize = {
    .x = 1920,
    .y = 1080,
};

constexpr uint8_t TileSize = 96;

constexpr Rectangle tower_menu_rect = {
    .x      = 0,
    .y      = WindowSize.y - WindowSize.y / 5,
    .width  = WindowSize.x,
    .height = WindowSize.y / 5,
};

constexpr Rectangle play_area_rect = {
    .x      = 0,
    .y      = 0,
    .width  = WindowSize.x,
    .height = WindowSize.y - WindowSize.y / 5,
};

constexpr Rectangle stats_rect = {
    .x      = 0,
    .y      = 0,
    .width  = WindowSize.x / 6,
    .height = WindowSize.y / 6,
};

void draw_play_area() {
    for (int y = 0; y < play_area_rect.height / TileSize; y++) {
        for (int x = 0; x < play_area_rect.width / TileSize; x++) {
            DrawRectangleLines(x * TileSize, y * TileSize, TileSize, TileSize,
                               BROWN);
        }
    }
}

void draw_stats(Font font) {
    const size_t font_size = 48;
    Vector2 size           = MeasureTextEx(font, "", font_size, 0);
    DrawTextEx(font, "Health: ", {10, 10}, font_size, size.x, WHITE);
    DrawTextEx(font, "Money: ", {10, 10 + size.y}, font_size, size.x, WHITE);
    DrawTextEx(font, "Round: ", {10, 10 + size.y * 2}, font_size, size.x,
               WHITE);
}

class Path {
    std::vector<Vector2> points;

public:
    Path() = default;
    Path(const char* file_name) {
        points     = std::vector<Vector2>();
        FILE* file = fopen(file_name, "rb");

        size_t n = fgetc(file);

        for (; n > 0; n--) {
            size_t x = (fgetc(file) * TileSize) + TileSize / 2.0;
            size_t y = (fgetc(file) * TileSize) + TileSize / 2.0;
            points.push_back({static_cast<float>(x), static_cast<float>(y)});
        }
        fclose(file);
    }

    void Push(Vector2 point) {
        points.emplace_back(
            Vector2Add(Vector2Multiply(point, {TileSize, TileSize}),
                       {TileSize / 2.0, TileSize / 2.0}));
    }

    void Draw() {
        for (size_t i = 0; i < points.size(); i++) {
            if (i + 1 != points.size()) {
                DrawLineV(points[i], points[i + 1], WHITE);
            }
        }
    }
};

int main() {
    SetConfigFlags(FLAG_FULLSCREEN_MODE);
    SetConfigFlags(FLAG_VSYNC_HINT);
    InitWindow(WindowSize.x, WindowSize.y, "Hello world!");
    Font font = LoadFontEx("assets/agave.ttf", 96, nullptr, 250);
    Path p("assets/level1.path");

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(GetColor(0x181818ff));
        DrawRectangleRec(tower_menu_rect, RED);
        draw_play_area();
        draw_stats(font);
        p.Draw();
        EndDrawing();
    }
    CloseWindow();
}
