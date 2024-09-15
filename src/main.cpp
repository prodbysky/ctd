#include "config.hpp"
#include "path.hpp"

#include <cmath>
#include <cstddef>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <raylib.h>
#include <raymath.h>

static size_t health    = 100;
static size_t round_num = 1;
static size_t money     = 100;

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
    DrawTextEx(font, TextFormat("Health: 100/%d", health), {10, 10}, font_size,
               size.x, WHITE);
    DrawTextEx(font, TextFormat("Money: %d", money), {10, 10 + size.y},
               font_size, size.x, WHITE);
    DrawTextEx(font, TextFormat("Round: %d", round_num), {10, 10 + size.y * 2},
               font_size, size.x, WHITE);
}

struct Enemy {
public:
    Vector2 pos;

private:
    Vector2 target;
    float speed;
    size_t n_points;
    size_t curr_target_i;
    bool finished;

public:
    Enemy(const Path& p) {
        curr_target_i = 1;
        finished      = false;
        n_points      = p.GetPoints().size();
        pos           = p.Beginning();
        target        = p.GetPoints()[curr_target_i];
        speed         = 200;
    }

    void NextTarget(const Path& p) {
        if (curr_target_i + 1 != n_points) {
            target = p.GetPoints()[++curr_target_i];
            return;
        }
        finished = true;
    }

    void Update(const Path& p) {
        if (finished) {
            return;
        }
        bool y   = false;
        bool x   = false;
        float dt = GetFrameTime();
        if (target.x - pos.x == 0)
            y = true;
        if (target.y - pos.y == 0)
            x = true;

        if (x) {
            if (pos.x < target.x) {
                pos.x += speed * dt;
                pos.x  = Clamp(pos.x, pos.x, target.x);
                if (pos.x == target.x) {
                    NextTarget(p);
                    return;
                }
            } else {
                pos.x -= speed * dt;

                pos.x = Clamp(pos.x, target.x, pos.x);
                if (pos.x == target.x) {
                    NextTarget(p);
                    return;
                }
            }
        } else {
            if (pos.y < target.y) {
                pos.y += speed * dt;
                pos.y  = Clamp(pos.y, pos.y, target.y);
                if (pos.y == target.y) {
                    NextTarget(p);
                    return;
                }
            } else {
                pos.y -= speed * dt;
                pos.y  = Clamp(pos.y, target.y, pos.y);
                if (pos.y == target.y) {
                    NextTarget(p);
                    return;
                }
            }
        }
    }
};

int game_main() {
    Font font = LoadFontEx("assets/agave.ttf", 96, nullptr, 250);
    Path p("assets/test_path.path");
    Enemy e(p);

    while (!WindowShouldClose()) {
        e.Update(p);
        BeginDrawing();
        ClearBackground(GetColor(0x181818ff));
        DrawRectangleRec(tower_menu_rect, RED);
        // draw_play_area();
        draw_stats(font);
        p.Draw();
        DrawCircleV(e.pos, 10, WHITE);
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
        if (p.GetPoints().empty()) {
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
    p.Save("path.path", n);
    CloseWindow();
    return 0;
}

int main(int argc, char** argv) {
    SetConfigFlags(FLAG_FULLSCREEN_MODE | FLAG_VSYNC_HINT);
    InitWindow(WindowSize.x, WindowSize.y, "Hello world!");

    if (argc == 2) {
        if (strcmp(argv[1], "pather") == 0) {
            return pather_main();
        }
    }
    return game_main();
}
