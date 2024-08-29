#pragma once

#include <filesystem>
#include "eprint.h"

// Returns a valid directory from input string. Returns empty if path is invalid
std::filesystem::path getDirectory(const std::string& input);

// Returns a list of files inside of a directory
std::vector<std::filesystem::path> getFiles(const std::filesystem::path inputDir);