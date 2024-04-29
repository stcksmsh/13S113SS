include ../makefile.variable

# Path: assembler/makefile

ASSEMBLER_SRC=$(wildcard src/*.cpp)
ASSEMBLER_OBJ=$(patsubst src/%.cpp,obj/%.o,$(ASSEMBLER_SRC))

FLEX=flex
BISON=bison

MISC_DIR=misc

LEXER=${MISC_DIR}/lexer.l
LEXER_SRC=$(patsubst ${MISC_DIR}/%.l,${SRC_DIR}/%.c,$(LEXER))
LEXER_OBJ=$(patsubst ${MISC_DIR}/%.l,${OBJ_DIR}/%.o,$(LEXER))

PARSER=${MISC_DIR}/parser.y
PARSER_HEADER=$(patsubst ${MISC_DIR}/%.y,${SRC_DIR}/%.h,$(PARSER))
PARSER_SRC=$(patsubst ${MISC_DIR}/%.y,${SRC_DIR}/%.c,$(PARSER))
PARSER_OBJ=$(patsubst ${MISC_DIR}/%.y,${OBJ_DIR}/%.o,$(PARSER))

build: $(ASSEMBLER_OBJ) $(LEXER_OBJ) $(PARSER_OBJ)
	@echo "Building assembler...  "
	@$(CXX) $(CXXFLAGS) -o $(ASSEMBLER) $(ASSEMBLER_OBJ) $(LEXER_OBJ) $(PARSER_OBJ)
	@echo "Assembler built successfully."

$(ASSEMBLER_OBJ): $(ASSEMBLER_SRC) | $(OBJ_DIR)
	@echo -n "Compiling $<...  "
	@$(CXX) $(CXXFLAGS) -c $< -o $@
	@echo "Done."

$(LEXER_OBJ): ${LEXER_SRC} ${PARSER_SRC}
	@echo -n "Compiling $<...   "
	@$(GCC) -c $< -o $@
	@echo "Done."

${LEXER_SRC}: ${LEXER}
	@echo -n "Generating lexer...  "
	@$(FLEX) -o $@ $<
	@echo "Done."


${PARSER_SRC}: ${PARSER}
	@echo -n "Compiling $<...   "
	@$(BISON) -d -o $@ $<
	@echo "Done."

${PARSER_OBJ}: ${PARSER_SRC}
	@echo -n "Compiling parser...  "
	@$(GCC) -c $< -o $@
	@echo "Done."


$(OBJ_DIR):
	@mkdir -p $@

clean:
	@echo -n "Cleaning assembler...  "
	@rm -rf $(ASSEMBLER_OBJ) $(ASSEMBLER)
	@rm -rf $(LEXER_OBJ) $(LEXER_SRC)
	@rm -rf $(PARSER_OBJ) $(PARSER_SRC)
	@echo "Done."


.PHONY: build clean
default: build
