#pragma once

#include <iostream>
#include <filesystem>
#include <fstream>
#include <algorithm>
#include "../tools/eprint.h"
#include "../tools/edir.h"

class decrypt {
public:
	// Decrypt all discovered files in a directory
	static void decryptDirectory(const std::filesystem::path& dirPath, const std::string& seed);

	// Decrypt a file at the given path with the given string
	static void decryptFile(const std::filesystem::path& filePath, const std::string& seed);
};