#include <stdint.h>
#include <msxhal.h>
#include <tile_.h>

#include <res/fonts/tiles.png.h>

/* character blocks */
#define BLOCK_TOP_1 "\x01"
#define BLOCK_TOP_2 "\x02"
#define BLOCK_TOP_3 "\x03"
#define BLOCK_TOP_4 "\x04"
#define BLOCK_TOP_5 "\x05"
#define BLOCK_TOP_6 "\x06"
#define BLOCK_TOP_7 "\x07"
#define BLOCK_TOP_8 "\x08"

#define BLOCK_WALL_1 "\x09"
#define BLOCK_WALL_2 "\x09"
#define BLOCK_WALL_3 "\x0a"
#define BLOCK_WALL_4 "\x0a"
#define BLOCK_WALL_5 "\x13"
#define BLOCK_WALL_6 "\x14"
#define BLOCK_WALL_7 "\x15"
#define BLOCK_WALL_8 "\x16"

#define BLOCK_END_1 "\x0f"
#define BLOCK_END_2 "\x10"
#define BLOCK_END_3 "\x11"
#define BLOCK_END_4 "\x12"

#define BLOCK_PRESSED_1 "\x17"
#define BLOCK_PRESSED_2 "\x18"
#define BLOCK_PRESSED_3 "\x19"
#define BLOCK_PRESSED_4 "\x1a"
#define BLOCK_PRESSED_5 "\x1b"
#define BLOCK_PRESSED_6 "\x1c"
#define BLOCK_PRESSED_7 "\x1d"
#define BLOCK_PRESSED_8 "\x1e"


/* Big structures, like this one, its better to store them in global storage */
static tileset_t main_tileset;


INLINE void set_tile_w
(
    uint8_t index,
    const U8x8 shape0, const U8x8 color0,
    const U8x8 shape1, const U8x8 color1
)
{
    g2_set_tile (MODE2_ALL_ROWS, index, shape0, color0);
    g2_set_tile (MODE2_ALL_ROWS, index + 128, shape1, color1);
}


void prepare_tileset()
{
    g2_wipe_tileset (main_tileset);

    static const uint8_t names1[] =
    {
        1, 2, 3, 4,     // 1..4: first row tiles (top)
        5, 6, 7, 8,     // 5..8: first row tiles (bottom)
        13, 13,         // 9..10: square tiles (lighted and shaded)
        1, 2, 3, 4,     // 11..14: middle row tiles
        0, 0, 0, 0,     // 15..18: last row of tiles
        3, 4, 1, 2,     // 19..22: connection tiles
        /* pressed tiles */
        1, 2, 3, 4,     // 23..26: first row tiles (top)
        5, 6, 7, 8,     // 27..30: first row tiles (bottom)
    };

    static const uint8_t color_indexes1[] =
    {
        1, 1, 1, 1,     // 1..4: first row tiles (upper)
        1, 1, 1, 1,     // 5..8: first row tiles (lower)
        0, 0,           // 9..10: square tiles (lighted and shaded)
        2, 2, 3, 3,     // 11..14: middle row tiles (lighted and shaded)
        0, 0, 0, 0,     // 15..18: last row of tiles
        1, 1, 1, 1,     // 19..22: connection tiles
        /* pressed tiles */
        4, 4, 4, 4,     // 23..26: first row tiles (top)
        4, 4, 4, 4,     // 27..30: first row tiles (bottom)
    };

    static const uint8_t names2[] =
    {
        1, 2, 3, 4,     // 1..4: tiles from first row
        3, 4, 1, 2,     // 5..8: tiles from second row
        13, 13,         // 9..10: square tiles (lighted and shaded)
        0, 0, 0, 0,     // 11..14: middle row tiles (lighted and shaded)
        5, 6, 7, 8,     // 15..18: last row tiles
        5, 6, 7, 8,     // 19..22: connection tiles
        /* pressed tiles */
        1, 2, 3, 4,     // 23..26: first row tiles (top)
        3, 4, 1, 2,     // 27..30: first row tiles (bottom)
    };

    static const uint8_t color_indexes2[] =
    {
        4, 4, 4, 4,     // 1..4: first row tiles (upper)
        5, 5, 6, 6,     // 5..8: first row tiles (lower)
        5, 6,           // 9..10: square tiles (lighted and shaded)
        0, 0, 0, 0,     // 11..14: middle row tiles (lighted and shaded)
        5, 5, 6, 6,     // 15..18: last row of tiles
        5, 5, 6, 6,     // 19..22: connection tiles
        /* pressed tiles */
        4, 4, 4, 4,     // 23..26: first row tiles (top)
        5, 5, 6, 6,     // 27..30: first row tiles (bottom)
    };

    static const u8x8_t colors[] =
    {
        {BBlack+FBlack, BBlack+FBlack, BBlack+FBlack, BBlack+FBlack, BBlack+FBlack, BBlack+FBlack, BBlack+FBlack, BBlack+FBlack},
        {BBlack+FLightRed, BBlack+FLightRed, BBlack+FLightRed, BBlack+FLightRed, BBlack+FLightRed, BBlack+FLightRed, BBlack+FLightRed, BBlack+FLightRed},
        {BBlack+FLightBlue, BBlack+FLightBlue, BBlack+FLightBlue, BBlack+FLightBlue, BBlack+FLightBlue, BBlack+FLightBlue, BBlack+FLightBlue, BBlack+FLightBlue},
        {BBlack+FDarkBlue, BBlack+FDarkBlue, BBlack+FDarkBlue, BBlack+FDarkBlue, BBlack+FDarkBlue, BBlack+FDarkBlue, BBlack+FDarkBlue, BBlack+FDarkBlue},
        {BBlack+FWhite, BBlack+FWhite, BBlack+FWhite, BBlack+FWhite, BBlack+FWhite, BBlack+FWhite, BBlack+FWhite, BBlack+FWhite},
        {BWhite+FLightBlue, BWhite+FLightBlue, BWhite+FLightBlue, BWhite+FLightBlue, BWhite+FLightBlue, BWhite+FLightBlue, BWhite+FLightBlue, BWhite+FLightBlue},
        {BWhite+FDarkBlue, BWhite+FDarkBlue, BWhite+FDarkBlue, BWhite+FDarkBlue, BWhite+FDarkBlue, BWhite+FDarkBlue, BWhite+FDarkBlue, BWhite+FDarkBlue},
    };

    for (uint8_t i = 0; i < 30; ++i)
    {
        set_tile_w(i + 1,
                   tiles[names1[i]], colors[color_indexes1[i]],
                   tiles[names2[i]], colors[color_indexes2[i]]);
    }
}


static void draw_block1(uint8_t x, uint8_t y)
{
    g2_put_at (x, y, BLOCK_TOP_1 BLOCK_TOP_2 BLOCK_TOP_3 BLOCK_TOP_4, 4);
    g2_put_at (x, y + 1, BLOCK_TOP_5 BLOCK_TOP_6 BLOCK_TOP_7 BLOCK_TOP_8, 4);
    g2_put_at (x, y + 2, BLOCK_WALL_1 BLOCK_WALL_2 BLOCK_WALL_3 BLOCK_WALL_4, 4);
    g2_put_at (x, y + 3, BLOCK_WALL_5 BLOCK_WALL_6 BLOCK_WALL_7 BLOCK_WALL_8, 4);
}


static void draw_block1_pressed(uint8_t x, uint8_t y)
{
    g2_put_at (x, y, BLOCK_PRESSED_1 BLOCK_PRESSED_2 BLOCK_PRESSED_3 BLOCK_PRESSED_4, 4);
    g2_put_at (x, y + 1, BLOCK_PRESSED_5 BLOCK_PRESSED_6 BLOCK_PRESSED_7 BLOCK_PRESSED_8, 4);
}


static void draw_block2(uint8_t x, uint8_t y)
{
    g2_put_at (x, y, BLOCK_TOP_1 BLOCK_TOP_2 BLOCK_TOP_3 BLOCK_TOP_4, 4);
    g2_put_at (x, y + 1, BLOCK_TOP_5 BLOCK_TOP_6 BLOCK_TOP_7 BLOCK_TOP_8, 4);
    g2_put_at (x, y + 2, BLOCK_WALL_1 BLOCK_WALL_2 BLOCK_WALL_3 BLOCK_WALL_4, 4);
    g2_put_at (x, y + 3, BLOCK_END_1 BLOCK_END_2 BLOCK_END_3 BLOCK_END_4, 4);
}


void draw_scenery()
{
    // From bottom to top.
    draw_block2(2, 20);
    draw_block2(6, 20);
    draw_block2(10, 20);
    draw_block2(14, 20);
    draw_block2(18, 20);
    draw_block2(22, 20);
    draw_block2(26, 20);

    draw_block1(4, 17);
    draw_block1(8, 17);
    draw_block1(12, 17);
    draw_block1(16, 17);
    draw_block1(20, 17);
    draw_block1(24, 17);

    draw_block1(6, 14);
    draw_block1(10, 14);
    draw_block1(14, 14);
    draw_block1(18, 14);
    draw_block1(22, 14);

    draw_block1(8, 11);
    draw_block1(12, 11);
    draw_block1(16, 11);
    draw_block1(20, 11);

    draw_block1(10, 8);
    draw_block1(14, 8);
    draw_block1(18, 8);

    draw_block1(12, 5);
    draw_block1(16, 5);

    draw_block1(14, 2);

    // draw_block1_pressed(12, 17);
}


void change_floor_color ()
{
    static u8x8_t color1 = {BBlack+FLightYellow, BBlack+FLightYellow, BBlack+FLightYellow,
        BBlack+FLightYellow, BBlack+FLightYellow, BBlack+FLightYellow, BBlack+FLightYellow,
        BBlack+FLightYellow};
    // static u8x8_t color2 = {BBlack+FWhite, BBlack+FWhite, BBlack+FWhite, BBlack+FWhite,
    // BBlack+FWhite, BBlack+FWhite, BBlack+FWhite, BBlack+FWhite};

    for (uint8_t i = 1; i < 9; ++i)
    {
        g2_set_tile_color (MODE2_ALL_ROWS, i, color1);
        //g2_set_tile_color (MODE2_ALL_ROWS, i + 128, color2);
    }
}


void init_tiles ()
{
    Tiles_init ();
    g2_set_interlaced (true);
    prepare_tileset ();
    //change_floor_color ();
}
