// Encryptor.cpp : Defines the entry point for the application.

#include "Encryptor.h"

// Global version declaration
const std::string VERSION = "v1.2";

int main()
{
	// CMD init
	system(("title Encryptor " + VERSION).c_str());
	eprint("Encryptor " + VERSION + " Initialized", Color::Blue, true, true);

	// Main loop
	while (true) {
		// Select directory
		std::filesystem::path dirPath = getDirectory();
		eprint("Selected directory: " + dirPath.string());

		// Get command
		int userChoice = 0;
		while (true) {
			eprint("Please enter '1' to encrypt, '2' to decrypt: ", Color::Blue, false);

			try {
				std::string userChoiceStr;
				std::getline(std::cin, userChoiceStr);
				if (userChoiceStr.empty()) {
					return 0;
				}
				userChoice = stoi(userChoiceStr);
			}
			catch (std::exception& e) {
				eprinterror(e);
			}

			if ((userChoice == 1) || (userChoice == 2)) {
				break;
			}

			eprint("Invalid choice", Color::Red);
		}

		// Seed generation
		std::string seed;
		if (userChoice == 1) {
			// Encrypt
			// Generate the encryption seed
			seed = generateSeed(16);
			eprint("Seed: " + seed);
		}
		else {
			// Decrypt
			// Extract the user decryption seed
			while (seed.empty()) {
				eprint("Please enter the decryption seed: ", Color::Blue, false);

				try {
					std::string userChoiceStr;
					std::getline(std::cin, seed);

					if (seed.empty()) {
						eprint("Error: Invalid seed", Color::Red);
					}
				}
				catch (std::exception& e) {
					eprinterror(e);
				}
			}
		}

		// Begin file processing
		processDirectory(dirPath, userChoice, seed);
	}

	return 1;
}

std::filesystem::path getDirectory() {
	// Defines the path variable
	std::filesystem::path path;

	// Core loop
	while (path.empty()) {
		try {
			eprint("Please enter a file directory: ", Color::Blue, false);

			// Get user input
			std::string userInput;
			std::getline(std::cin, userInput);

			// Construct the path
			path = std::filesystem::path(userInput);

			// Ensure the path is valid
			if (std::filesystem::exists(path) && std::filesystem::is_directory(path)) {
				return path;
			}
			else {
				eprint("Error: Invalid directory", Color::Red);
				path.clear();
			}
		}
		catch (std::exception& e) {
			eprinterror(e);
			path.clear();
		}
	}

	// This should not be reached
	return path;
}

std::string generateSeed(int seedLength) {
	// Init variables
	std::string seed;

	// Init random
	srand(time(0));
	
	// Generate random digits
	while (seed.length() < seedLength) {
		seed += std::to_string((rand() % 10));
	}

	return seed;
}

void processDirectory(const std::filesystem::path& dirPath, const int& userChoice, const std::string& seed) {
	// Parse the directory for each file
	for (const auto& entry : std::filesystem::directory_iterator(dirPath)) {
		if (std::filesystem::is_regular_file(entry.status())) {
			eprint("Discovered file: " + entry.path().string());

			if (userChoice == 1) {
				// Don't encrypt .ses files
				if (entry.path().extension().string() == ".ses") {
					eprint("File already encrypted: " + entry.path().string(), Color::Yellow);
					continue;
				}
				encrypt::encryptFile(entry.path(), seed);
				encrypt::generateKeyFile(dirPath, seed);
			}
			else {
				decrypt::decryptFile(entry.path(), seed);
			}
		}
	}

	// Final message
	if (userChoice == 1) {
		eprint("Successfully encrypted files with seed: " + seed, Color::Green);
		eprint("Please remember to save the seed for later decryption.");
	}
	else {
		eprint("Successfully decrypted files with seed: " + seed, Color::Green);
	}
}