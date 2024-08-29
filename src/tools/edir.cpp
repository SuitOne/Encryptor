#include "edir.h"

std::filesystem::path getDirectory(const std::string& input) {
	// Defines the path variable
	std::filesystem::path path(input);

    // Ensure the path is valid
    if (!std::filesystem::exists(path)) {
        eprint("Error: Path does not exist: " + input, Color::Red);
        return std::filesystem::path(); // Return an empty path
    }

    if (!std::filesystem::is_directory(path)) {
        eprint("Error: Path is not a directory: " + input, Color::Red);
        return std::filesystem::path(); // Return an empty path
    }

    return path; // Valid path
}

std::vector<std::filesystem::path> getFiles(const std::filesystem::path inputDir) {
	// Declare vector
	std::vector<std::filesystem::path> files;

	// Parse the directory for each file
	for (const auto& entry : std::filesystem::directory_iterator(inputDir)) {
		if (std::filesystem::is_regular_file(entry.status())) {
			eprint("Discovered file: " + entry.path().string());
			files.push_back(entry.path());
		}
	}

	// Final message
	eprint("Total files discovered: " + std::to_string(files.size()), Color::Green);

	return files; // Return all discovered files
}