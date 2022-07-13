INC_PATH = ../code/include
OBJ_PATH = ../../../out/obj
TARGET_PATH = ../../../out/tar

NCC = nasm
GCC = gcc
LD = ld

SRC_MBR = ../code/boot/mbr.S
SRC_LOADER = ../code/boot/loader.S

SRC_KERNEL = ../code/kernel/main.c
SRC_PRINT = ../code/kernel/lib/print.S

SRC_TEST = ../code/test/test.S

all: kernel.bin mbr.bin loader.bin

print.o: ${SRC_PRINT}
	${NCC} -f elf -o ${OBJ_PATH}/$@ $<

main.o: ${SRC_KERNEL}
	${GCC} -I ${INC_PATH} -m32 -c -o ${OBJ_PATH}/$@ $<

loader.bin: ${SRC_LOADER}
	${NCC} -I ${INC_PATH} -o ${TARGET_PATH}/$@ $<

mbr.bin: ${SRC_MBR}
	${NCC} -I ${INC_PATH} -o ${TARGET_PATH}/$@ $<

kernel.bin: main.o print.o
	${LD} -m elf_i386 ${OBJ_PATH}/*.o -Ttext 0xc0001500 -e main -o ${TARGET_PATH}/$@


clean_obj:
	rm ${OBJ_PATH}/*

clean_tar:
	rm ${TAR_PATH}/*

test:
	${NCC} -o $@ ${SRC_TEST}
