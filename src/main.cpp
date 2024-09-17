#include "config.hpp"
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
    DrawTextEx(font, TextFormat("Health: %d/100", health), {10, 10}, font_size,
               size.x, WHITE);
    DrawTextEx(font, TextFormat("Money: %d", money), {10, 10 + size.y},
               font_size, size.x, WHITE);
    DrawTextEx(font, TextFormat("Round: %d", round_num), {10, 10 + size.y * 2},
               font_size, size.x, WHITE);
}

class PathFollower {
public:
    PathFollower() = default;
    PathFollower(const Path& p, float w, float h) :
        finished(false), iter(p.GetPoints().begin()) {
        rect      = {.x      = iter->x - w / 2,
                     .y      = iter->y - h / 2,
                     .width  = w,
                     .height = h};
        target    = *(++iter);
        target.x -= w / 2;
        target.y -= h / 2;
        move_dir  = Vector2Normalize(Vector2Subtract(target, Pos()));
    }

    Vector2 Pos() { return {.x = rect.x, .y = rect.y}; }

    void NextTarget(const Path& p) {
        if (iter < p.GetPoints().end()) {
            target    = *(++iter);
            target.x -= rect.width / 2;
            target.y -= rect.height / 2;
            move_dir  = Vector2Normalize(Vector2Subtract(target, Pos()));
            return;
        }
        finished = true;
    }

    void Update(const Path& p, float speed) {
        if (finished) {
            return;
        }
        const float dt = GetFrameTime();

        Vector2 diff = Vector2Add(Pos(), Vector2Scale(move_dir, speed * dt));
        rect.x       = diff.x;
        rect.y       = diff.y;
        if (Vector2Distance(diff, target) < speed * dt) {
            NextTarget(p);
        }
    }

public:
    Rectangle rect;
    bool finished;

private:
    Vector2 move_dir;
    Vector2 target;
    std::vector<Vector2>::const_iterator iter;
};

// speed, type
// Speed: uint8_t
// Type: uint8_t
// Where Type:
// 0x00 -> C
// 0x01 -> C++
// 0x02 -> Python
// ...

enum EnemyType : uint8_t {
    ET_C      = 0,
    ET_CPP    = 1,
    ET_PYTHON = 2,
};

const static Rectangle SpritesLoc[] = {
    [ET_C]      = {.x = 0,   .y = 0, .width = 96, .height = 96},
    [ET_CPP]    = {.x = 96,  .y = 0, .width = 96, .height = 96},
    [ET_PYTHON] = {.x = 192, .y = 0, .width = 96, .height = 96},
};

class Enemy {
public:
    Enemy() = default;
    Enemy(const Path& p, FILE* from) {
        fread(&m_speed, 1, 1, from);
        EnemyType t;
        fread(&t, 1, 1, from);
        sprite_atlas_pos = SpritesLoc[t];
        follower =
            PathFollower(p, sprite_atlas_pos.width, sprite_atlas_pos.height);
    }
    void Update(const Path& p) { follower.Update(p, m_speed); }

public:
    PathFollower follower;

private:
    uint8_t m_speed;
    Rectangle sprite_atlas_pos;
};

int game_main() {
    Font font = LoadFontEx("assets/agave.ttf", 96, nullptr, 250);
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
        DrawRectangleRec(e.follower.rect, WHITE);
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
