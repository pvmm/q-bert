#include <stdint.h>
#include <msxhal.h>

#include "tile.h"
#include "tiles.h"


struct level_colors_t
{
    uint8_t bg_color;
    uint8_t plate_color;
    uint8_t wall_color1;
    uint8_t wall_color2;
};

#define MAX_PLATES      28
#define PLATE_COUNT     29

/* Store plate status. Last position is the total of plates pressed. */
uint8_t plate_status[PLATE_COUNT + 1];


/**
 * Draw level according to level number in level_num.
 */
void compose_level (uint8_t level_num)
{
    static const struct level_colors_t LVL[] =
    {
        { BBlack + FBlack, FDarkRed, BLightBlue, BDarkBlue, },
    };

    uint8_t bg_color = LVL[level_num].bg_color;
    uint8_t plate_color = BG_MASK (bg_color) + LVL[level_num].plate_color;
    uint8_t wall_color1 = LVL[level_num].wall_color1 + FG_MASK (bg_color);
    uint8_t wall_color2 = LVL[level_num].wall_color2 + FG_MASK (bg_color);
    init_tiles (bg_color, plate_color, wall_color1, wall_color2);

    draw_block_00 (14, 2);

    draw_block_01 (12, 5);
    draw_block_10 (16, 5);

    draw_block_01 (10, 8);
    draw_block_11 (14, 8);
    draw_block_10 (18, 8);

    draw_block_01 (8, 11);
    draw_block_11 (12, 11);
    draw_block_11 (16, 11);
    draw_block_10 (20, 11);

    draw_block_01 (6, 14);
    draw_block_11 (10, 14);
    draw_block_11 (14, 14);
    draw_block_11 (18, 14);
    draw_block_10 (22, 14);

    draw_block_01 (4, 17);
    draw_block_11 (8, 17);
    draw_block_11 (12, 17);
    draw_block_11 (16, 17);
    draw_block_11 (20, 17);
    draw_block_10 (24, 17);

    draw_block_01 (2, 20);
    draw_block_11 (6, 20);
    draw_block_11 (10, 20);
    draw_block_11 (14, 20);
    draw_block_11 (18, 20);
    draw_block_11 (22, 20);
    draw_block_10 (26, 20);

    draw_block_end (2, 23);
    draw_block_end (6, 23);
    draw_block_end (10, 23);
    draw_block_end (14, 23);
    draw_block_end (18, 23);
    draw_block_end (22, 23);
    draw_block_end (26, 23);

    /* Q*Bert lands here. */
    draw_block_00_pressed (14, 2);

    /* Wipe all plates, except the first. */
    for (int8_t i = 1; i < 28; ++i)
        plate_status[i] = 0;

    plate_status[0] = 1;
    plate_status[29] = 1;
}


/**
 * Press plate defined by position pos. If all plates were pressed, return true otherwise false.
 */
bool press_plate (uint8_t pos, uint8_t x, uint8_t y)
{
    switch (pos)
    {
        case 1:
            draw_block_00_pressed (x, y);
            break;
        case 2: case 4: case 7: case 11: case 16: case 22:
            draw_block_01_pressed (x, y);
            break;
        case 3: case 6: case 10: case 15: case 21: case 28:
            draw_block_10_pressed (x, y);
            break;
        default:
            draw_block_11_pressed (x, y);
    }

    if (!plate_status[--pos])
    {
        plate_status[pos] = 1;
        return (++plate_status[PLATE_COUNT] == MAX_PLATES);
    }

    return false;
}


void finish_level ()
{
    uint8_t foreground = 0;

    for (int i = 0; i < 160; ++i)
    {
        foreground += 16;
        change_plate_color (foreground + BBlack);

        TMS99X8_activateBuffer (MODE2_BUFFER_0);
        wait_frame ();

        TMS99X8_activateBuffer (MODE2_BUFFER_1);
        wait_frame ();
    }

    change_plate_color (FLightYellow + BBlack);
}
