all: a b c

include ../makefile.variable

SHELL := /bin/bash

a: FORCE
	@echo "Running a..."
	@source a/start.sh
	@echo "Done."

b: FORCE
	@echo "Running b..."
	@source b/start.sh
	@echo "Done."

c: FORCE
	@echo "Running c..."
	@source c/start.sh
	@echo "Done."

clean:
	@echo -n "Cleaning...   "
	@rm -f a/*.o a/program.hex b/*.o b/program.hex c/*.o c/program.hex
	@echo "Done."