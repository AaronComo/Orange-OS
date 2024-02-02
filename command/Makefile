# commands/Makefile

#ENTRYPOINT	= 0x1000
HD		= ../80m.img

ASM		= nasm
DASM	= objdump
CC		= x86_64-elf-gcc
LD		= x86_64-elf-ld
ASMFLAGS	= -I ../include/ -f elf
CFLAGS		= -I ../include/ -c -fno-builtin -Wall -m32 -fno-stack-protector
LDFLAGS		= -Ttext 0x1000 -m elf_i386 
DASMFLAGS	= -D
LIB		= ../lib/orangescrt.a
BIN		= echo pwd attack overflow attack_stack

# All Phony Targets
.PHONY : everything final clean realclean disasm all install

# Default starting position
everything : $(BIN)

install : all clean
	cp ../boot/hd*.bin ./ -v
	cp ../kernel.bin ./ -v
	tar vcf inst.tar kernel.bin $(BIN) hd*.bin
	dd if=inst.tar of=$(HD) seek=`echo "obase=10;ibase=16;(\`egrep -e '^ROOT_BASE' ../boot/include/load.inc | sed -e 's/.*0x//g'\`+\`egrep -e '#define[[:space:]]*INSTALL_START_SECT' ../include/sys/config.h | sed -e 's/.*0x//g'\`)*200" | bc` bs=1 count=`ls -l inst.tar | awk -F " " '{print $$5}'` conv=notrunc

all : realclean everything

final : all clean

clean :
	rm -f *.o

realclean :
	rm -f $(BIN) *.o

kernel.bin :
	cp ../kernel.bin ./

start.o : start.asm
	$(ASM) $(ASMFLAGS) -o $@ $<

echo.o: echo.c ../include/type.h ../include/stdio.h
	$(CC) $(CFLAGS) -o $@ $<

echo : echo.o start.o $(LIB)
	$(LD) $(LDFLAGS) -o $@ $?

pwd.o: pwd.c ../include/type.h ../include/stdio.h
	$(CC) $(CFLAGS) -o $@ $<

pwd : pwd.o start.o $(LIB)
	$(LD) $(LDFLAGS) -o $@ $?

attack.o: attack.c ../include/type.h ../include/stdio.h ../include/string.h
	$(CC) $(CFLAGS) -o $@ $<

attack: attack.o start.o $(LIB)
	$(LD) $(LDFLAGS) -o $@ $?

overflow.o: overflow.c ../include/type.h ../include/stdio.h ../include/string.h
	$(CC) $(CFLAGS) -o $@ $<

overflow: overflow.o start.o $(LIB)
	$(LD) $(LDFLAGS) -o $@ $?

attack_stack.o: attack_stack.c ../include/type.h ../include/stdio.h ../include/string.h
	$(CC) $(CFLAGS) -o $@ $<

attack_stack: attack_stack.o start.o $(LIB)
	$(LD) $(LDFLAGS) -o $@ $?