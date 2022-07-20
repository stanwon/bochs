#!/bin/bash
gcc -m32 -c -o main.o ../code/main.c
#strip --remove-section=.note.gnu.property main.o
ld -m elf_i386 main.o -Ttext 0xc0001500 -e main -o main
