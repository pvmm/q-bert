#include "level.h"


#define MAX_PLATES      28
#define PLATE_COUNT     29

/* Store plate status. Last position is the total of plates pressed. */
uint8_t plate_status[PLATE_COUNT + 1];


void init_plate(void)
{
    /* Wipe all plates, except the first. */
    for (int8_t i = 1; i < 28; ++i) {
        plate_status[i] = 0;
    }
}

void draw_block_00_pressed(uint8_t x, uint8_t y)
{
    x; y;
}

void draw_block_01_pressed(uint8_t x, uint8_t y)
{
    x; y;
}

void draw_block_10_pressed(uint8_t x, uint8_t y)
{
    x; y;
}

void draw_block_11_pressed(uint8_t x, uint8_t y)
{
    x; y;
}

bool press_plate(uint8_t pos, uint8_t x, uint8_t y)
{
    switch (pos)
    {
        case 1:
            draw_block_00_pressed(x, y);
            break;
        case 2: case 4: case 7: case 11: case 16: case 22:
            draw_block_01_pressed(x, y);
            break;
        case 3: case 6: case 10: case 15: case 21: case 28:
            draw_block_10_pressed(x, y);
            break;
        default:
            draw_block_11_pressed(x, y);
            break;
    }

    if (!plate_status[--pos])
    {
        plate_status[pos] = 1;
        return (++plate_status[PLATE_COUNT] == MAX_PLATES);
    }

    return false;
}
