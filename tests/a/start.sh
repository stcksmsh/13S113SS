#!/bin/bash
ASSEMBLER=..//bin/assembler
LINKER=..//bin/linker
EMULATOR=..//bin/emulator
LL=${LOG_LEVEL:-2}

${ASSEMBLER} -l ${LL} -o main.o a/main.s
${ASSEMBLER} -l ${LL} -o math.o a/math.s
${ASSEMBLER} -l ${LL} -o handler.o a/handler.s
${ASSEMBLER} -l ${LL} -o isr_timer.o a/isr_timer.s
${ASSEMBLER} -l ${LL} -o isr_terminal.o a/isr_terminal.s
${ASSEMBLER} -l ${LL} -o isr_software.o a/isr_software.s
${LINKER} -x -l ${LL} \
  -p my_code 0x40000000 -p math 0xF0000000 \
  -o program.hex \
  handler.o math.o main.o isr_terminal.o isr_timer.o isr_software.o
${EMULATOR} -l ${LL} program.hex
