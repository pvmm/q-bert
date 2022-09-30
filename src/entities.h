#ifndef ENTITIES_H
#define ENTITIES_H

#include <stdint.h>
#include <stdbool.h>

#ifndef LOCAL
#define LOCAL extern
#endif

// max items and enemies on screen at the same time
#define MAX_ENTITIES 5

// sprite pattern base offset
enum Pattern {
    SPRITE_DOWN_RIGHT = 0,
    SPRITE_DOWN_LEFT,
    SPRITE_UP_RIGHT,
    SPRITE_UP_LEFT,
};

#define DIR_LEFT_MASK(x)            ((x) & 0b100)

// sprite movement bits
enum Moves {
    MOV_LEFT            =  16,
    MOV_UP              =  32,
    MOV_DOWN            =  64,
    MOV_RIGHT           = 128,
    MOV_DOWN_RIGHT      = 192,      // 128 + 64
    MOV_DOWN_LEFT       =  80,      //  64 + 16
    MOV_UP_RIGHT        = 160,      // 128 + 32
    MOV_UP_LEFT         =  48,      //  32 + 16
};

// how long we wait between animation frames
#define FRAME_WAIT 3

// how long the player is invulnerable after death
// time in frames
#define INVUL_TIME 64

// used by all entities
LOCAL struct sprite_attr sp;

struct entity
{
    uint8_t type;
    uint8_t tile_x;                 // pressed plate position
    uint8_t tile_y;
    uint8_t x;
    uint8_t y;
    uint8_t x0;                     // previous x,y positions
    uint8_t y0;
    // uint8_t dir;                    // 4 directions
    bool dirty;
    uint8_t pattern;                // pattern group
    // uint8_t delay;
    uint8_t frame;                  // current animation frame
    uint8_t pos;

    void (*update)(void);
};

// fixed entity of the player
LOCAL struct entity qbert;

// other entities
LOCAL struct entity entities[MAX_ENTITIES];

void update_enemy(void);

void init_player(void);

void update_player(void);
void update_player_left(void);
void update_player_right(void);

void put_qbert_sprite();

#endif // ENTITIES_H
