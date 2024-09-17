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

class Enemy {
public:
    Enemy() = default;
    Enemy(const Path& p, FILE* from);
    void Update(const Path& p);
    void Draw(const Texture2D& atlas) const;

public:
    PathFollower follower;

private:
    uint8_t m_speed;
    Rectangle sprite_atlas_pos;
};
