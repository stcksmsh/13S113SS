include ../../makefile.variable

SRC = ${wildcard src/*.cpp}
OBJ = ${patsubst src/%.cpp,obj/%.o,${SRC}}

LIB = ../../lib/logger.o


build: ${OBJ}
	@echo -n "${INDENTTWO}Linking logger...  "
	@${CXX} ${OBJ} -r -o ${LIB} 
	@echo "Done."

obj/%.o: src/%.cpp | ${OBJ_DIR}
	@echo -n "${INDENTTWO}Building $@...  "
	@${CXX} -Iinc -c $< -o $@
	@echo "Done."

${OBJ_DIR}:
	@mkdir -p ${OBJ_DIR}

clean:
	@echo -n "${INDENTTWO}Cleaning logger...  "
	@rm -f ${LIB}
	@rm -rdf ${OBJ_DIR}/*
	@echo "Done."