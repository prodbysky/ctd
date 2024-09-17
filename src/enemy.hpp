#pragma once

#include "path.hpp"
#include "path_follower.hpp"

#include <cstdint>
#include <raylib.h>

enum EnemyType : uint8_t {
    ET_C      = 0,
    ET_CPP    = 1,
    ET_PYTHON = 2,
    ET_COUNT,
};

const static Rectangle SpritesLoc[] = {
    [ET_C]      = {.x = 0,   .y = 0, .width = 96, .height = 96},
    [ET_CPP]    = {.x = 96,  .y = 0, .width = 96, .height = 96},
    [ET_PYTHON] = {.x = 192, .y = 0, .width = 96, .height = 96},
};

class Enemy {
public:
    Enemy() = default;
    Enemy(const Path& p, FILE* from);
    void Update(const Path& p);

public:
    PathFollower follower;

private:
    uint8_t m_speed;
    Rectangle sprite_atlas_pos;
};
