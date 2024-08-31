#pragma once

#include <vector>
#include <filesystem>
#include "eprint.h"

// Returns a valid directory from input string. Returns empty if path is invalid
std::filesystem::path getDirectory(const std::string& input);

// Returns a list of directories inside of a directory
std::vector<std::filesystem::path> getDirectories(const std::filesystem::path inputDir);

// Returns a list of files inside of a directory
std::vector<std::filesystem::path> getFiles(const std::filesystem::path inputDir);

// Returns true if path1 and path2 are the same
bool comparePaths(const std::filesystem::path& path1, std::filesystem::path& path2);