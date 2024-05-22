all: a b c

include ../makefile.variable

SHELL := /bin/bash

a: FORCE
	@echo "${INDENTONE}Running a..."
	@source a/start.sh
	@echo "${INDENTONE}Done."

b: FORCE
	@echo "${INDENTONE}Running b..."
	@source b/start.sh
	@echo "${INDENTONE}Done."

c: FORCE
	@echo "${INDENTONE}Running c..."
	@source c/start.sh
	@echo "${INDENTONE}Done."

clean:
	@echo -n "${INDENTONE}Cleaning tests...   "
	@rm -f a/*.o a/program.hex b/*.o b/program.hex c/*.o c/program.hex
	@echo "${INDENTONE}Done."