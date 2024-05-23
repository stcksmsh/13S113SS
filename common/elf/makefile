include ../../makefile.variable

SRC = ${wildcard ${SRC_DIR}/*.cpp}
OBJ = ${patsubst ${SRC_DIR}/%.cpp,${OBJ_DIR}/%.o,${SRC}}

LIB = ../../lib/elf.o


build: ${OBJ}
	@echo -n "${INDENT}${INDENT}Linking elf...  "
	@${CXX} ${OBJ} -r -o ${LIB} 
	@echo "Done."

obj/%.o: src/%.cpp | ${OBJ_DIR}
	@echo -n "${INDENT}${INDENT}Building $@...  "
	@${CXX} -Iinc -c $< -o $@
	@echo "Done."

${OBJ_DIR}:
	@mkdir -p ${OBJ_DIR}

clean:
	@echo -n "${INDENT}${INDENT}Cleaning elf...  "
	@rm -f ${LIB}
	@rm -rdf ${OBJ_DIR}/*
	@echo "Done."