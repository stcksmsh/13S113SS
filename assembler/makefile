all: build test

include ../makefile.variable

# Path: assembler/makefile

ASSEMBLER_SRC=${wildcard src/*.cpp}
ASSEMBLER_OBJ=${patsubst src/%.cpp,obj/%.o,${ASSEMBLER_SRC}}


ASSEMBLER_EXE=assembler

PARSER_FILE=src/parser.y
SCANNER_FILE=src/scanner.l


LIB_HEADERS= ../lib/inc
LIBS= elf logger arguments
LIB_OBJS=$(foreach lib,$(subst $(space),$(newline),$(LIBS)),../lib/$(lib).o)

CXXFLAGS+= -I${LIB_HEADERS}

build: clean parser scanner ${ASSEMBLER_OBJ}
	@echo -n "${INDENT}Linking assembler...  "
	@${CXX} ${CXXFLAGS} ${LIB_OBJS} ${ASSEMBLER_OBJ} ${OBJ_DIR}/parser.o ${OBJ_DIR}/scanner.o -o ${ASSEMBLER_EXE}
	@echo "Done."

${OBJ_DIR}/%.o: ${SRC_DIR}/%.cpp
	@echo -n "${INDENT}Building $@...  "
	@${CXX} ${CXXFLAGS} -c $< -o $@
	@echo "Done."

parser: ${PARSER_FILE}
	@echo -n "${INDENT}Building parser...  "
	@bison -t -d -o ${SRC_DIR}/parser.cc ${PARSER_FILE}
	@mv ${SRC_DIR}/parser.hh ${INC_DIR}
	@mv ${SRC_DIR}/location.hh ${INC_DIR}
	@${CXX}  ${CXXFLAGS} -c ${SRC_DIR}/parser.cc -o ${OBJ_DIR}/parser.o
	@echo "Done."

scanner: ${SCANNER_FILE}
	@echo -n "${INDENT}Building scanner...  "
	@flex -o ${SRC_DIR}/scanner.cc ${SCANNER_FILE}
	@${CXX}  ${CXXFLAGS} -c ${SRC_DIR}/scanner.cc -o ${OBJ_DIR}/scanner.o
	@echo "Done."


test: FORCE
	@echo "${INDENT}Running assembler tests..."
	@source tests/start.sh
	@echo "${INDENT}Done."

clean:
	@echo -n "${INDENT}Cleaning assembler...  "
	@rm -f ${SRC_DIR}/parser.cc ${SRC_DIR}/scanner.cc
	@rm -f ${ASSEMBLER_EXE}
	@rm -rdf ${OBJ_DIR}/*
	@echo "Done."