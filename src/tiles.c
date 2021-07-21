#include <stdint.h>
#include <msxhal.h>
#include <tile_.h>

#include <res/fonts/tiles.png.h>

/* character blocks */
#define PLATE_1         "\x01" // 1
#define PLATE_2         "\x02" // 2
#define PLATE_3         "\x03" // 3
#define PLATE_4         "\x04" // 4
#define PLATE_5         "\x05" // 5
#define PLATE_6         "\x06" // 6
#define PLATE_7         "\x07" // 7
#define PLATE_8         "\x08" // 8

#define WALL_1          "\x09" // 9
#define WALL_2          "\x09" // 9
#define WALL_3          "\x0a" // 10
#define WALL_4          "\x0a" // 10

#define WALL_END_1      "\x0b"  // 11
#define WALL_END_2      "\x0c"  // 12
#define WALL_END_3      "\x0d"  // 13
#define WALL_END_4      "\x0e"  // 14

#define WALL_5          "\x0f" // 15
#define WALL_6          "\x10" // 16
#define WALL_7          "\x11" // 17
#define WALL_8          "\x12" // 18

#define PLATE_PRESSED_1 "\x13" // 19
#define PLATE_PRESSED_2 "\x14" // 20
#define PLATE_PRESSED_3 "\x15" // 21
#define PLATE_PRESSED_4 "\x16" // 22
#define PLATE_PRESSED_5 "\x17" // 23
#define PLATE_PRESSED_6 "\x18" // 24
#define PLATE_PRESSED_7 "\x19" // 25
#define PLATE_PRESSED_8 "\x1a" // 26


/* Big structures, like this one, its better to store them in global storage */
static tileset_t main_tileset;


INLINE void set_tile_w (uint8_t index,
                        const U8x8 shape0, const U8x8 color0,
                        const U8x8 shape1, const U8x8 color1)
{
    g2_set_tile (MODE2_ALL_ROWS, index, shape0, color0);
    g2_set_tile (MODE2_ALL_ROWS, index + 128, shape1, color1); // TODO: remove this fixed rule and set tiles manually
}


void prepare_tileset()
{
    g2_wipe_tileset (main_tileset);

    static const uint8_t names1[] =
    {
        1, 2, 3, 4,     // 1..4: tiles from first row (top tiles), colors: BBlack+FLightRed / BBlack+FWhite : BBlack+FLightRed (negative) / BBlack+Fwhite (negative)
        5, 6, 7, 8,     // 5..8: tiles from first row (bottom tiles), colors: BBlack+FLightRed/BWhite+FLightBlue
        13, 13,         // 9..10: square tiles (lighted and shaded)
        0, 0, 0, 0,     // 11..14: tiles from last row
        3, 4, 1, 2,     // 15..18: connection tiles
        /* pressed tiles */
        1, 2, 3, 4,     // 19..22: first row tiles (top tiles)
        5, 6, 7, 8,     // 23..26: first row tiles (bottom tiles)
    };

    static const uint8_t color_indexes1[] =
    {
        1, 1, 1, 1,     // 1..4: first row tiles (top tiles)
        1, 1, 1, 1,     // 5..8: first row tiles (bottom tiles)
        0, 0,           // 9..10: square tiles (lighted and shaded)
        0, 0, 0, 0,     // 11..14: tiles from last row
        1, 1, 1, 1,     // 15..18: connection tiles
        /* pressed tiles */
        4, 4, 4, 4,     // 19..22: first row tiles (top tiles)
        4, 4, 4, 4,     // 23..26: first row tiles (bottom tiles)
    };

    static const uint8_t names2[] =
    {
        0, 0, 0, 0,     // 1..4: tiles from first row (top tiles)
        5, 6, 7, 8,     // 5..8: tiles from second row (bottom tiles)
        13, 13,         // 9..10: square tiles (lighted and shaded solid blocks)

        3, 4, 1, 2,     // 11..14: tiles from last row
        3, 4, 1, 2,     // 15..18: connection tiles
        /* pressed tiles */
        0, 0, 0, 0,     // 19..22: first row tiles (top tiles)
        5, 6, 7, 8,     // 23..26: first row tiles (bottom tiles)
    };

    static const uint8_t color_indexes2[] =
    {
        0, 0, 0, 0,     // 1..4: first row tiles (top tiles)
        7, 7, 8, 8,     // 5..8: first row tiles (bottom tiles)
        5, 6,           // 9..10: square tiles (lighted and shaded)

        7, 7, 8, 8,     // 15..18: tiles from last row
        7, 7, 8, 8,     // 19..22: connection tiles
        /* pressed tiles */
        4, 4, 4, 4,     // 23..26: first row tiles (top tiles)
        7, 7, 8, 8,     // 27..30: first row tiles (bottom tiles)
    };

    static const u8x8_t colors[] =
    {
        /* 0 */ {BBlack+FBlack, BBlack+FBlack, BBlack+FBlack, BBlack+FBlack, BBlack+FBlack, BBlack+FBlack, BBlack+FBlack, BBlack+FBlack, },
        /* 1 */ {BBlack+FLightRed, BBlack+FLightRed, BBlack+FLightRed, BBlack+FLightRed, BBlack+FLightRed, BBlack+FLightRed, BBlack+FLightRed, BBlack+FLightRed, },
        /* 2 */ {BBlack+FLightBlue, BBlack+FLightBlue, BBlack+FLightBlue, BBlack+FLightBlue, BBlack+FLightBlue, BBlack+FLightBlue, BBlack+FLightBlue, BBlack+FLightBlue, },
        /* 3 */ {BBlack+FDarkBlue, BBlack+FDarkBlue, BBlack+FDarkBlue, BBlack+FDarkBlue, BBlack+FDarkBlue, BBlack+FDarkBlue, BBlack+FDarkBlue, BBlack+FDarkBlue, },
        /* 4 */ {BBlack+FLightYellow, BBlack+FLightYellow, BBlack+FLightYellow, BBlack+FLightYellow, BBlack+FLightYellow, BBlack+FLightYellow, BBlack+FLightYellow, BBlack+FLightYellow, },
        /* 5 */ {BLightBlue+FLightBlue, BWhite+FLightBlue, BWhite+FLightBlue, BWhite+FLightBlue, BWhite+FLightBlue, BWhite+FLightBlue, BWhite+FLightBlue, BWhite+FLightBlue, },
        /* 6 */ {BWhite+FDarkBlue, BWhite+FDarkBlue, BWhite+FDarkBlue, BWhite+FDarkBlue, BWhite+FDarkBlue, BWhite+FDarkBlue, BWhite+FDarkBlue, BWhite+FDarkBlue, },
        /* 7 */ {BLightBlue+FBlack, BLightBlue+FBlack, BLightBlue+FBlack, BLightBlue+FBlack, BLightBlue+FBlack, BLightBlue+FBlack, BLightBlue+FBlack, BLightBlue+FBlack, },
        /* 8 */ {BDarkBlue+FBlack, BDarkBlue+FBlack, BDarkBlue+FBlack, BDarkBlue+FBlack, BDarkBlue+FBlack, BDarkBlue+FBlack, BDarkBlue+FBlack, BDarkBlue+FBlack, },
    };

    for (uint8_t i = 0; i < 26; ++i)
    {
        set_tile_w (i + 1,
                    tiles[names1[i]], colors[color_indexes1[i]],
                    tiles[names2[i]], colors[color_indexes2[i]]);
    }
}


static void draw_plate_0_left (uint8_t x, uint8_t y)
{
    g2_put_at (x, y, PLATE_1 PLATE_2, 2);
    g2_put_at (x, y + 1, PLATE_5 PLATE_6, 2);
}


static void draw_plate_0_pressed_left (uint8_t x, uint8_t y)
{
    g2_put_at (x, y, PLATE_PRESSED_1 PLATE_PRESSED_2, 2);
    g2_put_at (x, y + 1, PLATE_PRESSED_5 PLATE_PRESSED_6, 2);
}


static void draw_plate_0_right (uint8_t x, uint8_t y)
{
    g2_put_at (x + 2, y, PLATE_3 PLATE_4, 2);
    g2_put_at (x + 2, y + 1, PLATE_7 PLATE_8, 2);
}


static void draw_plate_0_pressed_right (uint8_t x, uint8_t y)
{
    g2_put_at (x + 2, y, PLATE_PRESSED_3 PLATE_PRESSED_4, 2);
    g2_put_at (x + 2, y + 1, PLATE_PRESSED_7 PLATE_PRESSED_8, 2);
}


static void draw_plate_00(uint8_t x, uint8_t y)
{
    draw_plate_0_left (x, y);
    draw_plate_0_right (x, y);
}


static void draw_plate_00_pressed(uint8_t x, uint8_t y)
{
    draw_plate_0_pressed_left (x, y);
    draw_plate_0_pressed_right (x, y);
}


static void draw_plate_1_left(uint8_t x, uint8_t y)
{
    g2_put_at (x, y, WALL_7 WALL_8, 2);
    g2_put_at (x, y + 1, PLATE_5 PLATE_6, 2);
}


static void draw_plate_1_right(uint8_t x, uint8_t y)
{
    g2_put_at (x + 2, y, WALL_5 WALL_6, 2);
    g2_put_at (x + 2, y + 1, PLATE_7 PLATE_8, 2);
}


static void draw_plate_11(uint8_t x, uint8_t y)
{
    draw_plate_1_left (x, y);
    draw_plate_1_right (x, y);
}


static void draw_plate_01(uint8_t x, uint8_t y)
{
    draw_plate_0_left (x, y);
    draw_plate_1_right (x, y);
}


static void draw_plate_10(uint8_t x, uint8_t y)
{
    draw_plate_1_left (x, y);
    draw_plate_0_right (x, y);
}


static void draw_block_00(uint8_t x, uint8_t y)
{
    draw_plate_00 (x, y);
    g2_put_at (x, y + 2, WALL_1 WALL_2 WALL_3 WALL_4, 4);
}


static void draw_block_00_pressed(uint8_t x, uint8_t y)
{
    draw_plate_00_pressed (x, y);
    g2_put_at (x, y + 2, WALL_1 WALL_2 WALL_3 WALL_4, 4);
}


static void draw_block_01(uint8_t x, uint8_t y)
{
    draw_plate_01 (x, y);
    g2_put_at (x, y + 2, WALL_1 WALL_2 WALL_3 WALL_4, 4);
}


static void draw_block_10(uint8_t x, uint8_t y)
{
    draw_plate_10 (x, y);
    g2_put_at (x, y + 2, WALL_1 WALL_2 WALL_3 WALL_4, 4);
}


static void draw_block_11(uint8_t x, uint8_t y)
{
    draw_plate_11 (x, y);
    g2_put_at (x, y + 2, WALL_1 WALL_2 WALL_3 WALL_4, 4);
}


static void draw_block_end(uint8_t x, uint8_t y)
{
    g2_put_at (x, y, WALL_END_1 WALL_END_2 WALL_END_3 WALL_END_4, 4);
}


void draw_scenery()
{
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
