#include <stdint.h>
#include <msxhal.h>
#include <tile_.h>
//#include <tile.h>

#include <res/fonts/font1.png.h>
#include <res/fonts/font2.png.h>


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

    static const int names1[] =
    {
        0, 1, 2, 3,     // first row tiles (upper)
        4, 5, 6, 7,     // first row tiles (lower)
        12, 12,         // square tiles (lighted and shaded)
        0, 1, 2, 3,     // middle row tiles
        12, 12, 12, 12, // last row of tiles (empty)
        2, 3, 0, 1,     // connection tiles

    };

    static const uint8_t color_indexes1[] =
    {
        //* 32 to 39 */
        /* 0 to 7 */
        0, 0, 0, 0, 0, 0, 0, 0,
        /* 8 and 9 */
        1, 1, /* square tiles */
        // 42 and 43
        /* 10 and 11 */
        2, 2, /* lighted face tiles (left) */
        /* 12 and 13 */
        3, 3, /* shaded face tiles (right) */
        // 46 to 49
        /* 14 to 17 */
        4, 4, 4, 4,
        // 50 to 53
        /* 18 to 21 */
        5, 5, 5, 5
    };

    static const U8x8 colors1[] =
    {
        /* -- first row tiles -- */
        /* upper */
        /* 32 to 39 */
        {BBlack+FLightRed, BBlack+FLightRed, BBlack+FLightRed, BBlack+FLightRed, BBlack+FLightRed, BBlack+FLightRed, BBlack+FLightRed, BBlack+FLightRed},

        /* -- square tiles -- */
        /* 40 and 41 */
        {BBlack+FBlack, BBlack+FBlack, BBlack+FBlack, BBlack+FBlack, BBlack+FBlack, BBlack+FBlack, BBlack+FBlack, BBlack+FBlack},

        // -- middle row tiles --
        // lighted face tiles (left)
        /* 42 and 43 */
        {BBlack+FLightBlue, BBlack+FLightBlue, BBlack+FLightBlue, BBlack+FLightBlue, BBlack+FLightBlue, BBlack+FLightBlue, BBlack+FLightBlue, BBlack+FLightBlue},
        // shaded face tiles (right)
        /* 44 and 45 */
        {BBlack+FDarkBlue, BBlack+FDarkBlue, BBlack+FDarkBlue, BBlack+FDarkBlue, BBlack+FDarkBlue, BBlack+FDarkBlue, BBlack+FDarkBlue, BBlack+FDarkBlue},

        // -- last row tiles --
        // lighted face tiles (left)
        /* 46 */
        {BBlack+FBlack, BBlack+FBlack, BBlack+FBlack, BBlack+FBlack, BBlack+FBlack, BBlack+FBlack, BBlack+FBlack, BBlack+FBlack},

        /* -- connection tiles -- */
        /* upper */
        /* 50 to 53 */
        {BBlack+FLightRed, BBlack+FLightRed, BBlack+FLightRed, BBlack+FLightRed, BBlack+FLightRed, BBlack+FLightRed, BBlack+FLightRed, BBlack+FLightRed},
    };

    static const int names2[] =
    {
        0, 1, 2, 3,     // tiles from first row
        4, 5, 6, 7,     // tiles from second row
        12, 12,         // square tiles (lighted and shaded)
        4, 5, 6, 7,     // middle row tiles
        8, 9, 10, 11,   // last row tiles
        8, 9, 10, 11,   // connection tiles
    };

    static const uint8_t color_indexes2[] =
    {
        // 32 to 35
        /* 0 to 4 */
        0, 0, 0, 0,
        // 36 and 37
        1, 1,
        // 38 and 39
        2, 2,
        // -- square tiles --
        // lighted square and shaded square
        // 40 and 41
        1, 2,
        // 42 to 45
        3, 3, 3, 3,
        // last row tiles
        // 46 and 47
        4, 4,
        // 48 and 49
        5, 5,
        // 50 and 51
        1, 1,
        // 52 and 53
        2, 2,
    };

    static const U8x8 colors2[] =
    {
        // -- first row tiles --
        /* 32 to 35 */
        {BBlack+FWhite, BBlack+FWhite, BBlack+FWhite, BBlack+FWhite, BBlack+FWhite, BBlack+FWhite, BBlack+FWhite, BBlack+FWhite},

        // -- side wall tiles --
        // lighted face (left)
        // 36 and 37, 40
        {BWhite+FLightBlue, BWhite+FLightBlue, BWhite+FLightBlue, BWhite+FLightBlue, BWhite+FLightBlue, BWhite+FLightBlue, BWhite+FLightBlue, BWhite+FLightBlue},
        // shaded face (right)
        // 38 and 39, 41
        {BWhite+FDarkBlue, BWhite+FDarkBlue, BWhite+FDarkBlue, BWhite+FDarkBlue, BWhite+FDarkBlue, BWhite+FDarkBlue, BWhite+FDarkBlue, BWhite+FDarkBlue},

        // -- middle row tiles --
        // 42 to 45
        {BBlack+FBlack, BBlack+FBlack, BBlack+FBlack, BBlack+FBlack, BBlack+FBlack, BBlack+FBlack, BBlack+FBlack, BBlack+FBlack},
        // 46 and 47
        {BBlack+FLightBlue, BBlack+FLightBlue, BBlack+FLightBlue, BBlack+FLightBlue, BBlack+FLightBlue, BBlack+FLightBlue, BBlack+FLightBlue, BBlack+FLightBlue},
        // 48
        {BBlack+FDarkBlue, BBlack+FDarkBlue, BBlack+FDarkBlue, BBlack+FDarkBlue, BBlack+FDarkBlue, BBlack+FDarkBlue, BBlack+FDarkBlue, BBlack+FDarkBlue},
    };

    for (uint8_t i = 0; i < 22; ++i)
    {
        set_tile_w(i + 32,
                   font1[names1[i]], colors1[color_indexes1[i]],
                   font2[names2[i]], colors2[color_indexes2[i]]);
    }
}


static void draw_block1(uint8_t x, uint8_t y)
{
    g2_put_at (x, y, " !\"#", 4);
    g2_put_at (x, y + 1, "$%&'", 4);
    g2_put_at (x, y + 2, "(())", 4);
    g2_put_at (x, y + 3, "2345", 4);
}


static void draw_block1_pressed(uint8_t x, uint8_t y)
{
    g2_put_at (x, y, " !\"#", 4);
    g2_put_at (x, y + 1, "$%&'", 4);
    g2_put_at (x, y + 2, "(())", 4);
    g2_put_at (x, y + 3, "2345", 4);
}


static void draw_block2(uint8_t x, uint8_t y)
{
    g2_put_at (x, y, " !\"#", 4);
    g2_put_at (x, y + 1, "$%&'", 4);
    g2_put_at (x, y + 2, "(())", 4);
    g2_put_at (x, y + 3, "./01", 4);
}


void draw_scenery()
{
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
}


void init_tiles ()
{
    Tiles_init ();
    g2_set_interlaced (true);
    prepare_tileset ();
}
