#pragma once

#include <iostream>
#include <filesystem>
#include <fstream>
#include "eprint.h"

class decrypt {
public:
	static void decryptFile(const std::filesystem::path& filePath, const std::string& seed);
};