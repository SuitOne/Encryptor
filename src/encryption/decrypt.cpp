#include "decrypt.h"

void decrypt::decryptDirectory(const std::filesystem::path& dirPath, const std::string& seed) {
	eprint("Decrypting directory: " + dirPath.string());

	// Get files in directory
	auto files = getFiles(dirPath);

	// Check if there are files in the directory
	if (files.empty()) {
		eprint("Directory contains no files: " + dirPath.string(), Color::Yellow);
		return;
	}

	// Encrypt each file
	for (const auto& file : files) {
		decryptFile(file, seed);
	}

	eprint("Decrypted " + std::to_string(files.size()) + " files from " + dirPath.string(), Color::Green);
}

void decrypt::decryptFile(const std::filesystem::path& filePath, const std::string& seed) {
	eprint("Decrypting file: " + filePath.string());

	// Open the encrypted file for reading
	std::ifstream inputFile(filePath, std::ios::binary);
	if (!inputFile) {
		eprint("Error opening file for reading: " + filePath.string(), Color::Red);
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
		eprint("Error: Original file extension not found.", Color::Red);
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
		eprint("Error opening file for writing: " + newFilePath.string(), Color::Red);
		return;
	}
	outputFile.write(buffer.data(), buffer.size());
	outputFile.close();

	// Delete the encrypted file
	std::filesystem::remove(filePath);

	eprint("Successfully decrypted: " + newFilePath.string(), Color::Green);
}