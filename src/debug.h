#ifndef DEBUG_H
#define DEBUG_H

#ifdef USE_DEBUG_MODE

extern uint16_t _stored_debug_mode;

// send value to debug device (control)
void _out2e(uint8_t value) __z88dk_fastcall;

// send value to debug device (data)
void _out2f(uint8_t value) __z88dk_fastcall;

void debug_mode(uint8_t mode);

void _debug_str(const char* msg);

void debug_msg(char* msg);

void _debug_num(uint16_t value);

void debug(char* msg, int value);

// pause debug device (by tcl script)
void debug_break();

#else

typedef int make_iso_compilers_happy;

#endif // USE_DEBUG_MODE

#endif // DEBUG_H
