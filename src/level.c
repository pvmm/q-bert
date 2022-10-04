#include "main.h"
#include "level.h"


#define MAX_PLATES      28
//#define PLATE_COUNT     29

/* Store plate status. Last position is the total of plates pressed. */
uint8_t plate_status[MAX_PLATES + 1];
uint8_t plate_counter;


void init_level(void)
{
    /* Wipe all plates */
    for (int8_t i = 0; i < MAX_PLATES; ++i) {
        plate_status[i] = 0;
    }
    plate_counter = 0;
}


void draw_block_00_pressed(uint8_t x, uint8_t y)
{
    ubox_put_tile(x + 0, y + 0, 225);
    ubox_put_tile(x + 1, y + 0, 226);
    ubox_put_tile(x + 2, y + 0, 227);
    ubox_put_tile(x + 3, y + 0, 228);
    ubox_put_tile(x + 0, y + 1, 229);
    ubox_put_tile(x + 1, y + 1, 230);
    ubox_put_tile(x + 2, y + 1, 231);
    ubox_put_tile(x + 3, y + 1, 232);
}


void draw_block_01_pressed(uint8_t x, uint8_t y)
{
    ubox_put_tile(x + 0, y + 0, 225);
    ubox_put_tile(x + 1, y + 0, 226);

    ubox_put_tile(x + 2, y + 0, 233); // changed
    ubox_put_tile(x + 3, y + 0, 234);

    ubox_put_tile(x + 0, y + 1, 229);
    ubox_put_tile(x + 1, y + 1, 230);

    ubox_put_tile(x + 2, y + 1, 231);
    ubox_put_tile(x + 3, y + 1, 232);
}


void draw_block_10_pressed(uint8_t x, uint8_t y)
{
    ubox_put_tile(x + 0, y + 0, 235); // changed
    ubox_put_tile(x + 1, y + 0, 236);

    ubox_put_tile(x + 2, y + 0, 227);
    ubox_put_tile(x + 3, y + 0, 228);

    ubox_put_tile(x + 0, y + 1, 229);
    ubox_put_tile(x + 1, y + 1, 230);

    ubox_put_tile(x + 2, y + 1, 231);
    ubox_put_tile(x + 3, y + 1, 232);
}


void draw_block_11_pressed(uint8_t x, uint8_t y)
{
    ubox_put_tile(x + 0, y + 0, 235); // changed
    ubox_put_tile(x + 1, y + 0, 236);

    ubox_put_tile(x + 2, y + 0, 233); // changed
    ubox_put_tile(x + 3, y + 0, 234);

    ubox_put_tile(x + 0, y + 1, 229);
    ubox_put_tile(x + 1, y + 1, 230);
    ubox_put_tile(x + 2, y + 1, 231);
    ubox_put_tile(x + 3, y + 1, 232);
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
        return ++plate_counter == MAX_PLATES;
    }

    return false;
}
