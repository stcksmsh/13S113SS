all: a b c

include ../makefile.variable

SHELL := /bin/bash

a: FORCE
	@echo "${INDENT}Running a..."
	@source a/start.sh
	@echo "${INDENT}Done."

b: FORCE
	@echo "${INDENT}Running b..."
	@source b/start.sh
	@echo "${INDENT}Done."

c: FORCE
	@echo "${INDENT}Running c..."
	@source c/start.sh
	@echo "${INDENT}Done."

clean:
	@echo -n "${INDENT}Cleaning tests...   "
	@rm -f a/*.o a/program.hex b/*.o b/program.hex c/*.o c/program.hex
	@echo "${INDENT}Done."