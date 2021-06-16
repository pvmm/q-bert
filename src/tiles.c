#include <stdint.h>
#include <msxhal.h>
#include <monospace.h>
#include <tile.h>

#include <res/fonts/font1.png.h>
#include <res/fonts/font2.png.h>


static T_M2_MS_Font main_font; // Big structures, like this one, its better to store them in global storage


void tiles_init()
{
    Tiles_init();
}


INLINE void set_tile(uint8_t index,
                     const U8x8 shape0, const U8x8 color0,
                     const U8x8 shape1, const U8x8 color1)
{
    M2_MS_setCharDouble(main_font, MODE2_ALL_ROWS, index,
                        shape0, color0,
                        shape1, color1);
}


void init_font()
{
    M2_MS_initFontTiles(main_font);

    static const int names1[] = {
        0, 1, 2, 3,     // first row tiles (upper)
        4, 5, 6, 7,     // first row tiles (lower)
        12, 12,         // square tiles (lighted and shaded)
        0, 1, 2, 3,     // middle row tiles
        12, 12, 12, 12, // last row of tiles (empty)
        2, 3, 0, 1,     // connection tiles

    };
    static const U8x8 colors1[] = {
        /* -- first row tiles -- */
        /* upper */
        /* 32 */
        {BBlack+FLightRed, BBlack+FLightRed, BBlack+FLightRed, BBlack+FLightRed, BBlack+FLightRed, BBlack+FLightRed, BBlack+FLightRed, BBlack+FLightRed},
        /* 33 */
        {BBlack+FLightRed, BBlack+FLightRed, BBlack+FLightRed, BBlack+FLightRed, BBlack+FLightRed, BBlack+FLightRed, BBlack+FLightRed, BBlack+FLightRed},
        /* 34 */
        {BBlack+FLightRed, BBlack+FLightRed, BBlack+FLightRed, BBlack+FLightRed, BBlack+FLightRed, BBlack+FLightRed, BBlack+FLightRed, BBlack+FLightRed},
        /* 35 */
        {BBlack+FLightRed, BBlack+FLightRed, BBlack+FLightRed, BBlack+FLightRed, BBlack+FLightRed, BBlack+FLightRed, BBlack+FLightRed, BBlack+FLightRed},
        /* lower */
        /* 36 */
        {BBlack+FLightRed, BBlack+FLightRed, BBlack+FLightRed, BBlack+FLightRed, BBlack+FLightRed, BBlack+FLightRed, BBlack+FLightRed, BBlack+FLightRed},
        /* 37 */
        {BBlack+FLightRed, BBlack+FLightRed, BBlack+FLightRed, BBlack+FLightRed, BBlack+FLightRed, BBlack+FLightRed, BBlack+FLightRed, BBlack+FLightRed},
        /* 38 */
        {BBlack+FLightRed, BBlack+FLightRed, BBlack+FLightRed, BBlack+FLightRed, BBlack+FLightRed, BBlack+FLightRed, BBlack+FLightRed, BBlack+FLightRed},
        /* 39 */
        {BBlack+FLightRed, BBlack+FLightRed, BBlack+FLightRed, BBlack+FLightRed, BBlack+FLightRed, BBlack+FLightRed, BBlack+FLightRed, BBlack+FLightRed},

        /* -- square tiles -- */
        // lighted square
        /* 40 */
        {BBlack+FBlack, BBlack+FBlack, BBlack+FBlack, BBlack+FBlack, BBlack+FBlack, BBlack+FBlack, BBlack+FBlack, BBlack+FBlack},
        // shaded square
        /* 41 */
        {BBlack+FBlack, BBlack+FBlack, BBlack+FBlack, BBlack+FBlack, BBlack+FBlack, BBlack+FBlack, BBlack+FBlack, BBlack+FBlack},

        // -- middle row tiles --
        // lighted face tiles (left)
        /* 42 */
        {BBlack+FLightBlue, BBlack+FLightBlue, BBlack+FLightBlue, BBlack+FLightBlue, BBlack+FLightBlue, BBlack+FLightBlue, BBlack+FLightBlue, BBlack+FLightBlue},
        /* 43 */
        {BBlack+FLightBlue, BBlack+FLightBlue, BBlack+FLightBlue, BBlack+FLightBlue, BBlack+FLightBlue, BBlack+FLightBlue, BBlack+FLightBlue, BBlack+FLightBlue},
        // shaded face tiles (right)
        /* 44 */
        {BBlack+FDarkBlue, BBlack+FDarkBlue, BBlack+FDarkBlue, BBlack+FDarkBlue, BBlack+FDarkBlue, BBlack+FDarkBlue, BBlack+FDarkBlue, BBlack+FDarkBlue},
        /* 45 */
        {BBlack+FDarkBlue, BBlack+FDarkBlue, BBlack+FDarkBlue, BBlack+FDarkBlue, BBlack+FDarkBlue, BBlack+FDarkBlue, BBlack+FDarkBlue, BBlack+FDarkBlue},

        // -- last row tiles --
        // lighted face tiles (left)
        /* 46 */
        {BBlack+FBlack, BBlack+FBlack, BBlack+FBlack, BBlack+FBlack, BBlack+FBlack, BBlack+FBlack, BBlack+FBlack, BBlack+FBlack},
        /* 47 */
        {BBlack+FBlack, BBlack+FBlack, BBlack+FBlack, BBlack+FBlack, BBlack+FBlack, BBlack+FBlack, BBlack+FBlack, BBlack+FBlack},
        /* 48 */
        {BBlack+FBlack, BBlack+FBlack, BBlack+FBlack, BBlack+FBlack, BBlack+FBlack, BBlack+FBlack, BBlack+FBlack, BBlack+FBlack},
        /* 49 */
        {BBlack+FBlack, BBlack+FBlack, BBlack+FBlack, BBlack+FBlack, BBlack+FBlack, BBlack+FBlack, BBlack+FBlack, BBlack+FBlack},

        // {BBlack+FLightBlue, BBlack+FLightBlue, BBlack+FLightBlue, BBlack+FLightBlue, BBlack+FLightBlue, BBlack+FLightBlue, BBlack+FLightBlue, BBlack+FLightBlue},
        // {BBlack+FDarkBlue, BBlack+FDarkBlue, BBlack+FDarkBlue, BBlack+FDarkBlue, BBlack+FDarkBlue, BBlack+FDarkBlue, BBlack+FDarkBlue, BBlack+FDarkBlue},
        /* -- connection tiles -- */
        /* upper */
        /* 50 */
        {BBlack+FLightRed, BBlack+FLightRed, BBlack+FLightRed, BBlack+FLightRed, BBlack+FLightRed, BBlack+FLightRed, BBlack+FLightRed, BBlack+FLightRed},
        /* 51 */
        {BBlack+FLightRed, BBlack+FLightRed, BBlack+FLightRed, BBlack+FLightRed, BBlack+FLightRed, BBlack+FLightRed, BBlack+FLightRed, BBlack+FLightRed},
        /* 52 */
        {BBlack+FLightRed, BBlack+FLightRed, BBlack+FLightRed, BBlack+FLightRed, BBlack+FLightRed, BBlack+FLightRed, BBlack+FLightRed, BBlack+FLightRed},
        /* 53 */
        {BBlack+FLightRed, BBlack+FLightRed, BBlack+FLightRed, BBlack+FLightRed, BBlack+FLightRed, BBlack+FLightRed, BBlack+FLightRed, BBlack+FLightRed},
    };

    static const int names2[] = {
        0, 1, 2, 3,     // tiles from first row
        4, 5, 6, 7,     // tiles from second row
        12, 12,         // square tiles (lighted and shaded)
        4, 5, 6, 7,     // middle row tiles
        8, 9, 10, 11,   // last row tiles
        8, 9, 10, 11,   // connection tiles
        //9, 10, 11, 12   // connection tiles
    };
    static const U8x8 colors2[] = {
        // -- first row tiles --
        /* 32 */ {BBlack+FWhite, BBlack+FWhite, BBlack+FWhite, BBlack+FWhite, BBlack+FWhite, BBlack+FWhite, BBlack+FWhite, BBlack+FWhite},
        /* 33 */ {BBlack+FWhite, BBlack+FWhite, BBlack+FWhite, BBlack+FWhite, BBlack+FWhite, BBlack+FWhite, BBlack+FWhite, BBlack+FWhite},
        /* 34 */ {BBlack+FWhite, BBlack+FWhite, BBlack+FWhite, BBlack+FWhite, BBlack+FWhite, BBlack+FWhite, BBlack+FWhite, BBlack+FWhite},
        /* 35 */ {BBlack+FWhite, BBlack+FWhite, BBlack+FWhite, BBlack+FWhite, BBlack+FWhite, BBlack+FWhite, BBlack+FWhite, BBlack+FWhite},

        // -- side wall tiles --
        // lighted face (left)
        /* 36 */ {BWhite+FLightBlue, BWhite+FLightBlue, BWhite+FLightBlue, BWhite+FLightBlue, BWhite+FLightBlue, BWhite+FLightBlue, BWhite+FLightBlue, BWhite+FLightBlue},
        /* 37 */ {BWhite+FLightBlue, BWhite+FLightBlue, BWhite+FLightBlue, BWhite+FLightBlue, BWhite+FLightBlue, BWhite+FLightBlue, BWhite+FLightBlue, BWhite+FLightBlue},
        // shaded face (right)
        /* 38 */ {BWhite+FDarkBlue, BWhite+FDarkBlue, BWhite+FDarkBlue, BWhite+FDarkBlue, BWhite+FDarkBlue, BWhite+FDarkBlue, BWhite+FDarkBlue, BWhite+FDarkBlue},
        /* 39 */ {BWhite+FDarkBlue, BWhite+FDarkBlue, BWhite+FDarkBlue, BWhite+FDarkBlue, BWhite+FDarkBlue, BWhite+FDarkBlue, BWhite+FDarkBlue, BWhite+FDarkBlue},

        // -- square tiles --
        // lighted square
        /* 40 */ {BWhite+FLightBlue, BWhite+FLightBlue, BWhite+FLightBlue, BWhite+FLightBlue, BWhite+FLightBlue, BWhite+FLightBlue, BWhite+FLightBlue, BWhite+FLightBlue},
        // shaded square
        /* 41 */ {BWhite+FDarkBlue, BWhite+FDarkBlue, BWhite+FDarkBlue, BWhite+FDarkBlue, BWhite+FDarkBlue, BWhite+FDarkBlue, BWhite+FDarkBlue, BWhite+FDarkBlue},

        // -- middle row tiles --
        /* 42 */ {BBlack+FBlack, BBlack+FBlack, BBlack+FBlack, BBlack+FBlack, BBlack+FBlack, BBlack+FBlack, BBlack+FBlack, BBlack+FBlack},
        /* 43 */ {BBlack+FBlack, BBlack+FBlack, BBlack+FBlack, BBlack+FBlack, BBlack+FBlack, BBlack+FBlack, BBlack+FBlack, BBlack+FBlack},
        /* 44 */ {BBlack+FBlack, BBlack+FBlack, BBlack+FBlack, BBlack+FBlack, BBlack+FBlack, BBlack+FBlack, BBlack+FBlack, BBlack+FBlack},
        /* 45 */ {BBlack+FBlack, BBlack+FBlack, BBlack+FBlack, BBlack+FBlack, BBlack+FBlack, BBlack+FBlack, BBlack+FBlack, BBlack+FBlack},

        // last row tiles
        /* 46 */ {BBlack+FLightBlue, BBlack+FLightBlue, BBlack+FLightBlue, BBlack+FLightBlue, BBlack+FLightBlue, BBlack+FLightBlue, BBlack+FLightBlue, BBlack+FLightBlue},
        /* 47 */ {BBlack+FLightBlue, BBlack+FLightBlue, BBlack+FLightBlue, BBlack+FLightBlue, BBlack+FLightBlue, BBlack+FLightBlue, BBlack+FLightBlue, BBlack+FLightBlue},
        /* 48 */ {BBlack+FDarkBlue, BBlack+FDarkBlue, BBlack+FDarkBlue, BBlack+FDarkBlue, BBlack+FDarkBlue, BBlack+FDarkBlue, BBlack+FDarkBlue, BBlack+FDarkBlue},
        /* 49 */ {BBlack+FDarkBlue, BBlack+FDarkBlue, BBlack+FDarkBlue, BBlack+FDarkBlue, BBlack+FDarkBlue, BBlack+FDarkBlue, BBlack+FDarkBlue, BBlack+FDarkBlue},

        // lighted face (left)
        /* 50 */ {BWhite+FLightBlue, BWhite+FLightBlue, BWhite+FLightBlue, BWhite+FLightBlue, BWhite+FLightBlue, BWhite+FLightBlue, BWhite+FLightBlue, BWhite+FLightBlue},
        /* 51 */ {BWhite+FLightBlue, BWhite+FLightBlue, BWhite+FLightBlue, BWhite+FLightBlue, BWhite+FLightBlue, BWhite+FLightBlue, BWhite+FLightBlue, BWhite+FLightBlue},
        // shaded face (right)
        /* 52 */ {BWhite+FDarkBlue, BWhite+FDarkBlue, BWhite+FDarkBlue, BWhite+FDarkBlue, BWhite+FDarkBlue, BWhite+FDarkBlue, BWhite+FDarkBlue, BWhite+FDarkBlue},
        /* 53 */ {BWhite+FDarkBlue, BWhite+FDarkBlue, BWhite+FDarkBlue, BWhite+FDarkBlue, BWhite+FDarkBlue, BWhite+FDarkBlue, BWhite+FDarkBlue, BWhite+FDarkBlue},
    };

    for (uint8_t i = 0; i < 22; ++i) {
        set_tile(i + 32,
                 font1[names1[i]], colors1[i],
                 font2[names2[i]], colors2[i]);
    }
}


static void draw_block1(uint8_t x, uint8_t y)
{
    M2_MS_printAt(main_font, x, y, " !\"#");
    M2_MS_printAt(main_font, x, y + 1, "$%&'");
    M2_MS_printAt(main_font, x, y + 2, "(())");
    M2_MS_printAt(main_font, x, y + 3, "2345");
}


static void draw_block2(uint8_t x, uint8_t y)
{
    M2_MS_printAt(main_font, x, y, " !\"#");
    M2_MS_printAt(main_font, x, y + 1, "$%&'");
    M2_MS_printAt(main_font, x, y + 2, "(())");
    M2_MS_printAt(main_font, x, y + 3, "./01");
}


void draw_scenary()
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
