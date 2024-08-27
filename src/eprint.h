#pragma once

#include <string>
#include <sstream>
#include <iostream>

enum class Color {
	Default,
	Red,
	Yellow,
	Green,
	Blue
};

// Prints text with a color and optionally terminates the line
// Generally trying to follow the same color scheme:
// Red: Errors
// Yellow: Warnings
// Green: Important good information
// Blue: Important notifications or user input queries
template <typename T>
void eprint(const T& text, Color textColor = Color::Default, bool endl = true) {
	// Convert to string if not already one
	std::ostringstream oss;
	oss << text;
	std::string str = oss.str();

	// Get color code from enum
	std::string colorCode;
	switch (textColor) {
	case Color::Default:
		colorCode = "\033[0m";
		break;
	case Color::Red:
		colorCode = "\033[1;31m";
		break;
	case Color::Yellow:
		colorCode = "\033[1;33m";
		break;
	case Color::Green:
		colorCode = "\033[1;32m";
		break;
	case Color::Blue:
		colorCode = "\033[1;34m";
		break;
	default:
		colorCode = "\033[0m";
		break;
	}

	// Output text
	if (endl) {
		std::cout << colorCode << text << std::endl;
	}
	else {
		std::cout << colorCode << text;
	}
}

// Prints an exception using eprint
void eprinterror(const std::exception& e);