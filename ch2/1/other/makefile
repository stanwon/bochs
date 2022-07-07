SRC_MBR = ../code/mbr.S

NCC = nasm

all: clean

mbr.bin:
	${NCC} -o $@ ${SRC_MBR}

clean: mbr.bin
	mv *.bin ../../../out/

