SRC_MBR = ../src/code/mbr.S
SRC_LOADER = ../src/code/loader.S
INC_FILE_PATH = ../include/
CC = nasm

all: clean

loader:	loader.bin
	mv loader.bin ../out/

mbr.bin:
	${CC} -I ${INC_FILE_PATH} -o $@ ${SRC_MBR}

loader.bin:
	${CC} -I ${INC_FILE_PATH} -o $@ ${SRC_LOADER}

clean: mbr.bin loader.bin
	mv *.bin ../out/
