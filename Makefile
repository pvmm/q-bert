NAME := qbert

include sdcc_msx/Makefile.in

#CCZ80 = sdcc_msx/bin/sdcc-3.9.0/bin/sdcc
CCZ80 = sdcc
CCFLAGS_MSX   += -DKONAMI5
