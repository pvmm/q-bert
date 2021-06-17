#ifndef _TILES_H
#define _TILES_H

void tiles_init();

void set_tile(uint8_t index,
              const U8x8 shape0, const U8x8 color0,
              const U8x8 shape1, const U8x8 color1);

void draw_scenery();

#endif /* _TILES_H */
