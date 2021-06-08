// If DEBUG is defined, the function debugBorder changes the border color, otherwise it does nothing.
//#define DEBUG TRUE

#include <msxhal.h>
#include <tms99X8.h>
#include <monospace.h>
#include <tile.h>
#include <rand.h>

T_SA SA0, SA1;

#include <res/fonts/font1.png.h>
#include <res/fonts/font2.png.h>

#include "qbert.h"
#include "sprites.h"
#include "lookup_tables.h"

#define NUM_SPRITES 4

#define tiles_init Tiles_init

T_M2_MS_Font main_font; // Big structures, like this one, its better to store them in global storage

//extern uint8_t _jump_down_right_length;

extern struct Qbert qbert;
const int16_t (*jump_table)[16];


static void init_sprites()
{
    // 0: facing down, 1: facing up
    for (int j = 0; j < 2; ++j)
    {
        for (int i = 0; i < NUM_SPRITES; i++)
        {
            // j = 0: down right
            // 0,1,2,3
            // 4,5,6,7
            // 8,9,10,11
            // 12,13,14,15
            // j = 1: up right
            // 32,33,34,35
            // 36,37,38,39
            // 40,41,42,42
            // 44,45,46,47
            TMS99X8_writeSprite8(j * 32 + i * 4 + 0, &sprites[j][i][0]);
            TMS99X8_writeSprite8(j * 32 + i * 4 + 1, &sprites[j][i][8]);
            TMS99X8_writeSprite8(j * 32 + i * 4 + 2, &sprites[j][i][16]);
            TMS99X8_writeSprite8(j * 32 + i * 4 + 3, &sprites[j][i][24]);

	    // flipped (down left)
	    // 16,17,18,19
            // 20,21,22,23
            // 24,25,26,27
            // 28,29,30,31
            // j = 1 (up left)
            // 48,49,50,51
            // 52,53,54,55
            // 56,57,58,59
            // 60,61,62,63
            TMS99X8_writeSprite8_flip(j * 32 + i * 4 + 16, &sprites[j][i][16]);
            TMS99X8_writeSprite8_flip(j * 32 + i * 4 + 17, &sprites[j][i][24]);
            TMS99X8_writeSprite8_flip(j * 32 + i * 4 + 18, &sprites[j][i][0]);
            TMS99X8_writeSprite8_flip(j * 32 + i * 4 + 19, &sprites[j][i][8]);
	}
    }
}


static void put_sprite(uint8_t x, uint8_t y, enum Direction dir)
{
    uint8_t colors0[] = {BWhite, BBlack, BLightRed, BDarkRed,};
    uint8_t colors1[] = {BWhite, BBlack, BLightRed, BDarkRed,};
    uint8_t i, j;

    if (dir & DIR_LEFT)
    {
        x -= 4;
    }

    for (j = 0, i = 0; i < NUM_SPRITES * 4; i += 4, j++)
    {
        SA0[i + 0].x = SA1[i + 0].x = x;
        SA0[i + 1].x = SA1[i + 1].x = x;
        SA0[i + 2].x = SA1[i + 2].x = x + 8;
        SA0[i + 3].x = SA1[i + 3].x = x + 8;
        SA0[i + 0].y = SA1[i + 0].y = y;
        SA0[i + 1].y = SA1[i + 1].y = y + 8;
        SA0[i + 2].y = SA1[i + 2].y = y;
        SA0[i + 3].y = SA1[i + 3].y = y + 8;

	if (dir == DIR_DOWN_RIGHT)
	{
            SA0[i + 0].pattern = SA1[i + 0].pattern = i + 0;
            SA0[i + 1].pattern = SA1[i + 1].pattern = i + 1;
            SA0[i + 2].pattern = SA1[i + 2].pattern = i + 2;
            SA0[i + 3].pattern = SA1[i + 3].pattern = i + 3;
	}
	else if (dir == DIR_DOWN_LEFT)
	{
            SA0[i + 0].pattern = SA1[i + 0].pattern = i + 16;
            SA0[i + 1].pattern = SA1[i + 1].pattern = i + 17;
            SA0[i + 2].pattern = SA1[i + 2].pattern = i + 18;
            SA0[i + 3].pattern = SA1[i + 3].pattern = i + 19;
	}
	else if (dir == DIR_UP_RIGHT)
	{
            SA0[i + 0].pattern = SA1[i + 0].pattern = i + 32;
            SA0[i + 1].pattern = SA1[i + 1].pattern = i + 33;
            SA0[i + 2].pattern = SA1[i + 2].pattern = i + 34;
            SA0[i + 3].pattern = SA1[i + 3].pattern = i + 35;
	}
	else if (dir == DIR_UP_LEFT)
	{
            SA0[i + 0].pattern = SA1[i + 0].pattern = i + 48;
            SA0[i + 1].pattern = SA1[i + 1].pattern = i + 49;
            SA0[i + 2].pattern = SA1[i + 2].pattern = i + 50;
            SA0[i + 3].pattern = SA1[i + 3].pattern = i + 51;
	}

        SA0[i].color = SA0[i + 1].color = SA0[i + 2].color = SA0[i + 3].color = colors0[j];
        SA1[i].color = SA1[i + 1].color = SA1[i + 2].color = SA1[i + 3].color = colors1[j];
    }

    TMS99X8_writeSpriteAttributes(MODE2_BUFFER_0, SA0);
    TMS99X8_writeSpriteAttributes(MODE2_BUFFER_1, SA1);
}


INLINE void set_tile(uint8_t index,
                     const U8x8 shape0, const U8x8 color0,
                     const U8x8 shape1, const U8x8 color1)
{
    M2_MS_setCharDouble(main_font, MODE2_ALL_ROWS, index,
                        shape0, color0,
                        shape1, color1);
}


static void init_font()
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


static void draw_scenary()
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


static void update_player_l(int8_t fps) __z88dk_fastcall
{
    UNUSED(fps);

    if (qbert.frame < 16)
    {
        qbert.y = qbert.y0 + (*jump_table)[qbert.frame];
        qbert.x = qbert.x0 - ++qbert.frame;
    }
    else
    {
        // reset animation status
        qbert.x0 = qbert.x;
        qbert.y0 = qbert.y;
        qbert.frame = 0;
        qbert.update = nullptr;
    }
}


static void update_player_r(int8_t fps) __z88dk_fastcall
{
    UNUSED(fps);

    if (qbert.frame < 16)
    {
        qbert.y = qbert.y0 + (*jump_table)[qbert.frame];
        qbert.x = qbert.x0 + ++qbert.frame;
    }
    else
    {
        // reset animation status
        qbert.x0 = qbert.x;
        qbert.y0 = qbert.y;
        qbert.frame = 0;
        qbert.update = nullptr;
    }
}


int main(void)
{
    uint8_t input;

    // Normal initialization routine
    msxhal_init(); // Bare minimum initialization of the msx support
    tiles_init(); // It initializes the global tile storage (i.e., to allow to load a font afterwards)
    TMS99X8_activateMode2(MODE2_ALL_ROWS); // Activates mode 2 and clears the screen (in black)

    init_font();

    // We also show a few sprites along the way :)
    init_sprites();

    // Set the hello world, also getting some information from the BIOS (i.e., the frequency of this MSX).
    draw_scenary();

    put_sprite(qbert.x0, qbert.y0, DIR_DOWN_RIGHT);

    // Main loop, we alternate between buffers at each interruption.
    while (true)
    {
        input = msxhal_joystick_read(0);

        if (qbert.update == nullptr)
        {
            switch (input)
            {
            case 16: // left
            case 32: // up
            case 64: // down
            case 128: // right
                goto cycle_frames;

            case DIR_DOWN_RIGHT:
                qbert.direction = DIR_DOWN_RIGHT;
                jump_table = &jump_down;
                qbert.update = update_player_r;
                break;

            case DIR_DOWN_LEFT:
                qbert.direction = DIR_DOWN_LEFT;
                jump_table = &jump_down;
                qbert.update = update_player_l;
                break;

            case DIR_UP_RIGHT:
                qbert.direction = DIR_UP_RIGHT;
                jump_table = &jump_up;
                qbert.update = update_player_r;
                break;

            case DIR_UP_LEFT:
                qbert.direction = DIR_UP_LEFT;
                jump_table = &jump_up;
                qbert.update = update_player_l;
                break;

            default:
                goto cycle_frames;
            }
        }

        qbert.update(1);
        put_sprite(qbert.x, qbert.y, qbert.direction);

cycle_frames:
        wait_frame();

        //debugBorder(BMagenta);

        TMS99X8_activateBuffer(MODE2_BUFFER_0);

        // We select buffer 0, we modify sprites on buffer 1
        //debugBorder(BWhite);

        wait_frame();

        // We select buffer 1, so we modify sprites on buffer 0
        TMS99X8_activateBuffer(MODE2_BUFFER_1);
    };

    return 0;
}
