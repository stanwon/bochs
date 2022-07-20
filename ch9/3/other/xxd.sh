#!/usr/bin/bash
xxd -u -a -g 1 -s $1 -l $2 ../../../out/tar/kernel.bin | less
