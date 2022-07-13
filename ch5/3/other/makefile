INC_PATH = ../code/include
SRC_MBR = ../code/mbr.S
SRC_LOADER = ../code/loader.S
SRC_TEST = ../code/test.S
SRC_KERNEL = ../code/main.c

NCC = nasm
GCC = gcc
LD = ld

all: clean

loader.bin:
	${NCC} -I ${INC_PATH} -o $@ ${SRC_LOADER}

mbr.bin:
	${NCC} -I ${INC_PATH} -o $@ ${SRC_MBR}

main.o:
	${GCC} -m32 -c -o $@ ${SRC_KERNEL}

kernel.bin: main.o
	${LD} -m elf_i386 $< -Ttext 0xc0001500 -e main -o $@


clean: mbr.bin loader.bin kernel.bin
	mv *.bin ../../../out/
	#rm *.o

test:
	${NCC} -o $@ ${SRC_TEST}
