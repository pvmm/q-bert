#include "game.h"
#include "plate.h"
#include "set_tiles_colors_at.h"
#include "tiles.h"


// 96 = 12 bytes * 8 bytes
#define CLOSED_PLATES_ADDR          8           // first line
#define OPENED_PLATES_ADDR          1800        // seventh line
#define CHANGE_TO_PLATE_ADDR        (58 * 8)    // "change to" tile location
#define PLATE_TILES_LEN             96          // 12 * 8 bytes
uint8_t closed_plates_colors[PLATE_TILES_LEN];
uint8_t opened_plates_colors[PLATE_TILES_LEN];
uint8_t change_to_plate_color[8];


void init_plate_colors()
{
    uint8_t j = CLOSED_PLATES_ADDR;
    uint8_t k = OPENED_PLATES_ADDR;
    uint8_t l = CHANGE_TO_PLATE_ADDR;

    for (uint8_t i = 0; i < sizeof(closed_plates_colors); ++i)
    {
        closed_plates_colors[i] = tiles_colors[j++];
        opened_plates_colors[i] = tiles_colors[k++];
    }

    for (uint8_t i = 0; i < sizeof(change_to_plate_color); ++i)
    {
        change_to_plate_color[i] = tiles_colors[l++];
    }
}


void set_closed_plate_colors(uint8_t color)
{
    for (uint8_t i = 0; i < sizeof(closed_plates_colors); ++i)
    {
        closed_plates_colors[i] = color | (closed_plates_colors[i] & 0x0f);
    }

    set_tiles_colors_at(closed_plates_colors, CLOSED_PLATES_ADDR, sizeof(closed_plates_colors));
}


void set_opened_plate_colors(uint8_t color)
{
    for (uint8_t i = 0; i < sizeof(opened_plates_colors); ++i)
    {
        opened_plates_colors[i] = color | (opened_plates_colors[i] & 0x0f);
    }

    for (uint8_t i = 0; i < sizeof(change_to_plate_color); ++i)
    {
        change_to_plate_color[i] = color | (change_to_plate_color[i] & 0x0f);
    }

    set_tiles_colors_at(opened_plates_colors, OPENED_PLATES_ADDR, sizeof(opened_plates_colors));
    set_tiles_colors_at(change_to_plate_color, CHANGE_TO_PLATE_ADDR, sizeof(change_to_plate_color));
}

