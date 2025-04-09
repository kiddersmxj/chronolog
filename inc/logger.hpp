#ifndef CHRONO_LOGGER_H
#define CHRONO_LOGGER_H

#include "../inc/config.hpp"
#include "../inc/timer.hpp"

#include <fstream>
#include <string>
#include <chrono>
#include <filesystem>
#include <iomanip>
#include <ctime>
#include <optional>
#include <regex>
#include <stdexcept>
#include <iostream>

class TimerLogger {
    private:
        std::string trimQuotes(const std::string &str);
        std::filesystem::path log_dir;
        bool headers_written = false;
        
        void write_header(const std::string& path);
        
    public:
        TimerLogger() : log_dir(LogPath) {
            std::filesystem::create_directories(log_dir);
        }

        std::string get_dir_path(const std::string& name);
        std::string get_file_path(const std::string& name, bool increment = false);
        int get_current_log(const std::string &directory_path);
        void log_reset(TimerManager::TimerState timer);
        double read_prev_elapsed(const std::string &name);
        std::string get_last_event_type(const std::string &name);
        void log_event(TimerManager::TimerState timer);
        std::time_t get_last_start_time(const std::string& name);
};

#endif
