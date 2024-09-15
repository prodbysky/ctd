#include <cstdint>
#include <raylib.h>

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

int main() {
    SetConfigFlags(FLAG_FULLSCREEN_MODE);
    InitWindow(WindowSize.x, WindowSize.y, "Hello world!");
    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(GetColor(0x181818ff));
        DrawRectangleRec(tower_menu_rect, RED);
        draw_play_area();
        DrawRectangleRec(stats_rect, BLUE);
        EndDrawing();
    }
    CloseWindow();
}
