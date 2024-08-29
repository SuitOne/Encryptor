#pragma once

#include <string>
#include <algorithm>
#include <cctype>
#include <vector>
#include <regex>
#include <sstream>
#include <functional>
#include <map>
#include "../encryptor.h"
#include "../tools/eprint.h"
#include "../tools/edir.h"
#include "../encryption/encrypt.h"
#include "../encryption/decrypt.h"

struct Command {
	std::string name;
	std::string description;
	std::function<void(const std::map<std::string, std::string>&)> execute;
};

// Return a vector containing all available commands
std::vector<Command> initializeCommands();

// Parse user input for a command and execute if found
void parseCommand(const std::string& input, const std::vector<Command>& commands);

// Split input into vector<string>, separated by spaces
std::vector<std::string> split(const std::string& input);

// Convert an input string to lowercase
std::string toLowerCase(const std::string& input);

// Start the shell and handle the main shell loop
void initializeShell();