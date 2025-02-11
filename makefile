all: build test

# The include directive is used after the 'all' rule to make sure that the 'all' rule is the first rule in the makefile. 
include makefile.variable

build: ${ASSEMBLER} ${LINKER} ${EMULATOR}


${ASSEMBLER}: FORCE ${COMMON} | ${BIN_DIR}
	@echo "Building assembler..."
	@${MAKE} --no-print-directory -C ${ASSEMBLER} build
	@mv ${ASSEMBLER}/${ASSEMBLER} ${BIN_DIR}/${ASSEMBLER}
	@export INDENT=""
	@echo "Done building assembler."

${LINKER}: FORCE ${COMMON} | ${BIN_DIR}
	@echo "Building linker..."
	@${MAKE} --no-print-directory -C ${LINKER} build
	@mv ${LINKER}/${LINKER} ${BIN_DIR}/${LINKER}
	@echo "Done building linker."

${EMULATOR}: FORCE ${COMMON} | ${BIN_DIR}
	@echo "Building emulator..."
	@${MAKE} --no-print-directory -C ${EMULATOR} build
	@mv ${EMULATOR}/${EMULATOR} ${BIN_DIR}/${EMULATOR}
	@echo "Done building emulator."

${COMMON}: FORCE ${BIN_DIR}
	@echo "Building common..."
	@${MAKE} --no-print-directory -C ${COMMON} build
	@echo "Done building common."

${BIN_DIR}:
	@mkdir -p ${BIN_DIR}

test: mytests publictests

mytests: build
	@echo "Running assembler tests..."
	@${MAKE} --no-print-directory -C ${ASSEMBLER} test
	@echo "Done running ${ASSEMBLER} tests."
	@echo "Running linker tests..."
	@${MAKE} --no-print-directory -C ${LINKER} test
	@echo "Done running ${LINKER} tests."
	@echo "Running emulator tests..."
	@${MAKE} --no-print-directory -C ${EMULATOR} test
	@echo "Done running ${EMULATOR} tests."

publictests: build
	@echo "Running integration tests..."
	@${MAKE} --no-print-directory -C ${TESTS}
	@echo "Done running integration tests."

clean:
	@echo "Cleaning..."
	@${MAKE} --no-print-directory -C ${ASSEMBLER} clean
	@${MAKE} --no-print-directory -C ${LINKER} clean
	@${MAKE} --no-print-directory -C ${EMULATOR} clean
	@${MAKE} --no-print-directory -C ${TESTS} clean
	@${MAKE} --no-print-directory -C ${COMMON} clean
	@rm -f ${BIN_DIR}/*
	@echo "Done cleaning."

commit: clean
	@source updateLineCount.sh
	@git add .
	@git commit ./
	@git push