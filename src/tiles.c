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

#define WALL_PRESSED_5  "\x1b" // 27
#define WALL_PRESSED_6  "\x1c" // 28
#define WALL_PRESSED_7  "\x1d" // 29
#define WALL_PRESSED_8  "\x1e" // 30


typedef const u8x8_t *colormap_t[5];

// TODO: add more colors as parameters (bg_color)?
static colormap_t* prepare_colors
    (
        uint8_t bg_color,
        uint8_t plate_color,
        uint8_t wall_color1,
        uint8_t wall_color2
    )
{
    static colormap_t colors;
    static u8x8_t plate_ct;
    static u8x8_t bright_plate_ct;
    static u8x8_t bright_wall_ct;
    static u8x8_t dark_wall_ct;
    static u8x8_t bg_ct;

    for (int i = 0; i < 8; ++i)
        bg_ct[i] = bg_color;

    for (int i = 0; i < 8; ++i)
        bright_plate_ct[i] = (0xF & bg_color) + FLightYellow;

    for (int i = 0; i < 8; ++i)
        plate_ct[i] = plate_color;

    for (int i = 0; i < 8; ++i)
        bright_wall_ct[i] = wall_color1;

    for (int i = 0; i < 8; ++i)
        dark_wall_ct[i] = wall_color2;

    colors[0] = &bg_ct;
    colors[1] = &plate_ct;
    colors[2] = &bright_wall_ct;
    colors[3] = &dark_wall_ct;
    colors[4] = &bright_plate_ct;

    return &colors;
}

static void prepare_tileset
    (
        uint8_t bg_color,
        uint8_t plate_color,
        uint8_t wall_color1,
        uint8_t wall_color2
    )
{
    const colormap_t* colors = prepare_colors (bg_color, plate_color,
        wall_color1, wall_color2);

    static const uint8_t shape_indexes[] =
    {
        0,
        1, 2, 3, 4,     // 1..4: tiles from first row (top tiles)
        5, 6, 7, 8,     // 5..8: tiles from first row (bottom tiles)
        5, 6, 7, 8,     // 9..12: tiles from first row (bottom tiles) (2nd frame)
        0, 0,           // 13..14: square tiles (lighted and shaded)
        3, 4, 1, 2,     // 15..18: connection tiles

        /* pressed plate tiles */
        1, 2, 3, 4,     // 19..22: first row tiles (top tiles)
        5, 6, 7, 8,     // 23..26: first row tiles (bottom tiles)
    };

    static const uint8_t color_indexes[] =
    {
        0,
        1, 1, 1, 1,     // 1..4: first row tiles (top tiles)
        1, 1, 1, 1,     // 5..8: first row tiles (bottom tiles)
        2, 2, 3, 3,     // 9..12: first row tiles (bottom tiles) (2nd frame)
        2, 3,           // 13..14: square tiles (lighted and shaded)
        2, 2, 3, 3,     // 15..18: connection tiles

        /* pressed plate tiles */
        4, 4, 4, 4,     // 19..22: first row tiles (top tiles)
        4, 4, 4, 4,     // 23..26: first row tiles (bottom tiles)

        255,            // end marker
    };

    static const uint8_t assoc_tiles[][2] =
    {
        {0, 0},
        // plate (1-8)
        {1, 0}, {2, 0}, {3, 0}, {4, 0},
        {5, 9}, {6, 10}, {7, 11}, {8, 12},
        // wall tiles (9-10)
        {13, 0}, {14, 0},
        // tiles from last row (11-14)
        {15, 0}, {16, 0}, {17, 0}, {18, 0},
        // connection tiles (15-18)
        {3, 15}, {4, 16}, {1, 17}, {2, 18},
        // pressed tiles (19-26)
        {19, 0}, {20, 0}, {21, 0}, {22, 0},
        {23, 9}, {24, 10}, {25, 11}, {26, 12},
        // connection tiles pressed (27-30)
        {21, 15}, {22, 16}, {19, 17}, {20, 18},
        // end marker
        {255, 255},
    };

    for (int i = 0; i < 256; ++i)
    {
        const u8x8_t *color = (*colors)[color_indexes[i]];

        if (color_indexes[i] == 255)
            break;

        g2_set_tile(MODE2_ALL_ROWS, i, tiles[shape_indexes[i]], *color);
    }

    for (int i = 0; i < 256; ++i)
    {
        if (assoc_tiles[i][0] == 255 && assoc_tiles[i][1] == 255)
            break;

        g2_set_tile_pairing (i, assoc_tiles[i][0], assoc_tiles[i][1]);
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


static void draw_plate_00 (uint8_t x, uint8_t y)
{
    draw_plate_0_left (x, y);
    draw_plate_0_right (x, y);
}


static void draw_plate_00_pressed (uint8_t x, uint8_t y)
{
    draw_plate_0_pressed_left (x, y);
    draw_plate_0_pressed_right (x, y);
}


static void draw_plate_1_left (uint8_t x, uint8_t y)
{
    g2_put_at (x, y, WALL_7 WALL_8, 2);
    g2_put_at (x, y + 1, PLATE_5 PLATE_6, 2);
}


static void draw_plate_1_pressed_left (uint8_t x, uint8_t y)
{
    g2_put_at (x, y, WALL_PRESSED_7 WALL_PRESSED_8, 2);
    g2_put_at (x, y + 1, PLATE_PRESSED_5 PLATE_PRESSED_6, 2);
}


static void draw_plate_1_right (uint8_t x, uint8_t y)
{
    g2_put_at (x + 2, y, WALL_5 WALL_6, 2);
    g2_put_at (x + 2, y + 1, PLATE_7 PLATE_8, 2);
}


static void draw_plate_1_pressed_right (uint8_t x, uint8_t y)
{
    g2_put_at (x + 2, y, WALL_PRESSED_5 WALL_PRESSED_6, 2);
    g2_put_at (x + 2, y + 1, PLATE_PRESSED_7 PLATE_PRESSED_8, 2);
}


static void draw_plate_11 (uint8_t x, uint8_t y)
{
    draw_plate_1_left (x, y);
    draw_plate_1_right (x, y);
}


static void draw_plate_11_pressed (uint8_t x, uint8_t y)
{
    draw_plate_1_pressed_left (x, y);
    draw_plate_1_pressed_right (x, y);
}


static void draw_plate_01 (uint8_t x, uint8_t y)
{
    draw_plate_0_left (x, y);
    draw_plate_1_right (x, y);
}


static void draw_plate_01_pressed (uint8_t x, uint8_t y)
{
    draw_plate_0_pressed_left (x, y);
    draw_plate_1_pressed_right (x, y);
}


static void draw_plate_10 (uint8_t x, uint8_t y)
{
    draw_plate_1_left (x, y);
    draw_plate_0_right (x, y);
}


static void draw_plate_10_pressed (uint8_t x, uint8_t y)
{
    draw_plate_1_pressed_left (x, y);
    draw_plate_0_pressed_right (x, y);
}


void draw_block_00 (uint8_t x, uint8_t y)
{
    draw_plate_00 (x, y);
    g2_put_at (x, y + 2, WALL_1 WALL_2 WALL_3 WALL_4, 4);
}


void draw_block_00_pressed (uint8_t x, uint8_t y)
{
    draw_plate_00_pressed (x, y);
    g2_put_at (x, y + 2, WALL_1 WALL_2 WALL_3 WALL_4, 4);
}


void draw_block_01 (uint8_t x, uint8_t y)
{
    draw_plate_01 (x, y);
    g2_put_at (x, y + 2, WALL_1 WALL_2 WALL_3 WALL_4, 4);
}


void draw_block_01_pressed (uint8_t x, uint8_t y)
{
    draw_plate_01_pressed (x, y);
    g2_put_at (x, y + 2, WALL_1 WALL_2 WALL_3 WALL_4, 4);
}


void draw_block_10 (uint8_t x, uint8_t y)
{
    draw_plate_10 (x, y);
    g2_put_at (x, y + 2, WALL_1 WALL_2 WALL_3 WALL_4, 4);
}


void draw_block_10_pressed (uint8_t x, uint8_t y)
{
    draw_plate_10_pressed (x, y);
    g2_put_at (x, y + 2, WALL_1 WALL_2 WALL_3 WALL_4, 4);
}


void draw_block_11(uint8_t x, uint8_t y)
{
    draw_plate_11 (x, y);
    g2_put_at (x, y + 2, WALL_1 WALL_2 WALL_3 WALL_4, 4);
}


void draw_block_11_pressed (uint8_t x, uint8_t y)
{
    draw_plate_11_pressed (x, y);
    g2_put_at (x, y + 2, WALL_1 WALL_2 WALL_3 WALL_4, 4);
}


void draw_block_end(uint8_t x, uint8_t y)
{
    g2_put_at (x, y, WALL_END_1 WALL_END_2 WALL_END_3 WALL_END_4, 4);
}


static bool _initialized = false;


void init_tiles (uint8_t bg_color, uint8_t plate_color, uint8_t wall_color1, uint8_t wall_color2)
{
    if (!_initialized)
    {
        Tiles_init ();
        g2_init_tileset (true);
        prepare_tileset (bg_color, plate_color, wall_color1, wall_color2);
    }
    else
    {
        prepare_tileset (bg_color, plate_color, wall_color1, wall_color2);
    }
}
