// If DEBUG is defined, the function debugBorder changes the border color, otherwise it does nothing.
//#define DEBUG TRUE

#include <msxhal.h>
#include <tms99X8.h>
#include <tile.h>
//#include <rand.h>

#include "qbert.h"
#include "tiles.h"
#include "lookup_tables.h"

extern struct Qbert qbert;

const int8_t (*jump_table)[16];

void update_player_l(int8_t fps) __z88dk_fastcall
{
    UNUSED(fps);

    if (qbert.frame < 16)
    {
        qbert.y = qbert.y0 + (*jump_table)[qbert.frame];
        qbert.x = qbert.x0 - ++qbert.frame;
    }
    else
    {
        // reset animation status
        qbert.x0 = qbert.x;
        qbert.y0 = qbert.y;
        qbert.frame = 0;
        qbert.update = nullptr;
    }
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


void update_player_r(int8_t fps) __z88dk_fastcall
{
    UNUSED(fps);

    if (qbert.frame < 16)
    {
        qbert.y = qbert.y0 + (*jump_table)[qbert.frame];
        qbert.x = qbert.x0 + ++qbert.frame;
    }
    else
    {
        // reset animation status
        qbert.x0 = qbert.x;
        qbert.y0 = qbert.y;
        qbert.frame = 0;
        qbert.update = nullptr;
    }
}


void print_num(uint16_t num)
{
    do
    {
        _print("A");
        num--;
    }
    while (num != 0);
}


int main(void)
{
    uint8_t input = 0, old_input = 1;

    msxhal_init();

    TMS99X8_activateMode2(MODE2_ALL_ROWS);

    init_tiles();

    init_qbert_sprites();

    draw_scenery();

    // Main loop, we alternate between buffers at each interruption.
    while (true)
    {
        if (qbert.update == nullptr)
        {
            old_input = input;
            input = msxhal_joystick_read(0);
            qbert.dirty = (old_input != input);

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
        }
        else
        {
            qbert.update(1);
        }

        wait_frame();

        // We select buffer 0, we modify sprites on buffer 1

        //debugBorder(BMagenta);
        TMS99X8_activateBuffer(MODE2_BUFFER_0);
        put_qbert_sprite(MODE2_BUFFER_1);
        wait_frame();

        // We select buffer 1, so we modify sprites on buffer 0

        //debugBorder(BWhite);
        TMS99X8_activateBuffer(MODE2_BUFFER_1);
        put_qbert_sprite(MODE2_BUFFER_0);
        qbert.dirty = false;
    };

    return 0;
}
