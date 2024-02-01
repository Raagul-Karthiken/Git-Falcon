ARMGNU ?= arm-linux-gnueabi

XCPU = -mcpu=cortex-m0

AOPS = --warn --fatal-warnings $(XCPU)
COPS = -Wall -O2 -ffreestanding -nostartfiles $(XCPU)
LOPS = -nostdlib

all : notmain.bin

clean:
	rm -f *.bin
	rm -f *.o
	rm -f *.elf
	rm -f *.list
	rm -f *.srec
	
start.o : start.s
	$(ARMGNU)-as $(AOPS) start.s -o start.o

notmain.o : notmain.c
	$(ARMGNU)-gcc $(COPS) -mthumb -c notmain.c -o notmain.o

notmain.bin :               memmap.ld start.o notmain.o
	$(ARMGNU)-ld $(LOPS) -T memmap.ld start.o notmain.o -o notmain.elf
	$(ARMGNU)-objdump -D notmain.elf > notmain.list
	$(ARMGNU)-objcopy --srec-forceS3 notmain.elf -O srec notmain.srec
	$(ARMGNU)-objcopy -O binary notmain.elf notmain.bin
