all: build test

include ../makefile.variable

# Path: emulator/makefile

EMULATOR_SRC=${wildcard src/*.cpp}
EMULATOR_OBJ=${EMULATOR_SRC:.cpp=.o}

build: ${EMULATOR_OBJ}
	@echo -n "${INDENT}Building emulator...  "
	@${CXX} ${CXXFLAGS} -o ${EMULATOR} ${EMULATOR_OBJ}
	@echo "Done."

${EMULATOR_OBJ}: ${EMULATOR_SRC} | ${OBJ_DIR}
	@echo -n "${INDENT}Compiling $<...  "
	@${CXX} ${CXXFLAGS} -c $< -o $@
	@echo "Done."

${OBJ_DIR}:
	@mkdir -p $@

test: FORCE
	@echo "${INDENT}Running emulator tests..."
	@source tests/start.sh
	@echo "${INDENT}Done."

clean:
	@echo -n "${INDENT}Cleaning emulator...  "
	@rm -f ${EMULATOR_OBJ} ${EMULATOR}
	@echo "Done."
