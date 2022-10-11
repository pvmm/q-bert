#include <string.h>

#define LOCAL
#include "entities.h"

#include "game.h"
#include "palette.h"
#include "lookup_tables.h"
#include "level.h"
#include "spman.h"

// generated
#include "player.h"
#include "enemy.h"


const int8_t (*jump_table)[16];

extern enum game_status game_status;    // completed the plates?
extern uint8_t control;
uint8_t old_pattern;
uint8_t row;                            // player vertical coordinate


void init_entities()
{
    const uint8_t *m = cur_map;
    // uint8_t typ, last = 0;
    // uint16_t i;

    // this sets everything to 0, which is useful as
    // entity ET_UNUSED is 0
    memset(entities, 0, sizeof(struct entity) * MAX_ENTITIES);

    // get to the beginning of the entities:
    // map size + 3 bytes of header (the map size and the entities size)
    // m += (uint16_t)(m[0] | m[1] << 8) + 3;

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


void update_enemy(void)
{
    // check for the player; if alive and not invulnerable!
    // we use small hit boxes
    // if (lives && !invuln
    //         && entities[0].x + 6 < self->x + 10 && self->x + 6 < entities[0].x + 10
    //         && self->y == entities[0].y)
    // {
    //     // change direction
    //     self->dir ^= 1;
    //
    //     // remove one life (is more like "hits")
    //     lives--;
    //     draw_hud();
    //     invuln = INVUL_TIME;
    //
    //     if (!lives)
    //     {
    //         // different sound effects if is game over
    //         mplayer_init(SONG, SONG_SILENCE);
    //         mplayer_play_effect_p(EFX_DEAD, EFX_CHAN_NO, 0);
    //         gameover_delay = GAMEOVER_DELAY;
    //     }
    //     else
    //         mplayer_play_effect_p(EFX_HIT, EFX_CHAN_NO, 0);
    // }
    //
    // // left or right?
    // if (self->dir)
    // {
    //     // change direction
    //     if (self->x == 2 || is_map_blocked(self->x, self->y + 15))
    //         self->dir ^= 1;
    //     else
    //         self->x -= 1;
    // }
    // else
    // {
    //     // change direction
    //     if (self->x == 255 - 16 || is_map_blocked(self->x + 15, self->y + 15))
    //         self->dir ^= 1;
    //     else
    //         self->x += 1;
    // }
    //
    // // update the walking animation
    // if (self->delay++ == FRAME_WAIT)
    // {
    //     self->delay = 0;
    //     if (++self->frame == WALK_CYCLE)
    //         self->frame = 0;
    // }
    //
    // // allocate the sprites
    // sp.x = self->x;
    // // y on the screen starts in 255
    // sp.y = self->y - 1;
    // // find which pattern to show
    // sp.pattern = self->pat + (walk_frames[self->frame] + self->dir * 3) * 4;
    // // red
    // sp.attr = 9;
    // spman_alloc_sprite(&sp);
}


void init_player()
{
    // fill sprite allocation table up
    spman_alloc_pat(SPRITE_DOWN_RIGHT, *player_sprite, 4, 0);
    spman_alloc_pat(SPRITE_DOWN_LEFT , *player_sprite, 4, true);
    spman_alloc_pat(SPRITE_UP_RIGHT  , *(player_sprite + (sizeof(player_sprite)/32/2)), 4, 0);
    spman_alloc_pat(SPRITE_UP_LEFT   , *(player_sprite + (sizeof(player_sprite)/32/2)), 4, true);

    row = 1;
    qbert.dirty = true;
    qbert.pos = 1;
    qbert.frame = 0;
    qbert.x0 = 122;
    qbert.y0 = 8;
    qbert.x = 122;
    qbert.y = 8;
    qbert.tile_x = 14;
    qbert.tile_y = 2;
    qbert.pattern = SPRITE_DOWN_RIGHT;
    old_pattern = qbert.pattern;
    qbert.update = update_player;
}


void update_player()
{
    old_pattern = qbert.pattern;

    switch (control)
    {
        default:
        case 4: // left
        case 1: // up
        case 2: // down
        case 8: // right
            break;

        case MOV_DOWN_RIGHT:
            //debug_msg("down-right");
            qbert.pattern = SPRITE_DOWN_RIGHT;
            jump_table = &jump_down;
            qbert.update = update_player_right;
            break;

        case MOV_DOWN_LEFT:
            //debug_msg("down-left");
            qbert.pattern = SPRITE_DOWN_LEFT;
            jump_table = &jump_down;
            qbert.update = update_player_left;
            break;

        case MOV_UP_RIGHT:
            //debug_msg("up-right");
            qbert.pattern = SPRITE_UP_RIGHT;
            jump_table = &jump_up;
            qbert.update = update_player_right;
            break;

        case MOV_UP_LEFT:
            //debug_msg("up-left");
            qbert.pattern = SPRITE_UP_LEFT;
            jump_table = &jump_up;
            qbert.update = update_player_left;
            break;
    }

    put_qbert_sprite();
}


void update_player_left()
{
    put_qbert_sprite();

    if (qbert.frame < 16)
    {
        qbert.y = qbert.y0 + (*jump_table)[qbert.frame];
        qbert.frame++;
        qbert.x = qbert.x0 - qbert.frame;
    }
    else
    {
        //qbert.x--;
        qbert.tile_x -= 2;

        if (qbert.y0 < qbert.y)
        {
            // going down
            qbert.tile_y += 3;
            qbert.pos += row++;
        }
        else
        {
            // going up
            qbert.tile_y -= 3;
            qbert.pos -= row--;
        }

        game_status = press_plate(qbert.pos, qbert.tile_x, qbert.tile_y);

        // reset animation status
        qbert.x0 = qbert.x;
        qbert.y0 = qbert.y;
        qbert.frame = 0;
        qbert.update = update_player;
    }
}


void update_player_right()
{
    put_qbert_sprite();

    if (qbert.frame < 16)
    {
        qbert.y = qbert.y0 + (*jump_table)[qbert.frame];
        qbert.frame++;
        qbert.x = qbert.x0 + qbert.frame;
    }
    else
    {
        //qbert.x--;
        qbert.tile_x += 2;

        if (qbert.y0 < qbert.y)
        {
            // going down
            qbert.tile_y += 3;
            qbert.pos += ++row;
        }
        else
        {
            // going up
            qbert.tile_y -= 3;
            qbert.pos -= --row;
        }

        game_status = press_plate(qbert.pos, qbert.tile_x, qbert.tile_y);

        // reset animation status
        qbert.x0 = qbert.x;
        qbert.y0 = qbert.y;
        qbert.frame = 0;
        qbert.update = update_player;
    }
}


void put_qbert_sprite()
{
    struct sprite_attr sa;
    static uint8_t fast_x;
    static uint8_t fast_y;

    fast_x = LEFT_PATTERN_MASK(qbert.pattern) ? qbert.x - 3 : qbert.x;
    fast_y = qbert.y - 1;

    //
    // Update sprite atributes
    //

    sa.y = fast_y;
    sa.x = fast_x;

    // eyes and mouth
    sa.attr = BG_BLACK;
    sa.pattern = qbert.pattern;
    spman_alloc_sprite(&sa);

    // the bod (always visible)
    sa.attr = BG_DARKYELLOW;
    sa.pattern = qbert.pattern + 4;
    spman_alloc_sprite(&sa);

    // face
    //sa.attr = BG_LIGHTYELLOW;
    //sa.pattern = qbert.pattern + 8;
    //spman_alloc_sprite(&sa);

    // sclera (white of the eyes)
    sa.pattern = qbert.pattern + 8;
    sa.attr = BG_WHITE;
    spman_alloc_fixed_sprite(&sa);
}
