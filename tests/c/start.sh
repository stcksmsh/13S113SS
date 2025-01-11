#!/bin/bash
ASSEMBLER=..//bin/assembler
LINKER=..//bin/linker
EMULATOR=..//bin/emulator
LL=${LOG_LEVEL:-2}

${ASSEMBLER} -l ${LL} -o main.o c/main.s
${ASSEMBLER} -l ${LL} -o handler.o c/handler.s
${ASSEMBLER} -l ${LL} -o isr_terminal.o c/isr_terminal.s
${ASSEMBLER} -l ${LL} -o isr_timer.o c/isr_timer.s


${LINKER} -x -l ${LL} \
  -p code 0x40000000 \
  -o program.hex \
  main.o isr_terminal.o isr_timer.o handler.o
${EMULATOR} -l ${LL} program.hex


