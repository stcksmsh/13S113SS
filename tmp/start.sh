#!/bin/bash
ASSEMBLER=../bin/assembler
LINKER=../bin/linker

if [[ -z "${LOG_LEVEL}" ]]; then
  LOG_LEVEL=2
fi



${ASSEMBLER} -l 2 -o main.o main.s
${ASSEMBLER} -l 2 -o handler.o handler.s

# this is to be able to read the elf, and see changes
echo "---------------------------------"
echo "*********************************"
echo "---------------------------------"
${LINKER} -l ${LOG_LEVEL} -p my_code_main 0x40000000 -x -o out.hex main.o handler.o

rm main.o handler.o

