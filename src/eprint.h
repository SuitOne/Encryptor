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

// Prints text with a color and optionally terminates the line or emboldens the text
// Generally trying to follow the same color scheme:
// Red: Errors
// Yellow: Warnings
// Green: Important good information
// Blue: Important notifications or user input queries
template <typename T>
void eprint(const T& text, Color textColor = Color::Default, bool endl = true, bool bold = false) {
	// Definitions
	// Regular text
	#define BLK "\033[0;30m"
	#define RED "\033[0;31m"
	#define GRN "\033[0;32m"
	#define YEL "\033[0;33m"
	#define BLU "\033[0;34m"
	#define MAG "\033[0;35m"
	#define CYN "\033[0;36m"
	#define WHT "\033[0;37m"
	// Regular bold text
	#define BBLK "\033[1;30m"
	#define BRED "\033[1;31m"
	#define BGRN "\033[1;32m"
	#define BYEL "\033[1;33m"
	#define BBLU "\033[1;34m"
	#define BMAG "\033[1;35m"
	#define BCYN "\033[1;36m"
	#define BWHT "\033[1;37m"

	// Convert to string if not already one
	std::ostringstream oss;
	oss << text;
	std::string str = oss.str();

	// Get color code from enum
	std::string colorCode;
	switch (textColor) {
	case Color::Default:
		colorCode = bold ? BWHT : WHT;
		break;
	case Color::Red:
		colorCode = bold ? BRED : RED;
		break;
	case Color::Yellow:
		colorCode = bold ? BYEL : YEL;
		break;
	case Color::Green:
		colorCode = bold ? BGRN : GRN;
		break;
	case Color::Blue:
		colorCode = bold ? BBLU : BLU;
		break;
	default:
		colorCode = bold ? BWHT : WHT;
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