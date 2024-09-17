#pragma once

#include "path.hpp"

#include <raylib.h>
class PathFollower {
public:
    PathFollower() = default;
    PathFollower(const Path& p, float w, float h);
    Vector2 Pos();
    void NextTarget(const Path& p);
    void Update(const Path& p, float speed);

public:
    Rectangle rect;
    bool finished;

private:
    Vector2 move_dir;
    Vector2 target;
    Vector2* iter;
};
