#ifndef _QBERT_H_
#define _QBERT_H_

#include <stdint.h>
#include <msxhal.h>

struct Qbert
{
    uint8_t frame;
    uint8_t x0;
    uint8_t y0;
    uint8_t x;
    uint8_t y;
    bool moving;

    void (*update)(int8_t) __z88dk_fastcall;
};

#endif /* _QBERT_H_ */
