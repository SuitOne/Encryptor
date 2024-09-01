// Encryptor.cpp : Defines the entry point for the application.

#include "encryptor.h"

std::filesystem::path programFilePath;

int main(int argc, char* argv[])
{
	// Set global
	if (argc > 0 && argv[0]) {
		programFilePath = std::filesystem::absolute(argv[0]);
	}
	else {
		eprint("Failed to determine program path", Color::Red);
		return 1;
	}
	

	// Shell init
	initializeShell();

	return 0;
}