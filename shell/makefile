ASM_INC_FILE_PATH = ../src/code/include/asm
C_INC_FILE_PATH = ../src/code/include/c

SRC_ASM = ../src/code/kernel/asm
SRC_C = ../src/code/kernel/c

SRC_MBR = ${SRC_ASM}/mbr/mbr.S
SRC_LOADER = ${SRC_ASM}/loader/loader.S
SRC_PRINT = ${SRC_ASM}/print.S
SRC_KERNEL = ${SRC_C}/main.c

GCC = gcc
NCC = nasm
LD = ld

all: clean

loader:	loader.bin
	mv loader.bin ../out/

mbr.bin:
	${NCC} -I ${ASM_INC_FILE_PATH} -o $@ ${SRC_MBR}

loader.bin:
	${NCC} -I ${ASM_INC_FILE_PATH} -o $@ ${SRC_LOADER}

main.o:
	${GCC} -c -o $@ ${SRC_KERNEL}

kernel.bin: main.o
	${LD} -Ttext 0xc0001500 -e main -o $@ $<

clean: mbr.bin loader.bin kernel.bin
	mv *.bin ../out/
	rm *.o
