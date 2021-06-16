#ifndef _QBERT_H_
#define _QBERT_H_

#pragma once
#include <stdint.h>

enum Direction {
    DIR_LEFT       =  16,
    DIR_UP         =  32,
    DIR_DOWN       =  64,
    DIR_RIGHT      = 128,
    DIR_DOWN_RIGHT = 192,	// 128 + 64
    DIR_DOWN_LEFT  =  80,	//  64 + 16
    DIR_UP_RIGHT   = 160,	// 128 + 32
    DIR_UP_LEFT    =  48,	//  32 + 16
};


struct Qbert
{
    bool dirty;
    uint8_t frame;
    uint8_t x0;
    uint8_t y0;
    uint8_t x;
    uint8_t y;
    enum Direction direction;

    void (*update)(int8_t) __z88dk_fastcall;
};

void init_qbert_sprites();

void put_qbert_sprite(EM2_Buffer BUFFER);

#endif /* _QBERT_H_ */
