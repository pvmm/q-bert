#include <stdint.h>
#include <string.h>

#include "ubox.h"
#include "spman.h"
#include "mplayer.h"
#include "ap.h"

#include "helpers.h"
#include "main.h"

#define LOCAL
#include "entities.h"
#include "game.h"
#include "sprites.h"
#include "level.h"

// generated
#include "map.h"
#include "player.h"
#include "enemy.h"

#define BG_TILE_MAP 0x1800

// current entity
struct entity* self;

// game still running?
uint8_t completed_delay;
uint8_t completed;
#define COMPLETED_DELAY_IN_FRAMES   20

// Q*bert's rainbow-colored frisbees
uint8_t frisbees;
uint8_t frisbee_frame;
uint8_t frisbee_anim_delay;
#define FRISBEE_MAX_DELAY           35
#define FRISBEE_FRAME_DURATION      10
#define FRISBEE_TILE1               193
#define FRISBEE_TILE2               194
#define FRISBEE_NUM_TILES           2
#define FRISBEE_MAX_TILES           (4 * 2)


void init_map_entities()
{
    const uint8_t *m = cur_map;
    // uint8_t typ, last = 0;
    // uint16_t i;

    // init sprite and patterns
    spman_init();

    // this sets everything to 0, which is useful as
    // entity ET_UNUSED is 0
    memset(entities, 0, sizeof(struct entity) * MAX_ENTITIES);

    // get to the beginning of the entities:
    // map size + 3 bytes of header (the map size and the entities size)
    // m += (uint16_t)(m[0] | m[1] << 8) + 3;

    // fill sprite allocation table up
    spman_alloc_pat(SPRITE_DOWN_RIGHT   , sprites[0][0], 4, 0);
    spman_alloc_pat(SPRITE_DOWN_LEFT    , sprites[0][0], 4, 1);
    spman_alloc_pat(SPRITE_UP_RIGHT     , sprites[1][0], 4, 0);
    spman_alloc_pat(SPRITE_UP_LEFT      , sprites[1][0], 4, 1);

    // the entity list ends with 255
    // while (*m != 0xff)
    // {
    //     // first byte is type + direction flag on MSB
    //     // remove MSB
    //     typ = m[0] & (~DIR_FLAG);
    //
    //     entities[last].type = typ;
    //     entities[last].x = m[1];
    //     entities[last].y = m[2];
    //     // in the map: param is 1 (int) to look left
    //     entities[last].dir = m[0] & DIR_FLAG ? DIR_LEFT : DIR_RIGHT;
    //
    //     switch (typ)
    //     {
    //         // can be only one; always first entity
    //         // because our entities are sorted by type!
    //         case ET_PLAYER:
    //             // 3 frames x 2 sprites = 6
    //             entities[last].pat = spman_alloc_pat(PAT_PLAYER, player_sprite[0], 6, 0);
    //             spman_alloc_pat(PAT_PLAYER_FLIP, player_sprite[0], 6, 1);
    //             entities[last].update = update_player;
    //             break;
    //
    //         case ET_ENEMY:
    //             // 3 frames
    //             entities[last].pat = spman_alloc_pat(PAT_ENEMY, enemy_sprite[0], 3, 0);
    //             spman_alloc_pat(PAT_ENEMY_FLIP, enemy_sprite[0], 3, 1);
    //             entities[last].update = update_enemy;
    //             break;
    //     }
    //
    //     // next entity
    //     last++;
    //
    //     // all our entities are 3 bytes
    //     m += 3;
    // }

    // // count how many batteries are in the map
    // batteries = 0;
    // for (i = 0; i < MAP_W * MAP_H; ++i)
    //     if (cur_map_data[i] == BATTERY_TILE)
    //         batteries++;
}

void draw_map()
{
    // draw the map!
    //
    //  - is not compressed (which limits how many maps we can store)
    //  - our map is just the tile numbers
    //
    // so we do it in one call by coping our map to the backtround tile map
    // addr in the VDP VRAM
    ubox_wait_vsync();
    ubox_write_vm((uint8_t *)BG_TILE_MAP, MAP_W * MAP_H, cur_map_data);
}

void draw_hud()
{
    uint8_t i;

    // "Player 1"
    ubox_put_tile(0, 1, 19);
    ubox_put_tile(1, 1, 20);
    ubox_put_tile(2, 1, 21);
    ubox_put_tile(3, 1, 22);
    ubox_put_tile(4, 1, 23);
    ubox_put_tile(5, 1, 24);
    ubox_put_tile(7, 0, 25);
    ubox_put_tile(7, 1, 26);

    // "change to"
    ubox_put_tile(24, 1, 50);
    ubox_put_tile(25, 1, 51);
    ubox_put_tile(26, 1, 52);
    ubox_put_tile(27, 1, 53);
    ubox_put_tile(28, 1, 54);
    ubox_put_tile(29, 1, 55);
    ubox_put_tile(30, 1, 56);
    ubox_put_tile(25, 2, 57);
    ubox_put_tile(26, 2, 57);
    ubox_put_tile(27, 2, 58);
    ubox_put_tile(28, 2, 59);
    ubox_put_tile(29, 2, 59);

    for (i = 0; i < MAX_LIVES; ++i)
        if (i < lives)
            // our hearts tile
            ubox_put_tile(2 + i, 2, 192);
        else
            ubox_put_tile(2 + i, 2, WHITESPACE_TILE);
}


inline void update_frisbees()
{
    frisbee_anim_delay--;
    if (frisbees && frisbee_anim_delay == 0)
    {
        frisbee_anim_delay = FRISBEE_MAX_DELAY;
        ubox_put_tile(5, 12, FRISBEE_TILE1 + frisbee_frame);
        ubox_put_tile(6, 12, FRISBEE_TILE2 + frisbee_frame);
        ubox_put_tile(25, 12, FRISBEE_TILE1 + frisbee_frame);
        ubox_put_tile(26, 12, FRISBEE_TILE2 + frisbee_frame);
        frisbee_frame = (frisbee_frame < FRISBEE_MAX_TILES - FRISBEE_NUM_TILES) ? frisbee_frame + FRISBEE_NUM_TILES : 0;
    }
}


#include "set_tiles_colors_at.h"
void set_plates_colors(uint8_t color)
{
    uint8_t colors[96]; // 96 = 12 * 8

    for (uint8_t i = 0; i < sizeof(colors); ++i)
    {
        colors[i] = color;
    }

    set_tiles_colors_at(colors, 1800, sizeof(colors));
}


void run_game()
{
    uint8_t i;

    // init some variables; look at game.h for description
    frisbees = 4;
    frisbee_anim_delay = 1;
    frisbee_frame = 0;
    lives = MAX_LIVES;
    invuln = 0;
    gameover_delay = 0;
    completed = 0;
    completed_delay = COMPLETED_DELAY_IN_FRAMES;
    control = 0;

    ubox_disable_screen();

    ubox_fill_screen(WHITESPACE_TILE);

    // we only have one map, select it
    cur_map = map[0];
    // uncompress map data into RAM, we will modify it
    // map data starts on byte 3 (skip map data size and entities size)
    ap_uncompress(cur_map_data, cur_map + 3);

    // init entities before drawing
    init_player();
    init_map_entities();
    init_level();
    draw_map();

    draw_hud();

    ubox_enable_screen();

    mplayer_init(SONG, SONG_IN_GAME);

    // our game loop
    while (1)
    {
        // game completed!
        if (completed > 0) {
            if (--completed_delay == 0) {
                completed_delay = COMPLETED_DELAY_IN_FRAMES;
            }
            // done?
            if (completed == 0xf0)
                continue;
            set_plates_colors(completed += 0x10);
            break;
        }

        // exit the game
        if (ubox_read_keys(7) == UBOX_MSX_KEY_ESC)
            break;

        // we are in the gameover delay
        if (gameover_delay)
        {
            // if finished, exit
            if (--gameover_delay == 0)
                break;
        }

        // read the selected control
        control = ubox_read_ctl(ctl);
        qbert.update();

        // update all the entities:
        // - self is a pointer to THIS entity
        // - because we don't create/destroy entities dynamically
        //   when we found one that is unused we are done
        for (i = 0, self = entities; i < MAX_ENTITIES && self->type; i++, self++)
            self->update();

        // ensure we wait to our desired update rate
        ubox_wait();

        //
        update_frisbees();

        // update sprites on screen
        spman_update();
    }

    // stop the in game music
    mplayer_init(SONG, SONG_SILENCE);
    // hide all the sprites before going back to the menu
    spman_hide_all_sprites();
}
