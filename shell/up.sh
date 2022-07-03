#!/usr/bin/bash
dd if=../out/mbr.bin of=../src/other/hd60M.img bs=512 count=1 conv=notrunc
dd if=../out/loader.bin of=../src/other/hd60M.img bs=512 count=1 seek=2 conv=notrunc
bochs -f ../src/other/bochsrc.disk
