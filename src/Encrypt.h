#pragma once
#include <iostream>
#include <filesystem>
#include <fstream>

class encrypt {
public:
	/// Encrypt the given file with the provided seed
	static void encryptFile(const std::filesystem::path& filePath, const std::string& seed, bool recursive = false);

	// Generates a key file for the given seed
	static void generateKeyFile(const std::filesystem::path& dirPath, const std::string& seed);
};