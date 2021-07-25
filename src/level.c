#include <stdint.h>
#include <msxhal.h>
#include "tile_.h"
#include "tiles.h"


struct level_colors_t
{
    uint8_t bg_color;
    uint8_t plate_color;
    uint8_t wall_color1;
    uint8_t wall_color2;
};


void compose_level (uint8_t level_num)
{
    static const struct level_colors_t LVL[] =
    {
        { BBlack + FBlack, FLightRed, BLightBlue, BDarkBlue, },
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

    draw_block_00_pressed (14, 2);
}


void press_plate (uint8_t x, uint8_t y)
{
    switch (y * 32 + x)
    {
        case 78:
            draw_block_00_pressed (x, y);
            break;
        case 172: case 266: case 360: case 454: case 548: case 642:
            draw_block_01_pressed (x, y);
            break;
        case 176: case 274: case 372: case 470: case 568: case 666:
            draw_block_10_pressed (x, y);
            break;
        default:
            draw_block_11_pressed (x, y);
    }
}
