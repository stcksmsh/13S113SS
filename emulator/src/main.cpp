#include <iostream>
#include "emulator.hpp"
#include "logger.hpp"
#include "arguments.hpp"
#include <fstream>

int main(int argc, const char **argv)
{
	Arguments args({
		{"", "", "Emulates a proprietary basic school system", 1, false, true, -1},
		{"entry", "e", "The entry point of the program", 1, false, false, -1},
		{"log-level", "l", "Specifies the log level: 0 - no logging, 1 - errors only, 2 - warnings, 3 - info, 4 - debug", 1, false, false, -1},
		{"log-file", "f", "Specifies the log file", 1, false, false, -1}
	});

	if (args.parse(argc, argv) == EXIT_FAILURE)
		return EXIT_FAILURE;

	int log_level = 1;
	if (args.isPresent("log-level"))
		log_level = std::stoi(args.getArguments("log-level")[0][0]);
	
	std::ostream &logger_stream = std::cout;
	std::ofstream log_file;
	if (args.isPresent("log-file"))
	{
		log_file.open(args.getArguments("log-file")[0][0]);
		logger_stream.rdbuf(log_file.rdbuf());
	}



	Logger logger(log_level, true, logger_stream);

	Emulator emulator(&logger);

	std::string filename = args.getArguments()[0][0];

	emulator.loadFromDump(filename);

	uint32_t entry = 0xffffffff;
	if (args.isPresent("entry"))
		entry = std::stoul(args.getArguments("entry")[0][0], nullptr, 16);

	if (entry == 0xffffffff) emulator.run();
	else emulator.run(entry);

	emulator.printRegisters();
		
	if (log_file.is_open())
		log_file.close();
	
	return 0;
}