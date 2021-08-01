#ifndef _LEVEL_H
#define _LEVEL_H


enum level_t {
    LEVEL_1,
    LEVEL_MAX = 12
};


void compose_level (enum level_t);

bool press_plate (uint8_t pos, uint8_t x, uint8_t y);

void finish_level ();


#endif /* _LEVEL_H */
