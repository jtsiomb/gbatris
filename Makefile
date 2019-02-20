src = $(wildcard src/*.c)
ssrc = $(wildcard src/*.s)
dataobj = data/scorescr.o data/namescr.o
obj = $(src:.c=.o) $(ssrc:.s=.o) $(dataobj)
dep = $(src:.c=.d)
name = gbatris
elf = $(name).elf
bin = $(name).gba

ARCH = arm-none-eabi-

CPP = $(ARCH)cpp
CC = $(ARCH)gcc
AS = $(ARCH)as
OBJCOPY = $(ARCH)objcopy
EMU = vbam

opt = -O1 -fomit-frame-pointer -mcpu=arm7tdmi -mtune=arm7tdmi -mthumb -mthumb-interwork
#dbg = -g

CFLAGS = $(opt) $(dbg) -pedantic -Wall
ASFLAGS = -mthumb-interwork
LDFLAGS = -mthumb -mthumb-interwork
EMUFLAGS = -T 100 -f 1 --agb-print

IMG2TILES = tools/img2tiles/img2tiles


.PHONY: all
all: $(bin) $(bin_mb)

$(bin): $(elf)
	$(OBJCOPY) -O binary $(elf) $(bin)
	gbafix $(bin)

$(elf): $(obj)
	$(CC) -o $(elf) $(obj) -specs=gba.specs $(LDFLAGS)

-include $(dep)

src/data.o: src/data.s data/msglogo.img data/music

%.d: %.c
	$(CPP) $(CFLAGS) $< -MM -MT $(@:.d=.o) >$@

%.c: %.png $(IMG2TILES)
	$(IMG2TILES) -o $@ -c $<

$(IMG2TILES):
	$(MAKE) -C tools/img2tiles

.PHONY: clean
clean:
	rm -f $(obj) $(dep) $(bin) $(bin_mb) $(elf) $(elf_mb)

.PHONY: install
install: $(bin)
	if2a -n -f -W $<

.PHONY: run
run: $(bin_mb)
	if2a -m $<

.PHONY: simrun
simrun: $(bin)
	$(EMU) $(EMUFLAGS) $(bin)

.PHONY: disasm
disasm: $(elf)
	$(ARCH)objdump -d $< >$@
