#!/bin/bash
ASSEMBLER=..//bin/assembler
LINKER=..//bin/linker
EMULATOR=..//bin/emulator

${ASSEMBLER} -o main.o a/main.s
${ASSEMBLER} -o math.o a/math.s
${ASSEMBLER} -o handler.o a/handler.s
${ASSEMBLER} -o isr_timer.o a/isr_timer.s
${ASSEMBLER} -o isr_terminal.o a/isr_terminal.s
${ASSEMBLER} -o isr_software.o a/isr_software.s
${LINKER} -x\
  -p my_code 0x40000000 -p math 0xF0000000 \
  -o program.hex \
  handler.o math.o main.o isr_terminal.o isr_timer.o isr_software.o
${EMULATOR} program.hex