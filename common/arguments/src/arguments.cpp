#include "arguments.hpp"

int
Arguments::parse(const int argc, const char** argv) {
    std::vector<std::string> args;
    for (int i = 0; i < argc; i++) {
        args.push_back(argv[i]);
    }
    return parse(args);
}

void
Arguments::printHelp(std::ostream& out) {
    out << "Usage:  " << program_invocation_name;
    if(argSpecs[0].numArgs == 0){
        out << " ";
    }else if(argSpecs[0].numArgs == 1){
        out << " <arg> ";
    }else if(argSpecs[0].numArgs == 2){
        out << " <arg1> <arg2> ";
    }else {
        out << " <arg1> ... <arg" << argSpecs[0].numArgs << "> ";
    }
    out << " [options]" << std::endl;
    out << " Options: " << std::endl;
    for (int i = 1; i < argSpecs.size(); i++) {
        Option argSpec = argSpecs[i];
        out << " ";
        if (argSpec.required) {
            out << "(required) ";
        }
        out << " (";
        if (argSpec.shortName != "") {
            out << "-" << argSpec.shortName << " | ";
        }
        out << "--" << argSpec.name << ")";
        if (argSpec.numArgs == 0) {
            out << " ";
        } else if (argSpec.numArgs == 1) {
            out << " <arg> ";
        }else if (argSpec.numArgs == 2) {
            out << " <arg1> <arg2> ";
        } else {
            out << " <arg1> ... <arg" << argSpec.numArgs << "> ";
        }
        out << std::endl << "   Description: \n    " << argSpec.description << std::endl << std::endl;
    }
}

int
Arguments::parse(std::vector<std::string> args) {
    for (int i = 0; i < args.size(); i++) {
        if (args[i] == "-h" || args[i] == "--help") {
            printHelp();
            return EXIT_FAILURE;
        }
    }
    int currentOption = 0;
    std::vector<std::string> currentArguments; 
    args.erase(args.begin());
    for(std::string arg : args){
        int i = 1;
        while(i < argSpecs.size()){
            if(arg == "--" + argSpecs[i].name || arg == '-' + argSpecs[i].shortName){
                if(!currentArguments.empty() && currentArguments.size() == argSpecs[currentOption].numArgs){
                    arguments[currentOption].push_back(currentArguments);
                    currentArguments.clear();
                }else if(!currentArguments.empty()){
                    std::cerr << "Error: Argument --" << argSpecs[currentOption].name << " requires " << argSpecs[currentOption].numArgs << " arguments, but " << currentArguments.size() << " were supplied" << std::endl;
                    printHelp();
                    return EXIT_FAILURE;
                }
                currentOption = i;
                break;
            }
            i++;
        }
        if( argSpecs.size() == i){
            currentArguments.push_back(arg);
            if(currentArguments.size() == argSpecs[currentOption].numArgs){
                arguments[currentOption].push_back(currentArguments);
                currentArguments.clear();
                currentOption = 0;
            }
        }
    }
    if(!currentArguments.empty() && currentArguments.size() == argSpecs[currentOption].numArgs){
        arguments[currentOption].push_back(currentArguments);
    }else if(!currentArguments.empty()){
        std::cerr << "Error: Argument --" << argSpecs[currentOption].name << " requires " << argSpecs[currentOption].numArgs << " arguments, but " << currentArguments.size() << " were supplied" << std::endl;
        printHelp();
        return EXIT_FAILURE;
    }
    for(int i = 0; i < argSpecs.size(); i++){
        if(argSpecs[i].required && arguments[i].empty()){
            std::cerr << "Error: Required argument --" << argSpecs[i].name << " not supplied" << std::endl;
            printHelp();
            return EXIT_FAILURE;
        }
        if(!argSpecs[i].multiple && arguments[i].size() > 1){
            std::cerr << "Error: Argument --" << argSpecs[i].name << " can only be supplied once" << std::endl;
            printHelp();
            return EXIT_FAILURE;
        }
    }
    return EXIT_SUCCESS;
}

bool
Arguments::isPresent(std::string name) {
    for (int i = 0; i < argSpecs.size(); i++) {
        if (argSpecs[i].name == name) {
            return arguments[i].size() > 0;
        }
    }
    return false;
}

std::vector<std::vector<std::string>>
Arguments::getArguments(std::string name) {
    for (int i = 0; i < argSpecs.size(); i++) {
        if (argSpecs[i].name == name) {
            return arguments[i];
        }
    }
    return std::vector<std::vector<std::string>>();
}