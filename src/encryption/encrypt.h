#pragma once

#include <iostream>
#include <filesystem>
#include <fstream>
#include "../tools/eprint.h"
#include "../tools/edir.h"

class encrypt {
public:
	/// Encrypt the given file with the provided seed
	static void encryptFile(const std::filesystem::path& filePath, const std::string& seed, bool recursive = false);

	// Encrypts a directory of files with encryptFile
	static void encryptDirectory(const std::filesystem::path& dirPath, const std::string& seed);

	// Generates a key file for the given seed
	static void generateKeyFile(const std::filesystem::path& dirPath, const std::string& seed);

	// Returns a string seed of seedLength digits
	static std::string generateSeed(int seedLength);
};