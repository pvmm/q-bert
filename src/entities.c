#include "entities.h"
#include "palette.h"
#include "lookup_tables.h"
#include "level.h"
#include "spman.h"


const int8_t (*jump_table)[16];

extern uint8_t done;
extern uint8_t control;
uint8_t old_dir;
uint8_t row;                // player vertical coordinate


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
    old_dir = MOV_DOWN_RIGHT;
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
    qbert.update = update_player;
}


void update_player()
{
    // old_dir = qbert.dir;

    switch (control)
    {
        default:
        case 16:  // left
        case 32:  // up
        case 64:  // down
        case 128: // right
            break;

        case MOV_DOWN_RIGHT:
            qbert.pattern = SPRITE_DOWN_RIGHT;
            jump_table = &jump_down;
            qbert.update = update_player_right;
            break;

        case MOV_DOWN_LEFT:
            qbert.pattern = SPRITE_DOWN_LEFT;
            jump_table = &jump_down;
            qbert.update = update_player_left;
            break;

        case MOV_UP_RIGHT:
            qbert.pattern = SPRITE_UP_RIGHT;
            jump_table = &jump_up;
            qbert.update = update_player_right;
            break;

        case MOV_UP_LEFT:
            qbert.pattern = SPRITE_UP_LEFT;
            jump_table = &jump_up;
            qbert.update = update_player_left;
            break;
    }

    put_qbert_sprite();
}


void update_player_left()
{
    if (qbert.frame < 16)
    {
        qbert.y = qbert.y0 + (*jump_table)[qbert.frame];
        qbert.x = qbert.x0 - ++qbert.frame;
    }
    else
    {
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

        done = press_plate(qbert.pos, qbert.tile_x, qbert.tile_y);

        // reset animation status
        qbert.x0 = qbert.x;
        qbert.y0 = qbert.y;
        qbert.frame = 0;
        qbert.update = update_player;
    }
}


void update_player_right()
{
    if (qbert.frame < 16)
    {
        qbert.y = qbert.y0 + (*jump_table)[qbert.frame];
        qbert.x = qbert.x0 + ++qbert.frame;
    }
    else
    {
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

        done = press_plate(qbert.pos, qbert.tile_x, qbert.tile_y);

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
    uint8_t offset = DIR_LEFT_MASK(qbert.pattern) ? 4 : 0;
    uint8_t fast_x1;
    uint8_t fast_x2;
    uint8_t fast_y1;
    uint8_t fast_y2;

    fast_x1 = qbert.x; // - offset;
    fast_x2 = qbert.x; // - offset + 8;
    fast_y1 = qbert.y - 1;
    fast_y2 = qbert.y - 1; // + 7;

    // if (DIR_LEFT_MASK(qbert.pattern))
    // {
    //     fast_x1 += 1;
    //     fast_x2 += 1;
    //     fast_y1 += 1;
    //     fast_y2 += 1;
    // }

    //
    // Update sprite atributes
    //

    // sclera (white of the eyes)
    sa.y = fast_y1;
    sa.x = fast_x1;
    sa.pattern = qbert.pattern;
    sa.attr = BG_WHITE;
    spman_alloc_sprite(&sa);

    // eyes and mouth
    sa.y = fast_y2;
    sa.x = fast_x1;
    sa.pattern = qbert.pattern + 4;
    sa.attr = BG_BLACK;
    spman_alloc_sprite(&sa);

    // face
    sa.y = fast_y1;
    sa.x = fast_x2;
    sa.pattern = qbert.pattern + 8;
    sa.attr = BG_LIGHTRED;
    spman_alloc_sprite(&sa);

    // the bod (always visible)
    sa.y = fast_y2;
    sa.x = fast_x2;
    sa.pattern = qbert.pattern + 12;
    sa.attr = BG_DARKRED;
    spman_alloc_fixed_sprite(&sa);
}
