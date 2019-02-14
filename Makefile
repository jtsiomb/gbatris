src = $(wildcard src/*.c)
obj = $(src:.c=.o)
dep = $(obj:.o=.d)
name = gbatris
elf = $(name).elf
bin = $(name).gba
elf_mb = $(name)_mb.elf
bin_mb = $(name)_mb.gba

ARCH = arm-none-eabi

CPP = $(ARCH)-cpp
CC = $(ARCH)-gcc
AS = $(ARCH)-as
OBJCOPY = $(ARCH)-objcopy
EMU = vbam

opt = -O3 -fomit-frame-pointer -mcpu=arm7tdmi -mtune=arm7tdmi -mthumb
#dbg = -g

CFLAGS = $(opt) $(dbg) -pedantic -Wall
EMUFLAGS = -T 100 -f 1 --agb-print

.PHONY: all
all: $(bin) $(bin_mb)

$(bin): $(elf)
	$(OBJCOPY) -O binary $(elf) $(bin)
	gbafix $(bin)

$(bin_mb): $(elf_mb)
	$(OBJCOPY) -O binary $(elf_mb) $(bin_mb)
	gbafix $(bin_mb)

$(elf): $(obj)
	$(CC) -o $(elf) $(obj) -specs=gba.specs $(LDFLAGS)

$(elf_mb): $(obj)
	$(CC) -o $(elf_mb) $(obj) -specs=gba_mb.specs $(LDFLAGS)

-include $(dep)

%.d: %.c
	$(CPP) $(CFLAGS) $< -MM -MT $(@:.d=.o) >$@

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
