TARGET := game

include ../../config.env

CODE = 0x4000
# leaves 222 bytes for AKM player buffer
DATA = 0xc0de
OUTDIR = ../../bin
TMPDIR = ../build

# will automatically include (and compile) any .c or .z80 file
ifdef DEBUG
  EXTRA_CFLAGS += -DUSE_DEBUG
endif

C_FILES := data.c debug.c entities.c game.c helpers.c level.c plate.c main.c
OBJS := $(patsubst %.c,$(TMPDIR)/%.rel,$(C_FILES)) $(patsubst %.z80,$(TMPDIR)/%.rel,$(wildcard *.z80))
# crt0.z80 is compiled as requirement of the targets and MUST be linked first,
# so we exclude it from the list of user-provided source
OBJS := $(filter-out $(TMPDIR)/crt0.rel,$(OBJS))

UBOX_LIBS = $(wildcard ../../lib/*.lib)
LIBS = -lubox -lspman -lmplayer -lap

CC := sdcc
AS := sdasz80
AR := sdcclib
CHKSIZE := ../../tools/chksize
CFLAGS := -mz80 --Werror -I../../include -I../generated --fsigned-char --std-sdcc99 --opt-code-speed --fomit-frame-pointer $(EXTRA_CFLAGS) --less-pedantic
LDFLAGS := -L../../lib -L. --no-std-crt0

default:
	$(error Please use the Makefile from root directory)

all: $(TMPDIR)/$(TARGET).rom
	@$(CHKSIZE) $(ROM_MAX) 4000 $(TMPDIR)/$(TARGET).map
	@cp $(TMPDIR)/$(TARGET).rom $(OUTDIR)

cas: $(TMPDIR)/$(TARGET).cas
	@cp $(TMPDIR)/$(TARGET).cas $(OUTDIR)

openmsx: all
	DEBUG=$(DEBUG) openmsx -script env.tcl

$(TMPDIR)/%.rel: %.c
	$(CC) $(CFLAGS) $(LDFLAGS) -c $< -o $@

$(TMPDIR)/%.rel: %.z80
	$(AS) -g -o $@ $<

$(TMPDIR)/akm.rel: akm.z80 song.asm effects.asm
	rasm akm.z80 -o $(TMPDIR)/akm -s -sl -sq
	Disark --sourceProfile sdcc --symbolFile $(TMPDIR)/akm.sym --src16bitsValuesInHex --src8bitsValuesInHex --undocumentedOpcodesToBytes $(TMPDIR)/akm.bin $(TMPDIR)/akm_sdcc.asm
	$(AS) -g -o $@ $(TMPDIR)/akm_sdcc.asm

$(TMPDIR)/$(TARGET).rom: $(OBJS) $(TMPDIR)/crt0.rel $(UBOX_LIBS)
	$(CC) $(CFLAGS) $(LDFLAGS) $(LIBS) --code-loc $(CODE) --data-loc $(DATA) $(TMPDIR)/crt0.rel $(OBJS) -o $(TMPDIR)/$(TARGET).ihx
	hex2bin -e bin -p 00 -l $(ROM_MAX) $(TMPDIR)/$(TARGET).ihx
	@cp $(TMPDIR)/$(TARGET).bin $(TMPDIR)/$(TARGET).rom

LOADER_ADDR=0xe000
LOADER_WORK_ADDR=0x8000
$(TMPDIR)/$(TARGET).cas: $(TMPDIR)/$(TARGET).rom cas/loader.bas $(TMPDIR)/loader.bin $(TMPDIR)/loading.ap
	apultra -v $(TMPDIR)/$(TARGET).rom $(TMPDIR)/$(TARGET).ap
	../../tools/mkcas.py --name $(TARGET) $@ ascii cas/loader.bas
	../../tools/mkcas.py -a --name loader --addr $(LOADER_ADDR) --exec $(LOADER_ADDR) $@ binary $(TMPDIR)/loader.bin
	../../tools/mkcas.py -a --addr $(LOADER_WORK_ADDR) $@ custom-header $(TMPDIR)/loading.ap
	../../tools/mkcas.py -a --addr $(LOADER_WORK_ADDR) $@ custom-header $(TMPDIR)/$(TARGET).ap

$(TMPDIR)/loader.bin: $(TMPDIR)/loader.rel
	$(CC) $(CFLAGS) $(LDFLAGS) --code-loc $(LOADER_ADDR) --data-loc 0 -lap $< -o $(TMPDIR)/loader.ihx
	hex2bin -p 00 $(TMPDIR)/loader.ihx

$(TMPDIR)/loading.ap: ../data/loading.png
	../../tools/png2scr.py ../data/loading.png > $(TMPDIR)/loading.bin
	apultra -v $(TMPDIR)/loading.bin $(TMPDIR)/loading.ap

$(TMPDIR)/loader.rel: cas/loader.z80
	$(AS) -g -o $@ $<

clean:
	rm -f $(TMPDIR)/*
	rm -f $(OUTDIR)/$(TARGET).rom
	rm -f $(OUTDIR)/$(TARGET).cas

.PHONY: all cas clean

include Makefile.deps
