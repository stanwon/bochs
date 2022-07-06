SRC_MBR = ../src/code/kernel/mbr/mbr.S
SRC_LOADER = ../src/code/kernel/loader/loader.S
SRC_KERNEL = ../src/code/kernel/main.c
ASM_INC_FILE_PATH = ../src/code/include/asm
GCC = gcc
CC = nasm
LD = ld
C_INC_FILE_PATH = ../src/code/include/c

all: clean

loader:	loader.bin
	mv loader.bin ../out/

mbr.bin:
	${CC} -I ${ASM_INC_FILE_PATH} -o $@ ${SRC_MBR}

loader.bin:
	${CC} -I ${ASM_INC_FILE_PATH} -o $@ ${SRC_LOADER}

main.o:
	${GCC} -c -o $@ ${SRC_KERNEL}

kernel.bin: main.o
	${LD} -Ttext 0xc0001500 -e main -o $@ $<

clean: mbr.bin loader.bin kernel.bin
	mv *.bin ../out/
	rm *.o
