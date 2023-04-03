#include <iostream>
#include <filesystem>
#include <chrono>
#include <thread>
#include <csignal>


//headers
#include "error_codes.h"
#include "synchro_logic.h"

namespace fs = std::filesystem;

int main(int argc, char* argv[]) {

//    1 arg - src folder
//    2 arg - replica folder
//    3 arg - time interval

    if (argc != 5){
        std::cerr << "Wrong number of arguments was specified";
        return static_cast<int>(ErrorCode::WRONG_NUMBER_OF_ARGUMENTS);
    }

    fs::path source_folder(argv[1]);
    fs::path replica_folder(argv[2]);
    fs::path logging_file(argv[3]);

    if (!fs::exists(source_folder)) {
        std::cerr << "Source folder doesn't exist";
        return static_cast<int>(ErrorCode::FOLDER_NOT_FOUND);
    }

    if (source_folder==replica_folder){
        std::cerr << "Source and replica have to be different folders!";
        return static_cast<int>(ErrorCode::WRONG_FOLDER_SPECIFIED);
    }
    if (!fs::exists(replica_folder)) {
        fs::create_directory(replica_folder);
    }

    if (fs::exists(logging_file)){
        if (!fs::is_regular_file(logging_file)) {
            std::cerr << "Logging file has to be a regular text file";
            return static_cast<int>(ErrorCode::LOG_FILE_FORMAT_ERROR);
        }
    }

    std::ofstream logfile(logging_file, std::ios_base::app);
    if (!logfile.is_open()) {
        std::cerr << "Failed to open log file";
        return static_cast<int>(ErrorCode::LOG_FILE_WRITE_ERROR);
    }


    int interval = 0;
    if (argc > 4) {
        try {
            interval = std::stoi(argv[4]);
            if (interval <=0){
                std::cerr << "Interval should be positive" <<std::endl;
                return static_cast<int>(ErrorCode::NEGATIVE_INTERVAL);
            }
        } catch (const std::invalid_argument& e) {
            std::cerr << "Interval should be specified in seconds, number format!" <<std::endl;
            return static_cast<int>(ErrorCode::WRONG_INTERVAL_FORMAT);
        }

    }else{
        std::cerr << "Interval Not specified!";
        return static_cast<int>(ErrorCode::INTERVAL_NOT_SPECIFIED);
    }

    // start the time initially
    auto now = std::chrono::system_clock::now();
    std::time_t time = std::chrono::system_clock::to_time_t(now);

    std::string time_string = std::ctime(&time);
    time_string.erase(time_string.length() - 1);

    // start synchronization
    while (true) {
        logfile << "[" << time_string << "] " << "Synchronizing" << std::endl;
        std::cout << "Synchronizing" << std::endl;
        synchronize_folders(source_folder, replica_folder, logging_file);
        if (interval > 0) {
            std::this_thread::sleep_for(std::chrono::seconds(interval));
        }

    }

    return static_cast<int>(ErrorCode::SUCCESS);
}
