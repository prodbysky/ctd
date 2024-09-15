#pragma once
#include <cstdint>
#include <raylib.h>

constexpr Vector2 WindowSize = {
    .x = 1920,
    .y = 1080,
};

constexpr uint8_t TileSize = 96;

constexpr Rectangle tower_menu_rect = {
    .x      = 0,
    .y      = WindowSize.y - WindowSize.y / 5,
    .width  = WindowSize.x,
    .height = WindowSize.y / 5,
};

constexpr Rectangle play_area_rect = {
    .x      = 0,
    .y      = 0,
    .width  = WindowSize.x,
    .height = WindowSize.y - WindowSize.y / 5,
};

constexpr Rectangle stats_rect = {
    .x      = 0,
    .y      = 0,
    .width  = WindowSize.x / 6,
    .height = WindowSize.y / 6,
};
