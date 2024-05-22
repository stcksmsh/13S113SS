all: build test

include ../makefile.variable

# Path: emulator/makefile

EMULATOR_SRC=${wildcard src/*.cpp}
EMULATOR_OBJ=${EMULATOR_SRC:.cpp=.o}

build: ${EMULATOR_OBJ}
	@echo -n "${INDENTONE}Building emulator...  "
	@${CXX} ${CXXFLAGS} -o ${EMULATOR} ${EMULATOR_OBJ}
	@echo "Done."

${EMULATOR_OBJ}: ${EMULATOR_SRC} | ${OBJ_DIR}
	@echo -n "${INDENTONE}Compiling $<...  "
	@${CXX} ${CXXFLAGS} -c $< -o $@
	@echo "Done."

${OBJ_DIR}:
	@mkdir -p $@

clean:
	@echo -n "${INDENTONE}Cleaning emulator...  "
	@rm -f ${EMULATOR_OBJ} ${EMULATOR}
	@echo "Done."
