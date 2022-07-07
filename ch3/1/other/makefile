SRC_MBR = ../code/mbr.S
SRC_TEST = ../code/test.S

NCC = nasm

all: clean

mbr.bin:
	${NCC} -o $@ ${SRC_MBR}

clean: mbr.bin
	mv *.bin ../../../out/

test:
	${NCC} -o $@ ${SRC_TEST}
