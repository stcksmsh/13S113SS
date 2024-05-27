#include "arguments.hpp"

Arguments::Arguments(std::vector<Option> argSpecs) : argSpecs()
{
    Option program_option = {
        "",
        "",
        "The main program",
        0,
        false,
        false,
        -1};
    this->argSpecs.push_back(program_option);
    for (Option argSpec : argSpecs)
    {
        if (argSpec.name == "" && argSpec.shortName == "")
        {
            if (this->argSpecs.front().exclusiveGroup == -2)
            {
                std::cerr << "Error: Argument with no name or shortName specifies the main program parameters, but multiple of these were supplied" << std::endl;
                printHelp();
                exit(EXIT_FAILURE);
            }
            this->argSpecs.front().numArgs += argSpec.numArgs;
            this->argSpecs.front().description = argSpec.description;
            this->argSpecs.front().required = argSpec.required;
            this->argSpecs.front().multiple = argSpec.multiple;
            this->argSpecs.front().exclusiveGroup = -2;
            continue;
        }
        this->argSpecs.push_back(argSpec);
    }

    arguments.push_back(std::vector<std::vector<std::string>>()); // Add default argument (the arguments not for any option)
    for (Option argSpec : argSpecs)
    {
        arguments.push_back(std::vector<std::vector<std::string>>());
    }
}

int Arguments::parse(const int argc, const char **argv)
{
    std::vector<std::string> args;
    for (int i = 0; i < argc; i++)
    {
        args.push_back(argv[i]);
    }
    return parse(args);
}

void Arguments::printHelp(std::ostream &out)
{
    out << "Usage:  " << program_invocation_name;
    if (argSpecs[0].numArgs == 0)
    {
        out << " ";
    }
    else if (argSpecs[0].numArgs == 1)
    {
        out << " <arg> ";
    }
    else if (argSpecs[0].numArgs == 2)
    {
        out << " <arg1> <arg2> ";
    }
    else
    {
        out << " <arg1> ... <arg" << argSpecs[0].numArgs << "> ";
    }
    out << " [options]" << std::endl
        << std::endl;
    out << " Description: \n  " << argSpecs[0].description << std::endl
        << std::endl;
    out << " Options: " << std::endl;
    for (int i = 1; i < argSpecs.size(); i++)
    {
        Option argSpec = argSpecs[i];
        out << " ";
        if (argSpec.required)
        {
            out << "(required) ";
        }
        if (argSpec.shortName != "" || argSpec.name != "")
            out << " (";
        if (argSpec.name != "")
        {
            out << " --" << argSpec.name;
            if (argSpec.shortName != "")
            {
                out << " | ";
            }
            else
            {
                out << " )";
            }
        }
        if (argSpec.shortName != "")
        {
            out << "-" << argSpec.shortName << " )";
        }
        if (argSpec.numArgs == 0)
        {
            out << " ";
        }
        else if (argSpec.numArgs == 1)
        {
            out << " <arg> ";
        }
        else if (argSpec.numArgs == 2)
        {
            out << " <arg1> <arg2> ";
        }
        else
        {
            out << " <arg1> ... <arg" << argSpec.numArgs << "> ";
        }
        out << " : " << argSpec.description << std::endl;
        if (argSpec.exclusiveGroup != -1)
        {
            out << "      Exclusive group: " << argSpec.exclusiveGroup << std::endl;
        }
        out << std::endl;
    }
}

int Arguments::parse(std::vector<std::string> args)
{
    for (int i = 0; i < args.size(); i++)
    {
        if (args[i] == "-h" || args[i] == "--help")
        {
            printHelp();
            return EXIT_FAILURE;
        }
    }
    int currentOption = 0;
    std::vector<std::string> currentArguments;
    args.erase(args.begin());
    for (std::string arg : args)
    {
        int i = 1;
        while (i < argSpecs.size())
        {
            if (arg == "--" + argSpecs[i].name || arg == '-' + argSpecs[i].shortName)
            {
                if (!currentArguments.empty() && (currentArguments.size() == argSpecs[currentOption].numArgs || argSpecs[currentOption].numArgs == -1))
                {
                    arguments[currentOption].push_back(currentArguments);
                    currentArguments.clear();
                }
                else if (!currentArguments.empty())
                {
                    if(argSpecs[currentOption].name == "" && argSpecs[currentOption].shortName == "")
                    {
                        std::cerr << "Error: Program requires " << argSpecs[currentOption].numArgs << " arguments, but " << currentArguments.size() << " were supplied" << std::endl;
                    }else{
                        std::cerr << "Error: Option --" << argSpecs[currentOption].name << " requires " << argSpecs[currentOption].numArgs << " arguments, but " << currentArguments.size() << " were supplied" << std::endl;
                    }
                    printHelp();
                    return EXIT_FAILURE;
                }
                currentOption = i;
                if (argSpecs[currentOption].exclusiveGroup != -1)
                {
                    exclusiveGroups[argSpecs[currentOption].exclusiveGroup].push_back(argSpecs[currentOption].name);
                }
                if (argSpecs[currentOption].numArgs == 0)
                {
                    arguments[currentOption].push_back(std::vector<std::string>());
                    currentOption = 0;
                }
                break;
            }
            i++;
        }
        if (argSpecs.size() == i)
        {
            currentArguments.push_back(arg);
            if (currentArguments.size() == argSpecs[currentOption].numArgs)
            {
                arguments[currentOption].push_back(currentArguments);
                currentArguments.clear();
                currentOption = 0;
            }
        }
    }
    if (!currentArguments.empty() && (currentArguments.size() == argSpecs[currentOption].numArgs || argSpecs[currentOption].numArgs == -1))
    {
        arguments[currentOption].push_back(currentArguments);
    }
    else if (!currentArguments.empty())
    {
        if(argSpecs[currentOption].name == "" && argSpecs[currentOption].shortName == "")
        {
            std::cerr << "Error: Program requires " << argSpecs[currentOption].numArgs << " arguments, but " << currentArguments.size() << " were supplied" << std::endl;
        }else{
            std::cerr << "Error: Option --" << argSpecs[currentOption].name << " requires " << argSpecs[currentOption].numArgs << " arguments, but " << currentArguments.size() << " were supplied" << std::endl;
        }
        printHelp();
        return EXIT_FAILURE;
    }else if(argSpecs[currentOption].numArgs != 0 && !( currentOption == 0 && currentArguments.empty() ))
    {
        if(argSpecs[currentOption].name == "" && argSpecs[currentOption].shortName == "")
        {
            std::cerr << "Error: Program requires " << argSpecs[currentOption].numArgs << " arguments, but none were supplied" << std::endl;
        }else{
            std::cerr << "Error: Option --" << argSpecs[currentOption].name << " requires " << argSpecs[currentOption].numArgs << " arguments, but none were supplied" << std::endl;
        }
        printHelp();
        return EXIT_FAILURE;
    }
    for (int i = 0; i < argSpecs.size(); i++)
    {
        if (argSpecs[i].required && arguments[i].empty())
        {
            if(argSpecs[i].name == "")
            {
                std::cerr << "Error: required arguments not supplied" << std::endl;
            }else{
                std::cerr << "Error: Required option --" << argSpecs[i].name << " not supplied" << std::endl;
            }
            printHelp();
            return EXIT_FAILURE;
        }
        if (!argSpecs[i].multiple && arguments[i].size() > 1)
        {
            if(argSpecs[i].name == "" && argSpecs[i].shortName == "")
            {
                std::cerr << "Error: Program arguments can only be supplied once" << std::endl;
            }else{
                std::cerr << "Error: Option --" << argSpecs[i].name << " arguments can only be supplied once" << std::endl;
            }
            printHelp();
            return EXIT_FAILURE;
        }
        if (argSpecs[i].exclusiveGroup != -1 && exclusiveGroups[argSpecs[i].exclusiveGroup].size() > 1)
        {
            std::cerr << "Error: Options with exclusiveGroup " << argSpecs[i].exclusiveGroup << " are exclusive, but multiple were supplied:\n  ";
            for (std::string arg : exclusiveGroups[argSpecs[i].exclusiveGroup])
            {
                std::cerr << "  --" << arg;
            }
            std::cerr << std::endl
                      << std::endl;
            printHelp();
            return EXIT_FAILURE;
        }
    }
    return EXIT_SUCCESS;
}

bool Arguments::isPresent(std::string name)
{
    for (int i = 0; i < argSpecs.size(); i++)
    {
        if (argSpecs[i].name == name)
        {
            return arguments[i].size() > 0;
        }
    }
    return false;
}

std::vector<std::vector<std::string>>
Arguments::getArguments(std::string name)
{
    for (int i = 0; i < argSpecs.size(); i++)
    {
        if (argSpecs[i].name == name)
        {
            return arguments[i];
        }
    }
    return std::vector<std::vector<std::string>>();
}