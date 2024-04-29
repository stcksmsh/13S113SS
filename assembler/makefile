include ../makefile.variable

# Path: assembler/makefile

ASSEMBLER_SRC=$(wildcard src/*.cpp)
ASSEMBLER_OBJ=$(patsubst src/%.cpp,obj/%.o,$(ASSEMBLER_SRC))

FLEX=flex
BISON=bison

LEXER_OBJ=misc/flex/lexer.o

build: $(ASSEMBLER_OBJ) $(LEXER_OBJ)
	@echo -n "Building assembler...  "
	@$(CXX) $(CXXFLAGS) -o $(ASSEMBLER) $(ASSEMBLER_OBJ) $(LEXER_OBJ)
	@echo "Done."

$(ASSEMBLER_OBJ): $(ASSEMBLER_SRC) | $(OBJ_DIR)
	@echo -n "Compiling $<...  "
	@$(CXX) $(CXXFLAGS) -c $< -o $@
	@echo "Done."

$(LEXER_OBJ): misc/flex/lexer.l
	@echo -n "Generating lexer...  "
	$(FLEX) -o $(LEXER_OBJ:.o=.c) $<
	$(GCC) -c $(LEXER_OBJ:.o=.c) -o $(LEXER_OBJ)
	@echo "Done."

$(OBJ_DIR):
	@mkdir -p $@

clean:
	@echo -n "Cleaning assembler...  "
	@rm -f $(ASSEMBLER_OBJ) $(ASSEMBLER)
	@echo "Done."

