#ifndef _GAME_H
#define _GAME_H

#include "debug.h"
#include <stdint.h>

enum game_status
{
    PLAYING = 0,
    LEVEL_COMPLETED,
    SECOND_CYCLE,
};

#ifndef LOCAL
#define LOCAL extern
#else

#define WALK_CYCLE 4

// walk animation frames, private
const uint8_t walk_frames[WALK_CYCLE] = { 0, 1, 0, 2 };

#endif

/**
 * Game implementation and helpers.
 */

// map size in tiles
#define MAP_W 32
#define MAP_H 24

// some useful tiles
#define LAST_SOLID_TILE     10
#define BATTERY_TILE        224

#define MAX_LIVES 3

// MSB in our entity data
#define DIR_FLAG 128

// show some game frames before jumping to the game over
// screen; this helps the player to know what happened
#define GAMEOVER_DELAY 72

void run_game();

void draw_map();
void draw_hud();

//void erase_battery(uint8_t x, uint8_t y);

uint8_t is_map_blocked(uint8_t x, uint8_t y);
uint8_t is_map_elevator_up(uint8_t x, uint8_t y);
uint8_t is_map_elevator_down(uint8_t x, uint8_t y);

// used to read our control method
LOCAL uint8_t control;

// current map; pointer in ROM
LOCAL const uint8_t *cur_map;
// current map tile map (map data, not entities)
// we don't use ROM because we will modify it
LOCAL uint8_t cur_map_data[MAP_W * MAP_H];

// player lives
LOCAL uint8_t lives;
// invulnerability time after death
LOCAL uint8_t invuln;
// batteries left
LOCAL uint8_t batteries;
// show some delay once the lives run out
LOCAL uint8_t gameover_delay;

#ifdef LOCAL
#undef LOCAL
#endif

#endif // _GAME_H
