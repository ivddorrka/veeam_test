#include "synchro_logic.h"
#include "error_codes.h"

bool compare_files(const fs::path& file_src, const fs::path& file_repl) {
    if (fs::file_size(file_src) != fs::file_size(file_repl)) {
        return false;
    }

    std::ifstream f_src(file_src.string(), std::ios::binary);
    std::ifstream f_repl(file_repl.string(), std::ios::binary);

    return std::equal(std::istreambuf_iterator<char>(f_src.rdbuf()),
                      std::istreambuf_iterator<char>(),
                      std::istreambuf_iterator<char>(f_repl.rdbuf()));
}

void synchronize_folders(const fs::path& source_folder, const fs::path& replica_folder, const fs::path& logfile) {

    // Main loop that simply compares two folders and creates / alters files in case the were changed / created
    for (const auto& entry : fs::recursive_directory_iterator(source_folder)) {
        const auto relative_path = fs::relative(entry.path(), source_folder);
        const auto replica_path = replica_folder / relative_path;

        if (fs::is_directory(entry)) {
            if (!fs::exists(replica_path)) {
                fs::create_directory(replica_path);
                const std::string message = "directory " + replica_path.string() + " " + " created";
                std::cout << message << std::endl;
                log(message, logfile);
            }

        } else if (fs::is_regular_file(entry)) {
            if (!fs::exists(replica_path)) {
                const std::string message = entry.path().string() + " " + " created";
                std::cout << message << std::endl;
                log(message, logfile);
                fs::copy_file(entry.path(), replica_path, fs::copy_options::overwrite_existing);
            }
            else if (!compare_files(entry.path(), replica_path)) {
                const std::string message = entry.path().string() + " " + " altered";
                std::cout << message << std::endl;
                log(message, logfile);
                fs::copy_file(entry.path(), replica_path, fs::copy_options::overwrite_existing);
            }
        }
    }


    // This loop checks if there're some files still left in replica folder that are not supposed to stay
    for (const auto& file : std::filesystem::recursive_directory_iterator(replica_folder)) {
        std::string relative_path = std::filesystem::relative(file.path(), replica_folder);
        std::filesystem::path source_file_path = source_folder.string() +"/"+ relative_path;
        if (!std::filesystem::exists(source_file_path)) {
            const std::string message = file.path().string() + " " + " removed";
            std::cout << message<< std::endl;
            log(message, logfile);
            std::filesystem::remove(file.path());
        }
    }
}


void log(const std::string& message, const std::string& log_file_path){

    auto now = std::chrono::system_clock::now();
    std::time_t time = std::chrono::system_clock::to_time_t(now);

    std::string time_string = std::ctime(&time);
    time_string.erase(time_string.length() - 1);

    std::ofstream logfile;

    logfile.open(log_file_path, std::ios::app);

    logfile << "[" << time_string << "] " << message << std::endl;

}
