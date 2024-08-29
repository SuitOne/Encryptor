#include "shell.h"

void initializeShell() {
    // CLI init
    system(("title Encryptor " + VERSION).c_str());
    eprint("Encryptor " + VERSION + " Initialized", Color::Blue, true, true);
    eprint("Try 'Help' to see the command list", Color::Blue);

    // Commands init
    auto commands = initializeCommands();

    // Shell loop
    while (true) {
        std::string input;
        eprint("> ", Color::Blue, false);
        std::getline(std::cin, input);
        parseCommand(toLowerCase(input), commands);
    }
}

void parseCommand(const std::string& input, const std::vector<Command>& commands) {
    // Split the input, discard if empty
    auto tokens = split(input);
    if (tokens.empty()) return;

    // Break up tokens into command and params
    std::string commandName = tokens[0];
    std::map<std::string, std::string> parameters;
    for (size_t i = 1; i < tokens.size(); i++) {
        if (tokens[i][0] == '-') { // If the token is a flag (starts with '-')
            if (i + 1 < tokens.size() && tokens[i + 1][0] != '-') { // Ensure there is a value after the flag
                parameters[tokens[i]] = tokens[i + 1]; // Map the flag to its value
                i++; // Skip the next token as it is the value
            }
            else {
                parameters[tokens[i]] = ""; // Flag without a value
            }
        }
    }

    // Check for help command
    if (commandName == "help") {
        // List all initialized commands and their descriptions
        for (const Command& c : commands) {
            eprint(c.name, Color::Blue);
            eprint(c.description);
            eprint("");
        }

        return;
    }

    // Check for matching command
    for (const Command& c : commands) {
        if (c.name == commandName) {
            c.execute(parameters);
            return; // Command found and executed
        }
    }

    // No command found
    eprint("Unknown command: " + tokens[0], Color::Red);
}

std::vector<std::string> split(const std::string& input) {
    std::vector<std::string> tokens;
    std::regex regex(R"((\"[^\"]*\")|(\S+))"); // Regex to match quoted strings or non-whitespace sequences
    std::sregex_iterator iter(input.begin(), input.end(), regex);
    std::sregex_iterator end;

    while (iter != end) {
        // Remove quotes if the token is enclosed in quotes
        std::string token = (*iter)[0].str();
        if (token.front() == '"' && token.back() == '"') {
            token = token.substr(1, token.size() - 2);
        }
        tokens.push_back(token);
        ++iter;
    }

    return tokens;
}

std::string toLowerCase(const std::string& input) {
    std::string result = input; // Make a copy of the input string
    std::transform(result.begin(), result.end(), result.begin(),
        [](unsigned char c) { return std::tolower(c); });
    return result;
}

std::vector<Command> initializeCommands() {
    // All commands, separated by a line
    // Name
    // Description
    // Lambda

    return {
        {"encrypt",
        "Encrypt files in the directory\nRequired Parameters:\n -Directory: Directory to encrypt",
        [](const std::map<std::string, std::string>& params) {
            // Check for required parameter '-directory'
            auto dirIt = params.find("-directory");
            if (dirIt == params.end() || dirIt->second.empty()) {
                eprint("Error: Missing required parameter '-directory' or its argument.", Color::Red);
                return;
            }

            // Retrieve parameters
            std::string directory = dirIt->second; // Get the directory value
            bool recursive = (params.count("-recursive") && params.at("-recursive") == "true");

            // Execute
            encrypt::encryptDirectory(directory, encrypt::generateSeed(16));
        }},

        {"decrypt",
        "Decrypt files in the directory\nRequired Parameters:\n -Directory: Directory to decrypt\n -Seed: Decryption seed",
        [](const std::map<std::string, std::string>& params) {
            // Check for required parameters '-directory' and '-seed'
            auto dirIt = params.find("-directory");
            auto seedIt = params.find("-seed");
            if (dirIt == params.end() || dirIt->second.empty()) {
                eprint("Error: Missing required parameter '-directory' or its argument.", Color::Red);
                return;
            }
            else if (seedIt == params.end() || seedIt->second.empty()) {
                eprint("Error: Missing required parameter '-seed' or its argument.", Color::Red);
                return;
            }

            // Execute
            decrypt::decryptDirectory(dirIt->second, seedIt->second);
        }},

        {"exit", 
        "Exits the program", 
        [](const std::map<std::string, std::string>&) { exit(0); }}
    };
}