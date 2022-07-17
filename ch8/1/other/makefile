INC_PATH = ../code/include
OBJ_PATH = ../../../out/obj
TARGET_PATH = ../../../out/tar

NCC = nasm
GCC = gcc
LD = ld

SRC_MBR = ../code/boot/mbr.S
SRC_LOADER = ../code/boot/loader.S

SRC_MAIN = ../code/kernel/main.c
SRC_INTERRUPT = ../code/kernel/interrupt.c
SRC_INIT = ../code/kernel/init.c
SRC_DEBUG = ../code/kernel/debug.c

SRC_PRINT = ../code/kernel/lib/print.S
SRC_KERNEL = ../code/kernel/kernel.S

SRC_TEST = ../code/test/test.S

all: kernel.bin mbr.bin loader.bin

print.o: ${SRC_PRINT}
	${NCC} -f elf -o ${OBJ_PATH}/$@ $<

kernel.o: ${SRC_KERNEL}
	${NCC} -f elf -o ${OBJ_PATH}/$@ $<

main.o: ${SRC_MAIN}
	${GCC} -I ${INC_PATH} -m32 -c -fno-stack-protector -fno-builtin -o ${OBJ_PATH}/$@ $<

interrupt.o: ${SRC_INTERRUPT}
	${GCC} -I ${INC_PATH} -m32 -c -fno-stack-protector -fno-builtin -o ${OBJ_PATH}/$@ $<

init.o: ${SRC_INIT}
	${GCC} -I ${INC_PATH} -m32 -c -fno-stack-protector -fno-builtin -o ${OBJ_PATH}/$@ $<

debug.o: ${SRC_DEBUG}
	${GCC} -I ${INC_PATH} -m32 -c -fno-stack-protector -fno-builtin -o ${OBJ_PATH}/$@ $<

loader.bin: ${SRC_LOADER}
	${NCC} -I ${INC_PATH} -o ${TARGET_PATH}/$@ $<

mbr.bin: ${SRC_MBR}
	${NCC} -I ${INC_PATH} -o ${TARGET_PATH}/$@ $<

kernel.bin: main.o print.o kernel.o interrupt.o init.o debug.o
	${LD} -m elf_i386 ${OBJ_PATH}/*.o -Ttext 0xc0001500 -e main -o ${TARGET_PATH}/$@

clean_obj:
	rm ${OBJ_PATH}/*

clean_tar:
	rm ${TARGET_PATH}/*

test:
	${NCC} -o $@ ${SRC_TEST}

clean: clean_obj clean_tar
