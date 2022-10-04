#include "game.h"
#include "entities.h"
#include "palette.h"
#include "lookup_tables.h"
#include "level.h"
#include "spman.h"


const int8_t (*jump_table)[16];

extern uint8_t completed;   // completed the plates?
extern uint8_t control;
uint8_t old_pattern;
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

        completed = press_plate(qbert.pos, qbert.tile_x, qbert.tile_y);

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

        completed = press_plate(qbert.pos, qbert.tile_x, qbert.tile_y);

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
    fast_y = qbert.y;

    //
    // Update sprite atributes
    //

    sa.y = fast_y;
    sa.x = fast_x;

    // sclera (white of the eyes)
    sa.pattern = qbert.pattern;
    //debug("pattern: ", qbert.pattern);
    sa.attr = BG_WHITE;
    spman_alloc_sprite(&sa);

    // eyes and mouth
    sa.pattern = qbert.pattern + 4;
    sa.attr = BG_BLACK;
    spman_alloc_sprite(&sa);

    // face
    sa.pattern = qbert.pattern + 8;
    sa.attr = BG_LIGHTRED;
    spman_alloc_sprite(&sa);

    // the bod (always visible)
    sa.pattern = qbert.pattern + 12;
    sa.attr = BG_DARKRED;
    spman_alloc_fixed_sprite(&sa);
}
