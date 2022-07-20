INC_PATH 			= ../code/include
OBJ_PATH 			= ../../../out/obj
TARGET_PATH 	= ../../../out/tar

NCC = nasm
GCC = gcc
LD = ld

CFLAGS = -I ${INC_PATH} -m32 -c -fno-builtin -fno-stack-protector
NFLAGS = -f elf

SRC_MAIN 			= ../code/kernel/main.c
SRC_INTERRUPT = ../code/kernel/interrupt.c
SRC_INIT 			= ../code/kernel/init.c
SRC_DEBUG 		= ../code/kernel/debug.c
SRC_BITMAP 		= ../code/kernel/bitmap.c
SRC_MEMORY 		= ../code/kernel/memory.c
SRC_THREAD 		= ../code/kernel/thread.c
SRC_LIST 			= ../code/kernel/list.c
SRC_TIMER 			= ../code/kernel/timer.c
SRC_STRING 		= ../code/kernel/lib/string.c

SRC_PRINT = ../code/kernel/lib/print.S
SRC_KERNEL = ../code/kernel/kernel.S
SRC_SWITCH = ../code/kernel/switch.S

all: kernel.bin

switch.o: ${SRC_SWITCH}
	${NCC} ${NFLAGS} -o ${OBJ_PATH}/$@ $<

print.o: ${SRC_PRINT}
	${NCC} ${NFLAGS} -o ${OBJ_PATH}/$@ $<

kernel.o: ${SRC_KERNEL}
	${NCC} ${NFLAGS} -o ${OBJ_PATH}/$@ $<

main.o: ${SRC_MAIN}
	${GCC} ${CFLAGS} -o ${OBJ_PATH}/$@ $<

interrupt.o: ${SRC_INTERRUPT}
	${GCC} ${CFLAGS} -o ${OBJ_PATH}/$@ $<

init.o: ${SRC_INIT}
	${GCC} ${CFLAGS} -o ${OBJ_PATH}/$@ $<

debug.o: ${SRC_DEBUG}
	${GCC} ${CFLAGS} -o ${OBJ_PATH}/$@ $<

bitmap.o: ${SRC_BITMAP}
	${GCC} ${CFLAGS} -o ${OBJ_PATH}/$@ $<

memory.o: ${SRC_MEMORY}
	${GCC} ${CFLAGS} -o ${OBJ_PATH}/$@ $<

string.o: ${SRC_STRING}
	${GCC} ${CFLAGS} -o ${OBJ_PATH}/$@ $<

thread.o: ${SRC_THREAD}
	${GCC} ${CFLAGS} -o ${OBJ_PATH}/$@ $<

list.o: ${SRC_LIST}
	${GCC} ${CFLAGS} -o ${OBJ_PATH}/$@ $<

timer.o: ${SRC_TIMER}
	${GCC} ${CFLAGS} -o ${OBJ_PATH}/$@ $<

objs: main.o \
			interrupt.o\
			debug.o \
			memory.o \
			bitmap.o \
			init.o \
			thread.o \
			list.o \
			timer.o \
			string.o \
			switch.o \
			print.o \
			kernel.o

kernel.bin: objs
	${LD} -m elf_i386 -Ttext 0xc0001500 -e main -Map ${OBJ_PATH}/kernel.map -o ${TARGET_PATH}/$@ \
		${OBJ_PATH}/main.o \
		${OBJ_PATH}/init.o \
		${OBJ_PATH}/interrupt.o \
		${OBJ_PATH}/memory.o \
		${OBJ_PATH}/bitmap.o \
		${OBJ_PATH}/kernel.o \
		${OBJ_PATH}/thread.o \
		${OBJ_PATH}/list.o \
		${OBJ_PATH}/timer.o \
		${OBJ_PATH}/string.o \
		${OBJ_PATH}/switch.o \
		${OBJ_PATH}/debug.o \
		${OBJ_PATH}/print.o

clean_obj:
	rm -f ${OBJ_PATH}/*

clean_tar:
	rm -f ${TARGET_PATH}/*

clean: clean_obj clean_tar
