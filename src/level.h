#ifndef _LEVEL_H
#define _LEVEL_H


enum level_t {
    LEVEL_1,
    LEVEL_MAX = 12
};


void compose_level (enum level_t);

void press_plate (uint8_t x, uint8_t y);


#endif /* _LEVEL_H */
