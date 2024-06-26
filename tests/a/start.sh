#!/bin/bash
ASSEMBLER=..//bin/assembler
LINKER=..//bin/linker
EMULATOR=..//bin/emulator

${ASSEMBLER} -l 2 -o main.o a/main.s
${ASSEMBLER} -l 2 -o math.o a/math.s
${ASSEMBLER} -l 2 -o handler.o a/handler.s
${ASSEMBLER} -l 2 -o isr_timer.o a/isr_timer.s
${ASSEMBLER} -l 2 -o isr_terminal.o a/isr_terminal.s
${ASSEMBLER} -l 2 -o isr_software.o a/isr_software.s
${LINKER} -x -l 2 \
  -p my_code 0x40000000 -p math 0xF0000000 \
  -o program.hex \
  handler.o math.o main.o isr_terminal.o isr_timer.o isr_software.o
${EMULATOR} -l 2 program.hex