#include "enemy.hpp"

Enemy::Enemy(const Path& p, FILE* from) {
    fread(&m_speed, 1, 1, from);
    EnemyType t;
    fread(&t, 1, 1, from);
    sprite_atlas_pos = SpritesLoc[t];
    follower = PathFollower(p, sprite_atlas_pos.width, sprite_atlas_pos.height);
}
void Enemy::Update(const Path& p) { follower.Update(p, m_speed); }
