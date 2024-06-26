include ../makefile.variable

DIRS = ${wildcard */}

build: ../lib/inc
	@echo -n "${INDENT}Copying headers...   "
	@for dir in ${DIRS}; do \
		cp -r $$dir/inc/* ../lib/inc; \
	done
	@echo "${INDENT}Done."
	@echo "${INDENT}Building common..."
	@for dir in ${DIRS}; do \
		${MAKE} --no-print-directory -C $$dir build; \
		echo "${INDENT}Done."; \
	done

../lib/inc:
	@echo "${INDENT}Creating include directory..."
	@mkdir -p ../lib/inc
	@echo "${INDENT}Done."

clean:
	@echo "${INDENT}Cleaning common..."
	@for dir in ${DIRS}; do \
		${MAKE} --no-print-directory -C $$dir clean; \
		echo  "${INDENT}Done."; \
	done