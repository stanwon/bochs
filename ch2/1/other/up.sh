#!/usr/bin/bash

OUT_PATH="../../../out"
COMMON_PATH="../../../common"

dd if=${OUT_PATH}/mbr.bin of=${COMMON_PATH}/hd60M.img bs=512 count=1 conv=notrunc
bochs -f bochsrc.disk
