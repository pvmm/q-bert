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


T_M2_MS_Font main_font; // Big structures, like this one, its better to store them in global storage

static void init_sprites() {

    static const U8x8 sprites[] = { {
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000
    }, {
	0b00000000,
	0b00000000,
	0b00000000,
	0b00011000,
	0b00011000,
	0b00000000,
	0b00000000,
	0b00000000
    }, {
	0b00000000,
	0b00000000,
	0b00011000,
	0b00100100,
	0b00100100,
	0b00011000,
	0b00000000,
	0b00000000
    }, {
	0b00000000,
	0b00100100,
	0b00000000,
	0b10000001,
	0b00000000,
	0b10000001,
	0b00000000,
	0b00100100
    }};


    uint8_t i=0;
    for (i=0; i<32; i++) {
	SA0[i].y = SA1[i].y = i*6;
	SA0[i].x = SA1[i].x = rand16();

	SA0[i].pattern = (255-3*i)&0x1F;
	SA0[i].color = BCyan;

	SA1[i].pattern = (255-3*i)&0x1F;
	SA1[i].color = BDarkRed;
    }

    for (i=0; i<20; i++)
	TMS99X8_writeSprite8(i,sprites[0]);

    for (i=20; i<22; i++)
	TMS99X8_writeSprite8(i,sprites[1]);

    for (i=22; i<25; i++)
	TMS99X8_writeSprite8(i,sprites[2]);

    for (i=25; i<32; i++)
	TMS99X8_writeSprite8(i,sprites[3]);

    TMS99X8_writeSpriteAttributes(MODE2_BUFFER_0,SA0); // We also use alternating buffers for the sprites.
    TMS99X8_writeSpriteAttributes(MODE2_BUFFER_1,SA1);
}


INLINE void set_font_double( // It setups charachers between 32 and 127 (inclusive)
        T_M2_MS_Font font, // Font to be setup
        EM2_RowPageFlags pages, // Pages for which this font is active
        const U8x8 shapes0[96], // first font shapes
        const U8x8 shapes1[96], // second font shapes
        const U8x8 colors0[96],
        const U8x8 colors1[96] // Colors to use for the entire font
) {
        uint8_t i;
        M2_MS_initFontTiles(font);

        for (i=0; i<96; i++) {
                M2_MS_setCharDouble(
                        font,
                        pages,
                        i + 32,
                        shapes0[i],
                        colors0[i],
                        shapes1[i],
                        colors1[i]);
        }
}

INLINE void set_tile(uint8_t index,
                     const U8x8 shape0, const U8x8 color0,
                     const U8x8 shape1, const U8x8 color1)
{
    M2_MS_setCharDouble(main_font, MODE2_ALL_ROWS, index,
                        shape0, color0,
                        shape1, color1);
}

// Here we create a monospace font (i.e., each character being 8x8 pixels). By initializing it as "Double", we use two tiles per character.
// By alternating between buffers we can obtain a larger combination of colors, as well as better color clash properties.
static void init_font() {
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
        /* 32 */ {BBlack+FLightRed, BBlack+FLightRed, BBlack+FLightRed, BBlack+FLightRed, BBlack+FLightRed, BBlack+FLightRed, BBlack+FLightRed, BBlack+FLightRed},
        /* 33 */ {BBlack+FLightRed, BBlack+FLightRed, BBlack+FLightRed, BBlack+FLightRed, BBlack+FLightRed, BBlack+FLightRed, BBlack+FLightRed, BBlack+FLightRed},
        /* 34 */ {BBlack+FLightRed, BBlack+FLightRed, BBlack+FLightRed, BBlack+FLightRed, BBlack+FLightRed, BBlack+FLightRed, BBlack+FLightRed, BBlack+FLightRed},
        /* 35 */ {BBlack+FLightRed, BBlack+FLightRed, BBlack+FLightRed, BBlack+FLightRed, BBlack+FLightRed, BBlack+FLightRed, BBlack+FLightRed, BBlack+FLightRed},
        /* lower */
        /* 36 */ {BBlack+FLightRed, BBlack+FLightRed, BBlack+FLightRed, BBlack+FLightRed, BBlack+FLightRed, BBlack+FLightRed, BBlack+FLightRed, BBlack+FLightRed},
        /* 37 */ {BBlack+FLightRed, BBlack+FLightRed, BBlack+FLightRed, BBlack+FLightRed, BBlack+FLightRed, BBlack+FLightRed, BBlack+FLightRed, BBlack+FLightRed},
        /* 38 */ {BBlack+FLightRed, BBlack+FLightRed, BBlack+FLightRed, BBlack+FLightRed, BBlack+FLightRed, BBlack+FLightRed, BBlack+FLightRed, BBlack+FLightRed},
        /* 39 */ {BBlack+FLightRed, BBlack+FLightRed, BBlack+FLightRed, BBlack+FLightRed, BBlack+FLightRed, BBlack+FLightRed, BBlack+FLightRed, BBlack+FLightRed},

        /* -- square tiles -- */
        // lighted square
        /* 40 */ {BBlack+FBlack, BBlack+FBlack, BBlack+FBlack, BBlack+FBlack, BBlack+FBlack, BBlack+FBlack, BBlack+FBlack, BBlack+FBlack},
        // shaded square
        /* 41 */ {BBlack+FBlack, BBlack+FBlack, BBlack+FBlack, BBlack+FBlack, BBlack+FBlack, BBlack+FBlack, BBlack+FBlack, BBlack+FBlack},

        // -- middle row tiles --
        // lighted face tiles (left)
        /* 42 */ {BBlack+FLightBlue, BBlack+FLightBlue, BBlack+FLightBlue, BBlack+FLightBlue, BBlack+FLightBlue, BBlack+FLightBlue, BBlack+FLightBlue, BBlack+FLightBlue},
        /* 43 */ {BBlack+FLightBlue, BBlack+FLightBlue, BBlack+FLightBlue, BBlack+FLightBlue, BBlack+FLightBlue, BBlack+FLightBlue, BBlack+FLightBlue, BBlack+FLightBlue},
        // shaded face tiles (right)
        /* 44 */ {BBlack+FDarkBlue, BBlack+FDarkBlue, BBlack+FDarkBlue, BBlack+FDarkBlue, BBlack+FDarkBlue, BBlack+FDarkBlue, BBlack+FDarkBlue, BBlack+FDarkBlue},
        /* 45 */ {BBlack+FDarkBlue, BBlack+FDarkBlue, BBlack+FDarkBlue, BBlack+FDarkBlue, BBlack+FDarkBlue, BBlack+FDarkBlue, BBlack+FDarkBlue, BBlack+FDarkBlue},

        // -- last row tiles --
        // lighted face tiles (left)
        /* 46 */ {BBlack+FBlack, BBlack+FBlack, BBlack+FBlack, BBlack+FBlack, BBlack+FBlack, BBlack+FBlack, BBlack+FBlack, BBlack+FBlack},
        /* 47 */ {BBlack+FBlack, BBlack+FBlack, BBlack+FBlack, BBlack+FBlack, BBlack+FBlack, BBlack+FBlack, BBlack+FBlack, BBlack+FBlack},
        /* 48 */ {BBlack+FBlack, BBlack+FBlack, BBlack+FBlack, BBlack+FBlack, BBlack+FBlack, BBlack+FBlack, BBlack+FBlack, BBlack+FBlack},
        /* 49 */ {BBlack+FBlack, BBlack+FBlack, BBlack+FBlack, BBlack+FBlack, BBlack+FBlack, BBlack+FBlack, BBlack+FBlack, BBlack+FBlack},

        // {BBlack+FLightBlue, BBlack+FLightBlue, BBlack+FLightBlue, BBlack+FLightBlue, BBlack+FLightBlue, BBlack+FLightBlue, BBlack+FLightBlue, BBlack+FLightBlue},
        // {BBlack+FDarkBlue, BBlack+FDarkBlue, BBlack+FDarkBlue, BBlack+FDarkBlue, BBlack+FDarkBlue, BBlack+FDarkBlue, BBlack+FDarkBlue, BBlack+FDarkBlue},
        /* -- connection tiles -- */
        /* upper */
        /* 50 */ {BBlack+FLightRed, BBlack+FLightRed, BBlack+FLightRed, BBlack+FLightRed, BBlack+FLightRed, BBlack+FLightRed, BBlack+FLightRed, BBlack+FLightRed},
        /* 51 */ {BBlack+FLightRed, BBlack+FLightRed, BBlack+FLightRed, BBlack+FLightRed, BBlack+FLightRed, BBlack+FLightRed, BBlack+FLightRed, BBlack+FLightRed},
        /* 52 */ {BBlack+FLightRed, BBlack+FLightRed, BBlack+FLightRed, BBlack+FLightRed, BBlack+FLightRed, BBlack+FLightRed, BBlack+FLightRed, BBlack+FLightRed},
        /* 53 */ {BBlack+FLightRed, BBlack+FLightRed, BBlack+FLightRed, BBlack+FLightRed, BBlack+FLightRed, BBlack+FLightRed, BBlack+FLightRed, BBlack+FLightRed},
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

        // {BBlack+FBlack, BBlack+FBlack, BBlack+FBlack, BBlack+FBlack, BBlack+FBlack, BBlack+FBlack, BBlack+FBlack, BBlack+FBlack},
        // {BBlack+FBlack, BBlack+FBlack, BBlack+FBlack, BBlack+FBlack, BBlack+FBlack, BBlack+FBlack, BBlack+FBlack, BBlack+FBlack},
        // {BBlack+FBlack, BBlack+FBlack, BBlack+FBlack, BBlack+FBlack, BBlack+FBlack, BBlack+FBlack, BBlack+FBlack, BBlack+FBlack},
        // {BBlack+FBlack, BBlack+FBlack, BBlack+FBlack, BBlack+FBlack, BBlack+FBlack, BBlack+FBlack, BBlack+FBlack, BBlack+FBlack},
    };

    for (uint8_t i = 0; i < 22; ++i) {
        printf("i = %i (%c)\n", i + 32, i + 32);
        set_tile(i + 32,
                 font1[names1[i]], colors1[i],
                 font2[names2[i]], colors2[i]);
    }

    //static const U8x8 color1 = {BBlack+FDarkBlue, BBlack+FMagenta, BBlack+FMediumRed, BBlack+FLightRed, BBlack+FDarkYellow, BBlack+FLightBlue, BBlack+FDarkBlue, BBlack+FCyan};
    //static const U8x8 color0 = {BBlack+FLightRed, BBlack+FLightRed, BBlack+FLightRed, BBlack+FLightRed, BBlack+FLightRed, BBlack+FLightRed, BBlack+FLightRed, BBlack+FLightRed};
    //static const U8x8 color1 = {BWhite+FLightBlue, BWhite+FLightBlue, BWhite+FLightBlue, BBlack+FLightBlue, BBlack+FLightBlue, BBlack+FLightBlue, BBlack+FLightBlue, BBlack+FLightBlue};

    // set_font_double(main_font,
    //   MODE2_ALL_ROWS,
    //   font1, font2,
    //   colors0, colors1
    // );
}


/* OK */
static void draw_block2(uint8_t x, uint8_t y)
{
    M2_MS_printAt(main_font, x, y, " !\"#");
    M2_MS_printAt(main_font, x, y + 1, "$%&'");
    M2_MS_printAt(main_font, x, y + 2, "(())");
    M2_MS_printAt(main_font, x, y + 3, "./01");
}

/* FIXME */
static void draw_block1(uint8_t x, uint8_t y)
{
    M2_MS_printAt(main_font, x, y, " !\"#");
    M2_MS_printAt(main_font, x, y + 1, "$%&'");
    M2_MS_printAt(main_font, x, y + 2, "(())");
    M2_MS_printAt(main_font, x, y + 3, "2345");
}


int main(void) {

    // Normal initialization routine
    msxhal_init(); // Bare minimum initialization of the msx support
    Tiles_init(); // It initializes the global tile storage (i.e., to allow to load a font afterwards)
    TMS99X8_activateMode2(MODE2_ALL_ROWS); // Activates mode 2 and clears the screen (in black)

    init_font();

    // We also show a few sprites along the way :)
    init_sprites();

    // Set the hello world, also getting some information from the BIOS (i.e., the frequency of this MSX).
    if (msxhal_get_interrupt_frequency()==MSX_FREQUENCY_60HZ) {
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
    else
    {
        M2_MS_printAt(main_font, 0, 5, "This is a 50Hz MSX! Hello World!");
    }

    // Main loop, we alternate between buffers at each interruption.
    while (true) {

	wait_frame();

	debugBorder(BMagenta);

	TMS99X8_activateBuffer(MODE2_BUFFER_0);
	// We select buffer 0, we modify sprites on buffer 1
	debugBorder(BWhite);

	TMS99X8_writeSpriteAttributes(MODE2_BUFFER_1,SA1);
	{
	    uint8_t i=0;
	    for (i=0; i<32; i++) {
		uint8_t y = SA1[i].y, p = SA1[i].pattern;
		SA1[i].y = (y==24*8?247:y+1);
		SA1[i].pattern = (p==32?0:p+1);
		if (p==1) SA0[i].x = SA1[i].x = rand16();
	    }
	}
	debugBorder(BTransparent);

	wait_frame();

	// We select buffer 1, so we modify sprites on buffer 0
	TMS99X8_activateBuffer(MODE2_BUFFER_1);
	{
	    uint8_t i=0;
	    for (i=0; i<32; i++) {
		SA0[i].y = SA1[i].y;
		SA0[i].x = SA1[i].x;
		SA0[i].pattern = SA1[i].pattern;

	    }
	}
	TMS99X8_writeSpriteAttributes(MODE2_BUFFER_0,SA0);
    };

    return 0;
}
