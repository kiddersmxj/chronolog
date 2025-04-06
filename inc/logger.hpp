#ifndef CHRONO_LOGGER_H
#define CHRONO_LOGGER_H

#include <fstream>
#include <string>
#include <chrono>
#include <filesystem>
#include <iomanip>
#include <ctime>
#include <optional>
#include "../inc/config.hpp"
#include "../inc/timer.hpp"

class TimerLogger {
    std::filesystem::path log_dir;
    bool headers_written = false;
    
    std::string get_file_path(const std::string& name);
    void write_header(const std::string& path);
    // Get last start/reset time from log file
    std::optional<std::chrono::system_clock::time_point> get_last_start_time(const std::string& filename);
    
public:
    TimerLogger() : log_dir(LogPath) {
        std::filesystem::create_directories(log_dir);
    }
    
    void log_event(const std::string& name, const std::string& event_type);
};

#endif
