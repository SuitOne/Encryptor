// Encryptor.cpp : Defines the entry point for the application.

#include "Encryptor.h"

// Global version declaration
const std::string VERSION = "v1.1";

int main()
{
	// CMD init
	system(("title Encryptor " + VERSION).c_str());
	std::cout << "Encryptor " << VERSION << " Initialized" << std::endl;

	// Main loop
	while (true) {
		// Select directory
		std::filesystem::path dirPath = getDirectory();
		std::cout << "Selected directory: " << dirPath << std::endl;

		// Get command
		int userChoice = 0;
		while (true) {
			std::cout << "Please enter '1' to encrypt, '2' to decrypt: ";

			try {
				std::string userChoiceStr;
				std::getline(std::cin, userChoiceStr);
				if (userChoiceStr.empty()) {
					return 0;
				}
				userChoice = stoi(userChoiceStr);
			}
			catch (std::exception& e) {
				std::cout << "Error: " << e.what() << std::endl;
			}

			if ((userChoice == 1) || (userChoice == 2)) {
				break;
			}

			std::cout << "Invalid choice" << std::endl;
		}

		// Seed generation
		std::string seed;
		if (userChoice == 1) {
			// Encrypt
			// Generate the encryption seed
			seed = generateSeed(16);
			std::cout << "Seed: " << seed << std::endl;
		}
		else {
			// Decrypt
			// Extract the user decryption seed
			while (seed.empty()) {
				std::cout << "Please enter the decryption seed: ";

				try {
					std::string userChoiceStr;
					std::getline(std::cin, seed);

					if (seed.empty()) {
						std::cout << "Invalid choice" << std::endl;
					}
				}
				catch (std::exception& e) {
					std::cout << "Error: " << e.what() << std::endl;
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
			std::cout << "Please enter a file directory: ";

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
				std::cout << "Invalid directory" << std::endl;
				path.clear();
			}
		}
		catch (std::exception& e) {
			std::cout << "Error: " << e.what() << std::endl;
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
			std::cout << "Discovered file: " << entry.path() << std::endl;

			if (userChoice == 1) {
				// Don't encrypt .ses files
				if (entry.path().extension().string() == ".ses") {
					std::cout << "File already encrypted: " << entry.path() << std::endl;
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
		std::cout << "Successfully encrypted files with seed: " << seed << std::endl;
		std::cout << "Please remember to save the seed for later decryption." << std::endl;
	}
	else {
		std::cout << "Successfully decrypted files with seed: " << seed << std::endl;
	}
}