#!/bin/bash
gcc *.c -g -I ../
if [ $? -ne 0 ]; then exit 1; fi
./a.out 1
./a.out 2
./a.out 3
./a.out 4
./a.out 5
