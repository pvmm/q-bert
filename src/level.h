#ifndef LEVEL_H
#define LEVEL_H

#include <stdint.h>
#include <stdbool.h>
#include "ubox.h"
#include "game.h" 


// initial level status
void init_level(void);

// Press plate defined by position pos. If all plates were pressed, return true otherwise false.
enum game_status press_plate(uint8_t pos, uint8_t x, uint8_t y);

#endif // LEVEL_H
