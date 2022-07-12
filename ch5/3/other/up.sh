#!/usr/bin/bash

OUT_PATH="../../../out"
COMMON_PATH="../../../common"

dd if=${OUT_PATH}/mbr.bin of=${COMMON_PATH}/hd60M.img bs=512 count=1 conv=notrunc
dd if=${OUT_PATH}/loader.bin of=${COMMON_PATH}/hd60M.img bs=512 seek=2 count=4 conv=notrunc
dd if=${OUT_PATH}/kernel.bin of=${COMMON_PATH}/hd60M.img bs=512 seek=9 count=200 conv=notrunc
bochs -f bochsrc.disk
