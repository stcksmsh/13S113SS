all: build test

include ../makefile.variable

# Path: emulator/makefile

EMULATOR_SRC=${wildcard src/*.cpp}
EMULATOR_OBJ=${patsubst src/%.cpp,obj/%.o,${EMULATOR_SRC}}


EMULATOR_EXE=emulator

LIB_HEADERS= ../lib/inc
LIBS= logger arguments
LIB_OBJS=$(foreach lib,$(subst $(space),$(newline),$(LIBS)),../lib/$(lib).o)

CXXFLAGS+= -I${LIB_HEADERS}

build: clean ${EMULATOR_OBJ}
	@echo -n "${INDENT}Linking emulator...  "
	@${CXX} ${CXXFLAGS} ${LIB_OBJS} ${EMULATOR_OBJ} -o ${EMULATOR_EXE}
	@echo "Done."

${OBJ_DIR}/%.o: ${SRC_DIR}/%.cpp
	@echo -n "${INDENT}Building $@...  "
	@${CXX} ${CXXFLAGS} -c $< -o $@
	@echo "Done."

test: FORCE
	@echo "${INDENT}Running emulator tests..."
	@source tests/start.sh
	@echo "${INDENT}Done."

clean:
	@echo -n "${INDENT}Cleaning emulator...  "
	@rm -f ${SRC_DIR}/parser.cc ${SRC_DIR}/scanner.cc
	@rm -f ${EMULATOR_EXE}
	@rm -rdf ${OBJ_DIR}/*
	@echo "Done."