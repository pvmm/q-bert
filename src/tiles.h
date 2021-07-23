#ifndef _TILES_H
#define _TILES_H


void init_tiles
    (
        uint8_t bg_color,
        uint8_t plate_color,
        uint8_t wall_color1,
        uint8_t wall_color2
    );


void set_tile (uint8_t index,
               const U8x8 shape0, const U8x8 color0,
               const U8x8 shape1, const U8x8 color1);


void draw_block_00 (uint8_t x, uint8_t y);


void draw_block_01 (uint8_t x, uint8_t y);


void draw_block_10 (uint8_t x, uint8_t y);


void draw_block_11 (uint8_t x, uint8_t y);


void draw_block_end (uint8_t x, uint8_t y);


void draw_block_00_pressed (uint8_t x, uint8_t y);


void draw_block_01_pressed (uint8_t x, uint8_t y);


void draw_block_10_pressed (uint8_t x, uint8_t y);


void draw_block_11_pressed (uint8_t x, uint8_t y);


#endif /* _TILES_H */
