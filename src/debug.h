#ifndef DEBUG_H
#define DEBUG_H

#ifdef USE_DEBUG

#define DEBUG_HEX          0
#define DEBUG_INT          1
#define DEBUG_BIN          2
#define DEBUG_CHAR         3
#define DEBUG_INVALID      4

#include <stdint.h>

extern uint16_t _stored_debug_mode;

void debug_mode(uint8_t mode);

void debug_msg(char* msg);

void debug(char* msg, int value);

// pause debug device (by tcl script)
void debug_break();

#else // USE_DEBUG

#define debug_mode(x)
#define debug_msg(x)
#define debug(x, y)
#define debug_break()

#endif // USE_DEBUG
#endif // DEBUG_H
