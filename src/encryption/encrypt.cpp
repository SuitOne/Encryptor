#include "encrypt.h"

void encrypt::encryptDirectory(const std::filesystem::path& dirPath, const std::string& seed, bool generateKey) {
	eprint("Encrypting directory: " + dirPath.string());

	// Get files in directory
	auto files = getFiles(dirPath);

	// Check if there are files in the directory
	if (files.empty()) {
		eprint("Directory contains no files: " + dirPath.string(), Color::Yellow);
		return;
	}

	// Encrypt each file
	for (const auto& file : files) {
		encryptFile(file, seed);
	}

	// Generate key file
	if (generateKey) {
		generateKeyFile(dirPath, seed);
	}

	eprint("Encrypted " + std::to_string(files.size()) + " files from " + dirPath.string(), Color::Green);
}

void encrypt::encryptDirectoryRecursive(const std::filesystem::path& dirPath, const std::string& seed) {
	eprint("Encrypting directory recursively: " + dirPath.string());

	// Establish vectors of directories and files in root directory
	std::vector<std::filesystem::path> directories = { dirPath };

	// Establish counters
	int dirCount = 0;

	// Loop through main directory searching through all subdirectories
	while (!directories.empty()) {
		// Get the next directory to process
		std::filesystem::path currentDir = directories.back();
		directories.pop_back();
		dirCount++;

		// Add subdirectories
		auto subDirs = getDirectories(currentDir);
		directories.insert(directories.end(), subDirs.begin(), subDirs.end());

		// Encrypt directory
		encryptDirectory(currentDir, seed);
	}

	// Generate key file
	generateKeyFile(dirPath, seed);

	eprint("Encrypted " + std::to_string(dirCount) + " directories starting at " + dirPath.string(), Color::Green);
}

void encrypt::encryptFile(const std::filesystem::path& filePath, const std::string& seed, bool recursive) {
	eprint("Encrypting file: " + filePath.string());

	// Make sure file is not the program
	if (comparePaths(filePath, programFilePath)) {
		eprint("Cannot encrypt Encryptor", Color::Red);
		return;
	}

	// Get original file extension
	std::string originalExtension = filePath.extension().string();

	// Make sure file isn't encrypted
	if (originalExtension == ".ses") {
		eprint("File already encrypted: " + filePath.string(), Color::Yellow);
		return;
	}

	// Open the file for reading
	std::ifstream inputFile(filePath, std::ios::binary);
	if (!inputFile) {
		eprint("Error opening file for reading: " + filePath.string(), Color::Red);
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
		eprint("Error opening file for writing: " + filePath.string(), Color::Red);
		return;
	}
	outputFile.write(buffer.data(), buffer.size());
	outputFile.close();

	// Delete the original file
	std::filesystem::remove(filePath);

	eprint("Successfully encrypted: " + newFilePath.string(), Color::Green);
}

void encrypt::generateKeyFile(const std::filesystem::path& dirPath, const std::string& seed) {
	eprint("Generating key for seed: " + seed);

	// Generate the file 
	std::filesystem::path keyFilePath = dirPath / "key.txt";

	// Write seed to key file
	std::ofstream keyOutput(keyFilePath);
	keyOutput << seed;
	keyOutput.close();

	eprint("Key successfully generated: " + keyFilePath.string(), Color::Green);
}

std::string encrypt::generateSeed(int seedLength) {
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