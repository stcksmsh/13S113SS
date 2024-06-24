#include <iostream>
#include "emulator.hpp"
#include "logger.hpp"
#include "arguments.hpp"

int main(int argc, const char **argv)
{
	Arguments args({
		{"", "", "Emulates a proprietary basic school system", 1, false, true, -1},
		{"entry", "e", "The entry point of the program", 1, false, false, -1},
		{"log-level", "l", "Specifies the log level: 0 - no logging, 1 - errors only, 2 - warnings, 3 - info, 4 - debug", 1, false, false, -1},
	});

	if (args.parse(argc, argv) == EXIT_FAILURE)
		return EXIT_FAILURE;

	int log_level = 1;
	if (args.isPresent("log-level"))
		log_level = std::stoi(args.getArguments("log-level")[0][0]);
	
	Logger logger(log_level, true);

	Emulator emulator(&logger);

	std::string filename = args.getArguments()[0][0];

	emulator.loadFromDump(filename);

	uint32_t entry = 0xffffffff;
	if (args.isPresent("entry"))
		entry = std::stoul(args.getArguments("entry")[0][0], nullptr, 16);

	if (entry == 0xffffffff) emulator.run();
	else emulator.run(entry);

	emulator.printRegisters();
		
	return 0;
}