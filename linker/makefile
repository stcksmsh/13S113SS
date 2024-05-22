all: build test

include ../makefile.variable

# Path: emulator/makefile

LINKER_SRC=${wildcard src/*.cpp}
LINKER_OBJ=${LINKER_SRC:.cpp=.o}

build: ${LINKER_OBJ}
	@echo -n "${INDENTONE}Building linker...  "
	@${CXX} ${CXXFLAGS} -o ${LINKER} ${LINKER_OBJ}
	@echo "Done."

${LINKER_OBJ}: ${LINKER_SRC} | ${OBJ_DIR}
	@echo -n "${INDENTONE}Compiling $<...  "
	@${CXX} ${CXXFLAGS} -c $< -o $@
	@echo "Done."

${OBJ_DIR}:
	@mkdir -p $@

clean:
	@echo -n "${INDENTONE}Cleaning linker...  "
	@rm -f ${LINKER_OBJ} ${LINKER}
	@echo "Done."
