#include "../inc/logger.hpp"

std::string TimerLogger::get_file_path(const std::string& name) {
    std::string filename = LogFormat;
    size_t pos = filename.find("{name}");
    if(pos != std::string::npos) {
        filename.replace(pos, 6, name);
    }
    return (log_dir / filename).string();
}

void TimerLogger::write_header(const std::string& path) {
    if(!headers_written && !std::filesystem::exists(path)) {
        std::ofstream file(path);
        if(file.is_open()) {
            file << CSVHeaders << "\n";
            headers_written = true;
        }
    }
}

void TimerLogger::log_event(const std::string& name, const std::string& event_type) {
    auto now = std::chrono::system_clock::now();
    auto timestamp = std::chrono::system_clock::to_time_t(now);
    std::string path = get_file_path(name);
    write_header(path);
    
    std::ofstream file(path, std::ios::app);
    if(file.is_open()) {
        file << "\"" << event_type << "\","
             << "\"" << std::put_time(std::localtime(&timestamp), "%Y-%m-%dT%H:%M:%S") << "\","
             << "\"" << TimerManager().get_elapsed(name) << "\","
             << "\"" << name << "\"\n";
    }
}

// Copyright (c) 2024, Maxamilian Kidd-May
// All rights reserved.

// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree. 

