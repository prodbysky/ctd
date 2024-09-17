#include "enemy.hpp"

#include <raylib.h>
#include <raymath.h>

const static Rectangle SpritesLoc[] = {
    [ET_C]      = {.x = 0,   .y = 0, .width = 96, .height = 96},
    [ET_CPP]    = {.x = 96,  .y = 0, .width = 96, .height = 96},
    [ET_PYTHON] = {.x = 192, .y = 0, .width = 96, .height = 96},
};

Enemy::Enemy(const Path& p, FILE* from) {
    fread(&m_speed, 1, 1, from);
    EnemyType t;
    fread(&t, 1, 1, from);
    sprite_atlas_pos = SpritesLoc[t];
    follower = PathFollower(p, sprite_atlas_pos.width, sprite_atlas_pos.height);
}
void Enemy::Update(const Path& p) { follower.Update(p, m_speed); }

void Enemy::Draw(const Texture2D& atlas) const {
    DrawTexturePro(atlas, sprite_atlas_pos, follower.rect, Vector2Zero(), 0,
                   WHITE);
}
