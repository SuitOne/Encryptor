#include "Decrypt.h"

void decrypt::decryptFile(const std::filesystem::path& filePath, const std::string& seed) {
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