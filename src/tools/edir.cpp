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

std::vector<std::filesystem::path> getDirectories(const std::filesystem::path inputDir) {
	// Declare vector
	std::vector<std::filesystem::path> directories;

	// Parse the directory for each file
	for (const auto& entry : std::filesystem::directory_iterator(inputDir)) {
		if (std::filesystem::is_directory(entry.status())) {
			eprint("Discovered directory: " + entry.path().string());
			directories.push_back(entry.path());
		}
	}

	// Final message
	eprint("Total directories discovered: " + std::to_string(directories.size()), Color::Green);

	return directories; // Return all discovered directories
}

std::vector<std::filesystem::path> getFiles(const std::filesystem::path inputDir) {
	// Declare vector
	std::vector<std::filesystem::path> files;

	// Parse the directory for each file
	try {
		for (const auto& entry : std::filesystem::directory_iterator(inputDir)) {
			if (std::filesystem::is_regular_file(entry.status())) {
				eprint("Discovered file: " + entry.path().string());
				files.push_back(entry.path());
			}
		}
	}
	catch (std::exception& e) {
		eprinterror(e);
	}
	

	// Final message
	eprint("Total files discovered: " + std::to_string(files.size()), Color::Green);

	return files; // Return all discovered files
}

bool comparePaths(const std::filesystem::path& path1, std::filesystem::path& path2) {
	// Convert the paths to their generic format (with forward slashes)
	std::string normalizedPath1 = path1.generic_string();
	std::string normalizedPath2 = path2.generic_string();

	// Convert both strings to lowercase
	std::transform(normalizedPath1.begin(), normalizedPath1.end(), normalizedPath1.begin(), ::tolower);
	std::transform(normalizedPath2.begin(), normalizedPath2.end(), normalizedPath2.begin(), ::tolower);

	return normalizedPath1 == normalizedPath2;
}