// Encryptor.cpp : Defines the entry point for the application.

#include "Encryptor.h"

int main()
{
	std::cout << "Encryptor initialized" << std::endl;

	// Select directory
	std::filesystem::path dirPath = getDirectory();
	std::cout << "Selected directory: " << dirPath << std::endl;

	// Get command
	int userChoice = 0;
	while (true) {
		std::cout << "Please enter '1' to encrypt or '2' to decrypt: ";

		try {
			std::string userChoiceStr;
			std::getline(std::cin, userChoiceStr);
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

	// TODO: Add option to return to beginning here
	// or hold the thread to allow seed copy

	return 0;
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
				encryptFile(entry.path(), seed);
			}
			else {
				decryptFile(entry.path(), seed);
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

void encryptFile(const std::filesystem::path& filePath, const std::string& seed) {
	std::cout << "Encrypting file: " << filePath << std::endl;
	
	// Get original file extension
	std::string originalExtension = filePath.extension().string();

	// Open the file for reading
	std::ifstream inputFile(filePath, std::ios::binary);
	if (!inputFile) {
		std::cerr << "Error opening file for reading : " << filePath << std::endl;
		return;
	}

	// Read file content into buffer
	std::vector<char> buffer((std::istreambuf_iterator<char>(inputFile)), std::istreambuf_iterator<char>());
	inputFile.close();

	// Prepend the original extension to the buffer with a null term
	buffer.insert(buffer.begin(), originalExtension.begin(), originalExtension.end());
	buffer.insert(buffer.begin() + originalExtension.size(), '\0');

	// Flip bits according to seed
	for (size_t i = 0; i < buffer.size(); ++i) {
		int flipTimes = seed[i % seed.size()] - '0'; // Convert char digit to int
		buffer[i] ^= flipTimes; // XOR operation for encryption
	}

	// Change file extension to .ses
	std::filesystem::path newFilePath = filePath;
	newFilePath.replace_extension(".ses");

	// Write encrypted contents back to file with new extension
	std::ofstream outputFile(newFilePath, std::ios::binary);
	if (!outputFile) {
		std::cerr << "Error opening file for writing: " << filePath << std::endl;
		return;
	}
	outputFile.write(buffer.data(), buffer.size());
	outputFile.close();

	//Delete the original file
	std::filesystem::remove(filePath);

	std::cout << "Successfully encrypted: " << newFilePath << std::endl;
}

void decryptFile(const std::filesystem::path& filePath, const std::string& seed) {
	std::cout << "Decrypting file: " << filePath << std::endl;

	// Open the encrypted file for reading
	std::ifstream inputFile(filePath, std::ios::binary);
	if (!inputFile) {
		std::cerr << "Error opening file for reading: " << filePath << std::endl;
		return;
	}
	
	// Read the encrypted contents into a buffer
	std::vector<char> buffer((std::istreambuf_iterator<char>(inputFile)), std::istreambuf_iterator<char>());
	inputFile.close();

	// Flip bits according to seed
	for (size_t i = 0; i < buffer.size(); ++i) {
		int flipTimes = seed[i % seed.size()] - '0'; // Convert char digit to int
		buffer[i] ^= flipTimes; // XOR operation for decryption
	}

	// Extract the original extension from the decrypted buffer
	auto nullPos = find(buffer.begin(), buffer.end(), '\0');
	if (nullPos == buffer.end()) {
		std::cerr << "Error: Original file extension not found." << std::endl;
		return;
	}

	std::string originalExtension(buffer.begin(), nullPos);
	buffer.erase(buffer.begin(), nullPos + 1);

	// Restore the file extension
	std::filesystem::path newFilePath = filePath;
	newFilePath.replace_extension(originalExtension);

	// Write the decrypted contents back to the file with the original extension
	std::ofstream outputFile(newFilePath, std::ios::binary);
	if (!outputFile) {
		std::cerr << "Error opening file for writing: " << newFilePath << std::endl;
		return;
	}
	outputFile.write(buffer.data(), buffer.size());
	outputFile.close();

	// Delete the encrypted file
	std::filesystem::remove(filePath);
}