//#define DEBUG TRUE
#include <msxhal.h>
#include <tms99X8.h>

#include "level.h"
#include "qbert.h"
#include "lookup_tables.h"


extern struct Qbert qbert;

const int8_t (*jump_table)[16];

void _print(const char* msg);

uint8_t row;

/* Level finished? */
bool done = false;


void update_player_l (int8_t fps) __z88dk_fastcall
{
    UNUSED(fps);

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

        done = press_plate (qbert.pos, qbert.tile_x, qbert.tile_y);

        // reset animation status
        qbert.x0 = qbert.x;
        qbert.y0 = qbert.y;
        qbert.frame = 0;
        qbert.update = nullptr;
    }
}


void update_player_r (int8_t fps) __z88dk_fastcall
{
    UNUSED (fps);

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

        done = press_plate (qbert.pos, qbert.tile_x, qbert.tile_y);

        // reset animation status
        qbert.x0 = qbert.x;
        qbert.y0 = qbert.y;
        qbert.frame = 0;
        qbert.update = nullptr;
    }
}


void print_num (uint16_t num)
{
    do
    {
        _print("A");
        num--;
    }
    while (num != 0);
}


int main (void)
{
    enum Direction old_direction = DIR_DOWN_RIGHT;
    uint8_t input = 0;
    row = 1;

    msxhal_init ();

    TMS99X8_activateMode2 (MODE2_ALL_ROWS);

    init_qbert ();

    compose_level (LEVEL_1);

    // Main loop, we alternate between buffers at each interruption.
    while (true)
    {
        if (done)
        {
            done = false;
            finish_level (); 
        }

        wait_frame ();

        debugBorder(BMagenta);

        TMS99X8_activateBuffer (MODE2_BUFFER_0);

        debugBorder(BWhite);

        if (qbert.update != nullptr)
            qbert.update (1);

        debugBorder(BLightGreen);

        put_qbert_sprite (MODE2_BUFFER_1, qbert.dirty);

        debugBorder(BBlack);

        wait_frame ();

        TMS99X8_activateBuffer (MODE2_BUFFER_1);

        put_qbert_sprite (MODE2_BUFFER_0, qbert.dirty);
        qbert.dirty = false;

        if (qbert.update == nullptr)
        {
            input = msxhal_joystick_read (0);
            old_direction = qbert.direction;

            switch (input)
            {
                default:
                case 16: // left
                case 32: // up
                case 64: // down
                case 128: // right
                    break;

                case DIR_DOWN_RIGHT:
                    qbert.direction = DIR_DOWN_RIGHT;
                    jump_table = &jump_down;
                    qbert.update = update_player_r;
                    break;

                case DIR_DOWN_LEFT:
                    qbert.direction = DIR_DOWN_LEFT;
                    jump_table = &jump_down;
                    qbert.update = update_player_l;
                    break;

                case DIR_UP_RIGHT:
                    qbert.direction = DIR_UP_RIGHT;
                    jump_table = &jump_up;
                    qbert.update = update_player_r;
                    break;

                case DIR_UP_LEFT:
                    qbert.direction = DIR_UP_LEFT;
                    jump_table = &jump_up;
                    qbert.update = update_player_l;
                    break;
            }

            qbert.dirty = (old_direction != qbert.direction);
        }
        else
        {
            qbert.update (1);
        }
    }

    return 0;
}


#ifdef MSX
#pragma disable_warning 85
void _print(char* msg) {
        __asm
                ld      hl, #2; retrieve address from stack
                add     hl, sp
                ld              b, (hl)
                inc             hl
                ld              h, (hl)
                ld              l, b

                _printMSG_loop :
                ld              a, (hl); print
                or              a
                ret z
                push    hl
                push    ix
                ld              iy, (#0xfcc0); BIOS_ROMSLT
                ld              ix, #0x00a2; BIOS_CHPUT
                call    #0x001c; BIOS_CALSLT
                pop             ix
                pop             hl
                inc             hl
                jr              _printMSG_loop
        __endasm;

        return;
}
#else
void _print(const char* msg)
{
    printf("%s", msg);
}
#endif
