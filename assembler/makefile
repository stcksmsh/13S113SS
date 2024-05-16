include ../makefile.variable

# Comment out the following line to disable debug mode
DEBUG=1

# Path: assembler/makefile

ASSEMBLER_SRC=$(wildcard src/*.cpp)
ASSEMBLER_OBJ=$(patsubst src/%.cpp,obj/%.o,$(ASSEMBLER_SRC))

ASSEMBLER_EXE=assembler

PARSER_FILE=src/parser.y
SCANNER_FILE=src/scanner.l

CXXFLAGS= -Iinc

all: clean parser scanner ${ASSEMBLER_OBJ}
	${CXX} ${CXXFLAGS} ${ASSEMBLER_OBJ} ${OBJ_DIR}/parser.o ${OBJ_DIR}/scanner.o -o ${ASSEMBLER_EXE}

${OBJ_DIR}/%.o: ${SRC_DIR}/%.cpp
	${CXX} ${CXXFLAGS} -c $< -o $@

parser: $(PARSER_FILE)
	bison -t -d -o ${SRC_DIR}/parser.cc ${PARSER_FILE}
	mv ${SRC_DIR}/parser.hh ${INC_DIR}
	mv ${SRC_DIR}/location.hh ${INC_DIR}
	${CXX}  ${CXXFLAGS} -c ${SRC_DIR}/parser.cc -o ${OBJ_DIR}/parser.o

scanner: $(SCANNER_FILE)
	flex -o ${SRC_DIR}/scanner.cc ${SCANNER_FILE}
	${CXX}  ${CXXFLAGS} -c ${SRC_DIR}/scanner.cc -o ${OBJ_DIR}/scanner.o


clean:
	rm -f ${SRC_DIR}/parser.cc ${SRC_DIR}/scanner.cc
	rm -f ${ASSEMBLER_EXE}
	rm -rdf ${OBJ_DIR}/*