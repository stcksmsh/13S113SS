#!/bin/bash

ASSEMBLER=../bin/assembler

echo "Running assembler tests"


${ASSEMBLER} -l 2 -o main.o tests/main.s
../bin/linker -l 2 -p code 0x40000000 --hex -o main.hex main.o
../bin/emulator -l 2 main.hex