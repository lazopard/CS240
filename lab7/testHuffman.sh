#!/bin/bash
clear
gcc -m32 -std=c99 -g -o huffman huffman.c
cp huffman case1
cd case1
gdb --args ./huffman -i dot.txt -o dot.bin  --code dot.code
rm huffman
cd ..
rm huffman
