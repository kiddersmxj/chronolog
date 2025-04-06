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

std::optional<std::chrono::system_clock::time_point> TimerLogger::get_last_start_time(const std::string& name) {
    std::string path = get_file_path(name);
    std::ifstream file(path);
    if (!file.is_open()) {
        return std::nullopt;
    }

    std::string line;
    std::optional<std::chrono::system_clock::time_point> last_time;
    
    // Skip header
    std::getline(file, line);

    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string token;
        std::vector<std::string> parts;
        
        while (std::getline(iss, token, ',')) {
            // Remove quotes and trim whitespace
            if (!token.empty() && token.front() == '"') token.erase(0, 1);
            if (!token.empty() && token.back() == '"') token.pop_back();
            parts.push_back(token);
        }

        if (parts.size() >= 2 && (parts[0] == "START" || parts[0] == "RESET")) {
            std::tm tm = {};
            std::istringstream ss(parts[1]);
            ss >> std::get_time(&tm, "%Y-%m-%dT%H:%M:%S");
            
            if (!ss.fail()) {
                auto time_point = std::chrono::system_clock::from_time_t(std::mktime(&tm));
                if (!last_time || time_point > *last_time) {
                    last_time = time_point;
                }
            }
        }
    }
    
    return last_time;
}

// Copyright (c) 2024, Maxamilian Kidd-May
// All rights reserved.

// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree. 

