﻿#pragma once

#include <iostream>
#include <cstdlib>
#include <string>
#include <filesystem>
#include <fstream>
#include "encrypt.h"
#include "decrypt.h"
#include "eprint.h"

// Forward declared functions

/// <summary>Returns a string seed of seedLength digits</summary>
std::string generateSeed(int seedLength);

/// <summary>Returns a directory from user input</summary>///
std::filesystem::path getDirectory();

/// <summary>Iterate through a given directory and encrypt each file with the given seed</summary>///
void processDirectory(const std::filesystem::path& dirPath, const int& userChoice, const std::string& seed);