BIN = bin

include makefile.variable

all: $(ASSEMBLER) $(LINKER) $(EMULATOR)

build: $(ASSEMBLER) $(LINKER) $(EMULATOR)

$(ASSEMBLER): FORCE | $(BIN)
	@echo "Building assembler..."
	@$(MAKE) --no-print-directory -C $(ASSEMBLER) build
	@cp $(ASSEMBLER)/$(ASSEMBLER) $(BIN)/$(ASSEMBLER)
	@echo "Done building assembler."

$(LINKER): FORCE | $(BIN)
	@echo "Building linker..."
	@$(MAKE) --no-print-directory -C $(LINKER) build
	@cp $(LINKER)/$(LINKER) $(BIN)/$(LINKER)
	@echo "Done building linker."

$(EMULATOR): FORCE | $(BIN)
	@echo "Building emulator..."
	@$(MAKE) --no-print-directory -C $(EMULATOR) build
	@cp $(EMULATOR)/$(EMULATOR) $(BIN)/$(EMULATOR)
	@echo "Done building emulator."

$(BIN):
	@mkdir -p $(BIN)

test: $(ASSEMBLER) $(LINKER) $(EMULATOR) FORCE
	@echo "Running $(TESTS)..."
	@$(MAKE) --no-print-directory -C $(TESTS)
	@echo "Done running $(TESTS)."

clean:
	@echo "Cleaning..."
	@$(MAKE) --no-print-directory -C $(ASSEMBLER) clean
	@$(MAKE) --no-print-directory -C $(LINKER) clean
	@$(MAKE) --no-print-directory -C $(EMULATOR) clean
	@$(MAKE) --no-print-directory -C $(TESTS) clean
	@rm -f $(BIN)/*
	@echo "Done cleaning."