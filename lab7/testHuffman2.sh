#!/bin/bash
clear
gcc -m32 -std=c99 -g -o huffman huffman.c
cp huffman case2
cd case2
gdb --args ./huffman -i t.txt -o out.bin  --code t.code
rm huffman
cd ..
rm huffman
