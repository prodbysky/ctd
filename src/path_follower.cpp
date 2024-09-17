#include "path_follower.hpp"

#include <raymath.h>

PathFollower::PathFollower(const Path& p, float w, float h) :

    finished(false), iter(p.GetPoints().Begin()) {
    rect = {
        .x = iter->x - w / 2, .y = iter->y - h / 2, .width = w, .height = h};
    target    = *(++iter);
    target.x -= w / 2;
    target.y -= h / 2;
    move_dir  = Vector2Normalize(Vector2Subtract(target, Pos()));
}

Vector2 PathFollower::Pos() { return {.x = rect.x, .y = rect.y}; }

void PathFollower::NextTarget(const Path& p) {
    if (iter + 1 != p.GetPoints().End()) {
        target    = *(++iter);
        target.x -= rect.width / 2;
        target.y -= rect.height / 2;
        move_dir  = Vector2Normalize(Vector2Subtract(target, Pos()));
        return;
    }
    finished = true;
}

void PathFollower::Update(const Path& p, float speed) {
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
