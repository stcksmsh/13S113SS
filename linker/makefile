all: build test

include ../makefile.variable

# Path: linker/makefile

LINKER_SRC=${wildcard src/*.cpp}
LINKER_OBJ=${patsubst src/%.cpp,obj/%.o,${LINKER_SRC}}


LINKER_EXE=linker


LIB_HEADERS= ../lib/inc
LIBS= elf logger arguments
LIB_OBJS=$(foreach lib,$(subst $(space),$(newline),$(LIBS)),../lib/$(lib).o)

CXXFLAGS+= -I${LIB_HEADERS}

build: clean ${LINKER_OBJ}
	@echo -n "${INDENT}Linking linker...  "
	@${CXX} ${CXXFLAGS} ${LIB_OBJS} ${LINKER_OBJ} -o ${LINKER_EXE}
	@echo "Done."

${OBJ_DIR}/%.o: ${SRC_DIR}/%.cpp ${OBJ_DIR}
	@echo -n "${INDENT}Building $@...  "
	@${CXX} ${CXXFLAGS} -c $< -o $@
	@echo "Done."

${OBJ_DIR}:
	@mkdir -p ${OBJ_DIR}

test: FORCE
	@echo "${INDENT}Running linker tests..."
	@source tests/start.sh
	@echo "${INDENT}Done."

clean:
	@echo -n "${INDENT}Cleaning linker...  "
	@rm -f ${SRC_DIR}/parser.cc ${SRC_DIR}/scanner.cc
	@rm -f ${LINKER_EXE}
	@rm -rdf ${OBJ_DIR}/*
	@echo "Done."