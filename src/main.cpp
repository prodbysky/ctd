#include "config.hpp"
#include "path.hpp"

#include <cstddef>
#include <cstdio>
#include <raylib.h>
#include <raymath.h>

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

int main() {
    SetConfigFlags(FLAG_FULLSCREEN_MODE | FLAG_VSYNC_HINT);
    InitWindow(WindowSize.x, WindowSize.y, "Hello world!");
    Font font = LoadFontEx("assets/agave.ttf", 96, nullptr, 250);
    Path p("assets/level1.path");
    Enemy e(p);

    while (!WindowShouldClose()) {
        e.Update(p);
        BeginDrawing();
        ClearBackground(GetColor(0x181818ff));
        DrawRectangleRec(tower_menu_rect, RED);
        draw_play_area();
        draw_stats(font);
        p.Draw();
        DrawCircleV(e.pos, 10, WHITE);
        EndDrawing();
    }
    CloseWindow();
}
