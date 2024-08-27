#include "eprint.h"

void eprinterror(const std::exception& e) {
	eprint("Error: " + std::string(e.what()), Color::Red);
}