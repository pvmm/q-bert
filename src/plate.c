#include "game.h"
#include "plate.h"
#include "set_tiles_colors_at.h"
#include "tiles.h"


// 96 = 12 bytes * 8 bytes
#define CLOSED_PLATES_ADDR          8           // 2nd tile of 1st line
#define OPENED_PLATES_ADDR          1800        // 2nd tile of 7th line
#define CHANGE_TO_PLATE_ADDR        (58 * 8)    // (32 + 26) * 8: location of "change to" tile
#define PLATE_TILES_LEN             96          // 12 tiles * 8 bytes = 96 bytes
uint8_t last_closed_color;
uint8_t closed_plates_colors[PLATE_TILES_LEN];
uint8_t last_opened_color;
uint8_t opened_plates_colors[PLATE_TILES_LEN];
uint8_t change_to_plate_color[8];
#define DEFAULT_PLATE_COLOR         0xf0
#define FG_MASK                     0xf0
#define BG_MASK                     0x0f


void init_plate_colors()
{
    for (uint8_t i = 0; i < sizeof(closed_plates_colors); ++i)
    {
        closed_plates_colors[i] = tiles_colors[CLOSED_PLATES_ADDR + i];
        opened_plates_colors[i] = tiles_colors[OPENED_PLATES_ADDR + i];
    }

    for (uint8_t i = 0; i < sizeof(change_to_plate_color); ++i)
    {
        change_to_plate_color[i] = tiles_colors[CHANGE_TO_PLATE_ADDR + i];
    }

    last_opened_color = DEFAULT_PLATE_COLOR;
    last_closed_color = DEFAULT_PLATE_COLOR;
}


void set_closed_plate_colors(uint8_t color)
{
    for (uint8_t i = 0; i < sizeof(closed_plates_colors); ++i)
    {
        if ((closed_plates_colors[i] & FG_MASK) == last_closed_color)
            closed_plates_colors[i] = color | (closed_plates_colors[i] & BG_MASK);
    }

    set_tiles_colors_at(closed_plates_colors, CLOSED_PLATES_ADDR, sizeof(closed_plates_colors));
    last_closed_color = color;
}


void set_opened_plate_colors(uint8_t color)
{
    // update all plates
    for (uint8_t i = 0; i < sizeof(opened_plates_colors); ++i)
    {
        if ((opened_plates_colors[i] & FG_MASK) == last_opened_color)
            opened_plates_colors[i] = color | (opened_plates_colors[i] & BG_MASK);
    }

    // update tiny plate (5 top lines only)
    for (uint8_t i = 0; i < 5; ++i)
    {
        if ((change_to_plate_color[i] & FG_MASK) == last_opened_color)
            change_to_plate_color[i] = color | (change_to_plate_color[i] & BG_MASK);
    }

    set_tiles_colors_at(opened_plates_colors, OPENED_PLATES_ADDR, sizeof(opened_plates_colors));
    set_tiles_colors_at(change_to_plate_color, CHANGE_TO_PLATE_ADDR, sizeof(change_to_plate_color));
    last_opened_color = color;
}

