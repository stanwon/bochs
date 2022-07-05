SRC_MBR = ../src/code/mbr.S
SRC_LOADER = ../src/code/loader.S
SRC_KERNEL = ../src/code/main.c
INC_FILE_PATH = ../include/
GCC = gcc
CC = nasm
LD = ld

all: clean

loader:	loader.bin
	mv loader.bin ../out/

mbr.bin:
	${CC} -I ${INC_FILE_PATH} -o $@ ${SRC_MBR}

loader.bin:
	${CC} -I ${INC_FILE_PATH} -o $@ ${SRC_LOADER}

main.o:
	${GCC} -c -o $@ ${SRC_KERNEL}

kernel.bin: main.o
	${LD} -Ttext 0xc0001500 -e main -o $@ $<

clean: mbr.bin loader.bin kernel.bin
	mv *.bin ../out/
	rm *.o
