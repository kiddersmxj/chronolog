#include "../inc/logger.hpp"

namespace fs = std::filesystem;

std::string TimerLogger::get_file_path(const std::string& name, bool increment) {
    std::string filename = LogFormat;
    size_t pos = filename.find("{name}");

    int current_number = get_current_log(get_dir_path(name));

    current_number = (increment == true) ? current_number + 1 : current_number;

    std::string filenumber = "/" + std::to_string(current_number);
    std::cout << filenumber << std::endl;
    if(pos != std::string::npos) {
        filename.replace(pos, 6, name + filenumber);
    }
    return (log_dir / filename).string();
}

std::string TimerLogger::get_dir_path(const std::string& name) {
    std::string filename = LogFormat;
    size_t pos = filename.find("{name}");
    if(pos != std::string::npos) {
        filename.replace(pos, 10, name);
    }
    return (log_dir / filename).string();
}

void TimerLogger::write_header(const std::string& path) {
    if(!headers_written && !std::filesystem::exists(path)) {
        // Create parent directories if they don't exist
        std::filesystem::create_directories(fs::path(path).parent_path());
        std::ofstream file(path);
        if(file.is_open()) {
            file << CSVHeaders << "\n";
            headers_written = true;
        }
    }
}

// Function that appends a new event row to a CSV file.
// The CSV row format is: event_type,timestamp,elapsed_seconds,name
// For this example, event_type is "START", elapsed_seconds is 0.0, and name is "t8".
void TimerLogger::log_event(TimerManager::TimerState timer) {
    std::string filename = get_file_path(timer.name);
    write_header(filename);
    std::cout << filename << std::endl;

    // Obtain the current system time.
    auto now = std::chrono::system_clock::now();
    std::time_t now_time = std::chrono::system_clock::to_time_t(now);

    // Convert time_t into a tm structure representing local time.
    std::tm local_tm;
#ifdef _WIN32
    localtime_s(&local_tm, &now_time);
#else
    local_tm = *std::localtime(&now_time);
#endif

    // Format the time into an ISO‑8601 style string using 'T' to separate date and time.
    // For example: "2025-04-08T00:25:20"
    char buffer[100];
    std::strftime(buffer, sizeof(buffer), "%Y-%m-%dT%H:%M:%S", &local_tm);

    // Open the CSV file in append mode.
    std::ofstream file(filename, std::ios::app);
    if (file.is_open()) {
        file << "\"" << timer.event_type << "\","
             << "\"" << buffer    << "\","
             << "\"" << timer.total_elapsed << "\","
             << "\"" << timer.name << "\"\n";
        file.close();
        std::cout << "Event written to CSV file: " << filename << std::endl;
    } else {
        std::cerr << "Error: Cannot open file for writing: " << filename << std::endl;
    }
}

// Function: get_current_log
// Description: Traverses the provided directory, inspects each filename to determine 
//              if it is of the form "number.csv", and returns the greatest number found.
// Parameters:
//   - directory_path: The path to the directory as a std::string.
// Return Value: The highest number extracted from a filename matching the "number.csv" pattern.
int TimerLogger::get_current_log(const std::string &directory_path) {
    int max_log = 0;
    // Create directory if it doesn't exist
    if (!fs::exists(directory_path)) {
        fs::create_directories(directory_path);
        return 0; // No existing logs in new directory
    }
    
    // Regular expression to match a filename that starts with one or more digits and ends with ".csv".
    std::regex pattern(R"((\d+)\.csv$)");
    
    // Iterate over entries in the directory.
    for (const auto& entry : fs::directory_iterator(directory_path)) {
        // Ensure the entry is a regular file.
        if (entry.is_regular_file()) {
            std::string filename = entry.path().filename().string();
            std::smatch match_result;
            
            // If the file matches the pattern...
            if (std::regex_match(filename, match_result, pattern)) {
                try {
                    // Convert the captured group (the number) to an integer.
                    int number = std::stoi(match_result[1].str());
                    if (number > max_log) {
                        max_log = number;
                    }
                } catch (const std::exception &ex) {
                    // If conversion fails, we may log the error or simply continue.
                    std::cerr << "Conversion error for file " << filename << ": " << ex.what() << std::endl;
                }
            }
        }
    }
    return max_log;
}

void TimerLogger::log_reset(TimerManager::TimerState timer) {
    std::string newpath = get_file_path(timer.name, true);
    write_header(newpath);
}
// Utility function to remove surrounding quotes from a string.
std::string TimerLogger::trimQuotes(const std::string &str) {
    if (str.size() >= 2 && str.front() == '\"' && str.back() == '\"') {
        return str.substr(1, str.size() - 2);
    }
    return str;
}

// Function that reads the last (bottom-most) row from a CSV file,
// extracts the timestamp from the second column, converts it to a time_t,
// and returns that value.
std::time_t TimerLogger::get_last_start_time(const std::string& name) {
    std::string filename = get_file_path(name);
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Cannot open file for reading: " << filename << std::endl;
        return -1;
    }
    
    std::string line;
    std::string lastLine;
    bool headerSkipped = false;
    // Iterate through each line in the CSV.
    while (std::getline(file, line)) {
        if (line.empty()) {
            continue;
        }
        // Check if the first (header) line contains "event_type"; skip it.
        if (!headerSkipped) {
            if (line.find("event_type") != std::string::npos) {
                headerSkipped = true;
                continue;
            }
            headerSkipped = true; // if no header, we still treat the first line as data.
        }
        lastLine = line;
    }
    file.close();

    if (lastLine.empty()) {
        std::cerr << "Error: No data found in CSV file." << std::endl;
        return -1;
    }

    // The CSV columns are: event_type,timestamp,elapsed_seconds,name.
    // Use a stringstream to tokenize the line by commas.
    std::stringstream ss(lastLine);
    std::string token;
    int col = 0;
    std::string timestampStr;
    while (std::getline(ss, token, ',')) {
        if (col == 1) { // The timestamp is in the second column.
            timestampStr = trimQuotes(token);
            break;
        }
        col++;
    }
    
    if (timestampStr.empty()) {
        std::cerr << "Error: Could not find a timestamp in the last row." << std::endl;
        return -1;
    }

    // Replace any 'T' in the timestamp with a space for easier parsing.
    for (auto &ch : timestampStr) {
        if (ch == 'T') {
            ch = ' ';
        }
    }

    // Expected format after replacement: "YYYY-MM-DD HH:MM:SS"
    int year, month, day, hour, minute, second;
    if (sscanf(timestampStr.c_str(), "%d-%d-%d %d:%d:%d", 
               &year, &month, &day, &hour, &minute, &second) != 6) {
        std::cerr << "Error: Could not parse timestamp: " << timestampStr << std::endl;
        return -1;
    }

    // Populate a tm structure with the parsed values.
    std::tm tm_struct = {};
    tm_struct.tm_year = year - 1900; // Years since 1900.
    tm_struct.tm_mon  = month - 1;   // Months are 0-based.
    tm_struct.tm_mday = day;
    tm_struct.tm_hour = hour;
    tm_struct.tm_min  = minute;
    tm_struct.tm_sec  = second;
    tm_struct.tm_isdst = -1;         // Let the system determine DST.

    // Convert the tm structure to time_t.
    std::time_t parsed_time = std::mktime(&tm_struct);
    if (parsed_time == -1) {
        std::cerr << "Error: mktime() failed while converting parsed time." << std::endl;
    }
    return parsed_time;
}

// Function that retrieves the bottom-most value in the elapsed_seconds column from a CSV file.
// Returns the value as a double. The CSV is assumed to have the columns: 
// event_type, timestamp, elapsed_seconds, name.
double TimerLogger::read_prev_elapsed(const std::string &name) {
    std::string filename = get_file_path(name);

    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Cannot open CSV file: " << filename << std::endl;
        return 0.0;
    }
    
    std::string line;
    std::string lastLine;
    bool headerSkipped = false;

    // Iterate through the file lines. If a header is present, skip it.
    while (std::getline(file, line)) {
        if (line.empty())
            continue;
        
        // Check if the header line is present (contains "event_type").
        if (!headerSkipped) {
            if (line.find("event_type") != std::string::npos) {
                headerSkipped = true;
                continue;
            }
            headerSkipped = true; // Assume first line is data if no header.
        }
        lastLine = line;
    }
    
    file.close();
    
    if (lastLine.empty()) {
        std::cerr << "Error: No data found in CSV file." << std::endl;
        return 0.0;
    }
    
    // Tokenize the last line by comma to extract the columns.
    std::stringstream ss(lastLine);
    std::string token;
    int colIndex = 0;
    std::string elapsedStr;
    while (std::getline(ss, token, ',')) {
        if (colIndex == 2) { // The elapsed_seconds value is in the third column (0-based index 2)
            elapsedStr = trimQuotes(token);
            break;
        }
        colIndex++;
    }
    
    if (elapsedStr.empty()) {
        std::cerr << "Error: Could not find the elapsed_seconds column in the last row." << std::endl;
        return 0.0;
    }
    
    // Convert the elapsed string to a double.
    double elapsedValue = 0.0;
    try {
        elapsedValue = std::stod(elapsedStr);
    } catch(const std::exception &e) {
        std::cerr << "Error: Could not convert elapsed_seconds to a double: " << elapsedStr << "\nException: " << e.what() << std::endl;
        return 0.0;
    }
    
    return elapsedValue;
}

std::string TimerLogger::get_last_event_type(const std::string &name) {
    std::string filename = get_file_path(name);

    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Cannot open CSV file: " << filename << std::endl;
        return "";
    }
    
    std::string line;
    std::string lastLine;
    bool headerSkipped = false;

    // Iterate through the file lines. If a header is present, skip it.
    while (std::getline(file, line)) {
        if (line.empty())
            continue;
        
        // Check if the header line is present (contains "event_type").
        if (!headerSkipped) {
            if (line.find("event_type") != std::string::npos) {
                headerSkipped = true;
                continue;
            }
            headerSkipped = true; // Assume first line is data if no header.
        }
        lastLine = line;
    }
    
    file.close();
    
    if (lastLine.empty()) {
        std::cerr << "Error: No data found in CSV file." << std::endl;
        return "";
    }
    
    // Tokenize the last line by comma to extract the columns.
    std::stringstream ss(lastLine);
    std::string token;
    std::string eventType;
    if (std::getline(ss, token, ',')) {
        eventType = trimQuotes(token);
    }
    
    if (eventType.empty()) {
        std::cerr << "Error: Could not find the event_type column in the last row." << std::endl;
        return "";
    }
    
    return eventType;
}

// Copyright (c) 2024, Maxamilian Kidd-May
// All rights reserved.

// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree. 

