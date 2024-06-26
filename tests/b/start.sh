#!/bin/bash
ASSEMBLER=..//bin/assembler
LINKER=..//bin/linker
EMULATOR=..//bin/emulator

${ASSEMBLER} -l 2 -o main.o b/main.s
${ASSEMBLER} -l 2 -o handler.o b/handler.s
${ASSEMBLER} -l 2 -o isr_terminal.o b/isr_terminal.s
${ASSEMBLER} -l 2 -o isr_timer.o b/isr_timer.s
${LINKER} -x -l 2 \
  --place my_code 0x40000000 \
  -o program.hex \
  main.o isr_terminal.o isr_timer.o handler.o
${EMULATOR} -l 2 program.hex