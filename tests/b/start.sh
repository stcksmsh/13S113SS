#!/bin/bash
ASSEMBLER=..//bin/assembler
LINKER=..//bin/linker
EMULATOR=..//bin/emulator

${ASSEMBLER} -o main.o b/main.s
${ASSEMBLER} -o handler.o b/handler.s
${ASSEMBLER} -o isr_terminal.o b/isr_terminal.s
${ASSEMBLER} -o isr_timer.o b/isr_timer.s
${LINKER} -x \
  --place my_code 0x40000000 \
  -o program.hex \
  main.o isr_terminal.o isr_timer.o handler.o
${EMULATOR} -l 4 program.hex