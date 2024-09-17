#include "config.hpp"
#include "enemy.hpp"
#include "path.hpp"

#include <cmath>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <raylib.h>
#include <raymath.h>
#include <string>

static size_t health    = 100;
static size_t round_num = 1;
static size_t money     = 100;

const char* ETStr[] = {
    [ET_C]      = "C",
    [ET_CPP]    = "C++",
    [ET_PYTHON] = "Python",
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
    DrawTextEx(font, TextFormat("Health: %d/100", health), {10, 10}, font_size,
               size.x, WHITE);
    DrawTextEx(font, TextFormat("Money: %d", money), {10, 10 + size.y},
               font_size, size.x, WHITE);
    DrawTextEx(font, TextFormat("Round: %d", round_num), {10, 10 + size.y * 2},
               font_size, size.x, WHITE);
}

int game_main() {
    Font font       = LoadFontEx("assets/agave.ttf", 96, nullptr, 250);
    Texture2D atlas = LoadTexture("assets/atlas.png");
    Path p("assets/level1.path");
    FILE* file = fopen("assets/test.enemy", "rb");
    Enemy e(p, file);

    while (!WindowShouldClose()) {
        e.Update(p);
        BeginDrawing();
        ClearBackground(GetColor(0x181818ff));
        DrawRectangleRec(tower_menu_rect, RED);
        // draw_play_area();
        draw_stats(font);
        p.Draw();
        e.Draw(atlas);
        // DrawRectangleRec(e.follower.rect, WHITE);
        EndDrawing();
    }
    CloseWindow();
    return 0;
}

int pather_main() {
    Font font              = LoadFontEx("assets/agave.ttf", 96, nullptr, 250);
    bool editing           = false;
    const size_t font_size = 48;
    Vector2 size           = MeasureTextEx(font, "", font_size, 0);

    while (!editing) {
        BeginDrawing();
        ClearBackground(GetColor(0x181818ff));
        DrawTextEx(font, "To Begin Press 'a'", {10, 10}, font_size, size.x,
                   WHITE);
        if (IsKeyDown(KEY_A)) {
            editing = true;
        }
        EndDrawing();
    }

    Path p;
    int n = 0;

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(GetColor(0x181818ff));
        DrawRectangleLines(tower_menu_rect.x, tower_menu_rect.y,
                           tower_menu_rect.width, tower_menu_rect.height,
                           WHITE);
        draw_play_area();
        if (p.GetPoints().Empty()) {
            DrawTextEx(
                font,
                "Click a point in the grid to mark the beginning of the path",
                {10, tower_menu_rect.y + 10}, font_size, size.x, WHITE);
        } else {
            DrawTextEx(
                font,
                "Press `esc` to save the path (to the cwd with name path.path)",
                {10, tower_menu_rect.y + 10}, font_size, size.x, WHITE);
        }
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            Vector2 mouse_pos = GetMousePosition();
            Vector2 grid_pos  = Vector2Divide(mouse_pos, {TileSize, TileSize});
            grid_pos.x        = floorf(grid_pos.x);
            grid_pos.y        = floorf(grid_pos.y);
            p.Push(grid_pos);
            n++;
        }
        p.Draw();
        EndDrawing();
    }

    CloseWindow();
    std::cout << "Enter the path to save the path to:";
    std::string path;
    std::cin >> path;
    p.Save(path.c_str(), n);
    return 0;
}

int enemy_maker_main() {
    SetTargetFPS(30);
    Font font              = LoadFontEx("assets/agave.ttf", 96, nullptr, 250);
    const size_t font_size = 96;
    Vector2 size_96        = MeasureTextEx(font, "", font_size, 0);
    Vector2 size_36        = MeasureTextEx(font, "", 36, 0);

    uint8_t speed  = 200;
    uint8_t e_type = 0;

    while (!WindowShouldClose()) {
        BeginDrawing();
        DrawTextEx(font, TextFormat("Enemy speed: %d/255", speed), {10, 10},
                   font_size, 0, WHITE);
        DrawTextEx(font, TextFormat("Press `d` to increase speed", speed),
                   {10, 10 + size_96.y}, 36, 0, WHITE);
        DrawTextEx(font, TextFormat("Press `a` to decrease speed", speed),
                   {10, 10 + size_96.y + size_36.y}, 36, 0, WHITE);

        DrawTextEx(font, TextFormat("Enemy type: %s", ETStr[e_type]),
                   {10, 10 + size_96.y + 3 * size_36.y}, font_size, 0, WHITE);

        DrawTextEx(font, "Press `e` to change enemy type",
                   {10, 10 + 2 * size_96.y + 3 * size_36.y}, 36, 0, WHITE);

        if (IsKeyDown(KEY_D)) {
            speed += 1;
        }
        if (IsKeyDown(KEY_A)) {
            speed -= 1;
        }
        if (IsKeyPressed(KEY_E)) {
            e_type++;
            e_type %= ET_COUNT;
        }

        ClearBackground(GetColor(0x181818ff));
        EndDrawing();
    }
    CloseWindow();
    std::cout << "Enter the path to save the enemy to:";
    std::string path;
    std::cin >> path;
    FILE* file = fopen(path.c_str(), "wb");
    fputc(speed, file);
    fputc(e_type, file);
    fclose(file);
    return 0;
}

int main(int argc, char** argv) {
    SetConfigFlags(FLAG_FULLSCREEN_MODE | FLAG_VSYNC_HINT);
    InitWindow(WindowSize.x, WindowSize.y, "Hello world!");

    if (argc == 2) {
        if (strcmp(argv[1], "make_path") == 0) {
            return pather_main();
        }
        if (strcmp(argv[1], "make_enemy") == 0) {
            return enemy_maker_main();
        }
    }
    return game_main();
}
