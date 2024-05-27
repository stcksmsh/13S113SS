include ../makefile.variable

# Path: linker/makefile

LINKER_SRC=${wildcard src/*.cpp}
LINKER_OBJ=${patsubst src/%.cpp,obj/%.o,${LINKER_SRC}}

LIBRARIES= ../lib/libelf.so
LIB_HEADERS= ../lib/inc

LINKER_EXE=linker

CXXFLAGS= -I../lib/inc -Iinc -std=c++20

LIBS= elf logger arguments
LIB_OBJS=$(foreach lib,$(subst $(space),$(newline),$(LIBS)),../lib/$(lib).o)


build: clean ${LINKER_OBJ}
	@echo -n "${INDENT}Linking linker...  \n"
	@${CXX} ${CXXFLAGS} ${LIB_OBJS} ${LINKER_OBJ} -o ${LINKER_EXE}
	@echo "Done."

${OBJ_DIR}/%.o: ${SRC_DIR}/%.cpp
	@echo -n "${INDENT}Building $@...  "
	@${CXX} ${CXXFLAGS} -c $< -o $@
	@echo "Done."


clean:
	@echo -n "${INDENT}Cleaning linker...  "
	@rm -f ${SRC_DIR}/parser.cc ${SRC_DIR}/scanner.cc
	@rm -f ${LINKER_EXE}
	@rm -rdf ${OBJ_DIR}/*
	@echo "Done."