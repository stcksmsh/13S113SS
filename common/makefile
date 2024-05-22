include ../makefile.variable

DIRS = ${wildcard */}

build:
	@echo "${INDENTONE}Building common..."
	@for dir in ${DIRS}; do \
		${MAKE} --no-print-directory -C $$dir build; \
		echo "${INDENTONE}Done."; \
	done
	@echo -n "${INDENTONE}Copying headers...   "
	@for dir in ${DIRS}; do \
		cp -r $$dir/inc/* ../lib/inc; \
	done
	@echo "${INDENTONE}Done."

clean:
	@echo "${INDENTONE}Cleaning common..."
	@for dir in ${DIRS}; do \
		${MAKE} --no-print-directory -C $$dir clean; \
		echo  "${INDENTONE}Done."; \
	done