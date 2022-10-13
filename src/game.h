#ifndef _GAME_H
#define _GAME_H

#include "debug.h"
#include <stdint.h>

#ifndef LOCAL
#define LOCAL extern
#endif

enum game_status
{
    PLAYING = 0,
    LEVEL_COMPLETED,
    LEVEL_COMPLETED_2,
};

// game state machine
LOCAL enum game_status game_status;

/**
 * Game implementation and helpers.
 */

// map size in tiles
#define MAP_W 32
#define MAP_H 24

#define MAX_LIVES 3

void run_game();
void draw_map();
void draw_hud();

// used to read our control method
LOCAL uint8_t control;

// current map; pointer in ROM
LOCAL const uint8_t *cur_map;
// current map tile map (map data, not entities)
// we don't use ROM because we will modify it
LOCAL uint8_t cur_map_data[MAP_W * MAP_H];

#ifdef LOCAL
#undef LOCAL
#endif

#endif // _GAME_H
