#!/bin/bash
ASSEMBLER=..//bin/assembler
LINKER=..//bin/linker
EMULATOR=..//bin/emulator

${ASSEMBLER} -l 2 -o main.o c/main.s
${ASSEMBLER} -l 2 -o handler.o c/handler.s
${ASSEMBLER} -l 2 -o isr_terminal.o c/isr_terminal.s
${ASSEMBLER} -l 2 -o isr_timer.o c/isr_timer.s

rm program.hex
rm *.rpt

# valgrind --leak-check=yes --log-file=valgrind_linker.rpt ${LINKER} -x -l 2 \
#   -p code 0x40000000 \
#   -o program.hex \
#   main.o isr_terminal.o isr_timer.o handler.o

${LINKER} -x -l 2 \
  -p code 0x40000000 \
  -o program.hex \
  main.o isr_terminal.o isr_timer.o handler.o
# valgrind --leak-check=yes --log-file=valgrind_emulator.rpt ${EMULATOR} -l 2 program.hex
${EMULATOR} -l 2 program.hex


