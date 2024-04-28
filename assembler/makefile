include ../makefile.variable

# Path: assembler/makefile

ASSEMBLER_SRC=$(wildcard src/*.cpp)
ASSEMBLER_OBJ=$(patsubst src/%.cpp,obj/%.o,$(ASSEMBLER_SRC))

build: $(ASSEMBLER_OBJ)
	@echo -n "Building assembler...  "
	@$(CXX) $(CXXFLAGS) -o $(ASSEMBLER) $(ASSEMBLER_OBJ)
	@echo "Done."

$(ASSEMBLER_OBJ): $(ASSEMBLER_SRC) | $(OBJ_DIR)
	@echo -n "Compiling $<...  "
	@$(CXX) $(CXXFLAGS) -c $< -o $@
	@echo "Done."

$(OBJ_DIR):
	@mkdir -p $@

clean:
	@echo -n "Cleaning assembler...  "
	@rm -f $(ASSEMBLER_OBJ) $(ASSEMBLER)
	@echo "Done."
