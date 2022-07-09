INC_PATH = ../code/include
SRC_MBR = ../code/mbr.S
SRC_LOADER = ../code/loader.S
SRC_TEST = ../code/test.S

NCC = nasm

all: clean

loader.bin:
	${NCC} -I ${INC_PATH} -o $@ ${SRC_LOADER}

mbr.bin:
	${NCC} -I ${INC_PATH} -o $@ ${SRC_MBR}

clean: mbr.bin loader.bin
	mv *.bin ../../../out/

test:
	${NCC} -o $@ ${SRC_TEST}
