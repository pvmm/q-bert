#ifndef ENTITIES_H
#define ENTITIES_H

#include <stdint.h>
#include <stdbool.h>

#ifndef LOCAL
#define LOCAL extern
#endif

// max items and enemies on screen at the same time
#define MAX_ENTITIES 11

// player lives
LOCAL uint8_t lives;
// invulnerability time after death
LOCAL uint8_t invuln;
// batteries left
LOCAL uint8_t batteries;
// show some delay once the lives run out
LOCAL uint8_t gameover_delay;

// sprite pattern base offset
enum Pattern {
    QBERT_DOWN_RIGHT    = 0,  // 0b000000 (+3 = 3 sprites)
    QBERT_DOWN_LEFT     = 12, // 0b001100 (+3 = 6 sprites)
    QBERT_UP_RIGHT      = 24, // 0b011000 (+3 = 9 sprites) 
    QBERT_UP_LEFT       = 36, // 0b100100 (+3 = 12 sprites)
    BALL_NORMAL         = 48, //          (+1 = 13 sprites)
    BALL_PRESSED        = 52, //          (+1 = 14 sprites)
    GREENBALL_NORMAL    = 56, //          (+1 = 15 sprites)
    GREENBALL_PRESSED   = 60, //          (+1 = 16 sprites)
    UGG_UP              = 64, //          (+1 = 17 sprites)
    UGG_DOWN            = 68, //          (+1 = 18 sprites)
    WRONG_WAY_UP        = 72, //          (+1 = 19 sprites)
    WRONG_WAY_DOWN      = 76, //          (+1 = 20 sprites)
    COILY_DOWN_RIGHT    = 80, //          (+2 = 22 sprites)
    COILY_DOWN_LEFT     = 88, //          (+2 = 24 sprites)
    COILY_UP_LEFT       = 88, //          (+2 = 26 sprites)
    COILY_UP_RIGHT      = 92, //          (+2 = 28 sprites)
};

#define LEFT_PATTERN_MASK(x)            ((x) & 0b100)

// sprite movement bits
enum Moves {
    MOV_LEFT            =   4,
    MOV_UP              =   1,
    MOV_DOWN            =   2,
    MOV_RIGHT           =   8,
    MOV_DOWN_RIGHT      =  10,      // 2 + 8
    MOV_DOWN_LEFT       =   6,      // 2 + 4
    MOV_UP_RIGHT        =   9,      // 1 + 8
    MOV_UP_LEFT         =   5,      // 1 + 4
};

// how long we wait between animation frames
#define FRAME_WAIT              3

// how long the player is invulnerable after death
// time in frames
#define INVUL_TIME              64

// entities start position
#define ENTITY_START_X1          104
#define ENTITY_START_X2          136
#define ENTITY_START_Y           35

// used by all entities
LOCAL struct sprite_attr sp;

enum entity_id
{
    QBERT = 1,
    BALL1,
    BALL2,
    COILY,
    SAM,
    SLICK,
    WRONG_WAY1,
    WRONG_WAY2,
    UGG1,
    UGG2,
    GREENBALL,
};

struct entity
{
    enum entity_id type;
    uint8_t tile_x;                 // pressed plate position
    uint8_t tile_y;
    uint8_t x;
    uint8_t y;
    uint8_t x0;                     // previous x,y positions
    uint8_t y0;
    // uint8_t dir;                 // 4 directions
    bool active;
    uint8_t pattern;                // pattern group
    // uint8_t delay;
    uint8_t frame;                  // current animation frame
    uint8_t pos;

    void (*update)(struct entity* entity);
};

// fixed entity of the player
LOCAL struct entity qbert;

// other entities
LOCAL struct entity entities[MAX_ENTITIES];

void init_entities(void);
void update_entity(struct entity* entity);
void update_entity_left(struct entity* entity);
void update_entity_right(struct entity* entity);
void update_entity_up(struct entity* entity);
void update_entity_down(struct entity* entity);

void init_player(void);
void update_entity(struct entity* entity);
void move_left(struct entity* entity);
void move_right(struct entity* entity);
void move_up(struct entity* entity);
void move_down(struct entity* entity);

void put_entity_sprite(struct entity* entity);

#ifdef LOCAL
#undef LOCAL
#endif

#endif // ENTITIES_H
