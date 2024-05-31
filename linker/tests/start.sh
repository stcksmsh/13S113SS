#!/bin/bash
ASSEMBLER=../../bin/assembler
LINKER=../../bin/linker

if [[ -z "${LOG_LEVEL}" ]]; then
  LOG_LEVEL=2
fi


#Print contents of directory
cd tests

${ASSEMBLER} -l ${LOG_LEVEL} -o test1.o test1.s
${ASSEMBLER} -l ${LOG_LEVEL} -o test2.o test2.s

${LINKER} -l ${LOG_LEVEL} -r -o out.o test1.o test2.o

readelf -a out.o
