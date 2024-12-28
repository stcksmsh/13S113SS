include ../../makefile.variable

SRC = ${wildcard src/*.cpp}
OBJ = ${patsubst src/%.cpp,obj/%.o,${SRC}}

LIB = ../../lib/formatter.o


build: ${OBJ}
	@echo -n "${INDENT}${INDENT}Linking formatter...  "
	@${CXX} ${OBJ} -r -o ${LIB} 
	@echo "Done."

obj/%.o: src/%.cpp | ${OBJ_DIR}
	@echo -n "${INDENT}${INDENT}Building $@...  "
	@${CXX} -Iinc -c $< -o $@
	@echo "Done."

${OBJ_DIR}:
	@mkdir -p ${OBJ_DIR}

clean:
	@echo -n "${INDENT}${INDENT}Cleaning formatter...  "
	@rm -f ${LIB}
	@rm -rdf ${OBJ_DIR}/*
	@echo "Done."