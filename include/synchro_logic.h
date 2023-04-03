#ifndef SYNCHRO_LOGIC_H
#define SYNCHRO_LOGIC_H

#include <iostream>
#include <filesystem>
#include <fstream>
#include <string>
#include <chrono>
#include <ctime>

namespace fs = std::filesystem;

bool compare_files(const fs::path& file1, const fs::path& file2);
// File comparison
// In case some file hasn't changed since last synchro-interval - there's no
// need to blindly copy it again to replica folder


void synchronize_folders(const fs::path& source_folder, const fs::path& destination_folder, const fs::path& logfile);
// File comparison
// In case some file hasn't changed since last synchro-interval - there's no
// need to blindly copy it again to replica folder


void log(const std::string& message, const std::string& filename);
// Log to file the changes

#endif // SYNCHRO_LOGIC_H