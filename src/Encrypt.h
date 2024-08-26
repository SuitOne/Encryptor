#pragma once
#include <iostream>
#include <filesystem>
#include <fstream>

class encrypt {
public:
	/// <summary>Encrypt a given file with the given seed</summary>///
	static void encryptFile(const std::filesystem::path& filePath, const std::string& seed, bool recursive = false);
};