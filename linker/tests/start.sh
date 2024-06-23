#!/bin/bash
ASSEMBLER=../../bin/assembler
LINKER=../../bin/linker

if [[ -z "${LOG_LEVEL}" ]]; then
  LOG_LEVEL=2
fi


#Print contents of directory
cd tests

${ASSEMBLER} -l 2 -o test1.o test1.s
${ASSEMBLER} -l 2 -o test2.o test2.s

# this is to be able to read the elf, and see changes
echo "---------------------------------"
echo "*********************************"
echo "---------------------------------"
${LINKER} -l ${LOG_LEVEL} -p my_code_main 0x400000 -x -o out.hex test1.o test2.o

rm test1.o test2.o