#include "../inc/chronolog.hpp"

std::string format_duration(double total_seconds) {
    int hours = static_cast<int>(total_seconds) / 3600;
    int minutes = (static_cast<int>(total_seconds) % 3600) / 60;
    int seconds = static_cast<int>(total_seconds) % 60;
    
    std::stringstream ss;
    ss << std::setw(2) << std::setfill('0') << hours << ":"
       << std::setw(2) << std::setfill('0') << minutes << ":" 
       << std::setw(2) << std::setfill('0') << seconds;
    
    return ss.str();
}

double parse_duration(const std::string& duration_str) {
    std::vector<std::string> parts;
    std::stringstream ss(duration_str);
    std::string part;
    
    // Split string by colons
    while (std::getline(ss, part, ':')) {
        parts.push_back(part);
    }
    
    // Reverse so we have seconds first
    std::reverse(parts.begin(), parts.end());
    
    double total = 0.0;
    double multiplier = 1.0; // seconds
    
    for (const auto& p : parts) {
        try {
            total += std::stod(p) * multiplier;
        } catch (const std::invalid_argument&) {
            return 0.0; // Invalid format
        }
        multiplier *= 60; // move to next unit (sec->min->hour)
    }
    
    // If we had more than 3 parts, it's invalid
    if (parts.size() > 3) {
        return 0.0;
    }
    
    return total;
}

int main(int argc, char** argv) {
    InitConfig();

    int opt;
    int HelpFlag = 0;
    int VersionFlag = 0;
    int StartFlag = 0;
    int StopFlag = 0;
    int ResetFlag = 0;
    int PlainFlag = 0;
    int SecondsFlag = 0;
    std::string Name = "";
    std::string Add = "";

    // Get opt option defenitions
    struct option Opts[] = {
        { "help", no_argument, &HelpFlag, 1 },
        { "version", no_argument, &VersionFlag, 1 },
        { "name", required_argument, NULL, 'n' },
        { "start", no_argument, NULL, 't' },
        { "stop", no_argument, NULL, 'o' },
        { "reset", no_argument, NULL, 'r' },
        { "add", required_argument, NULL, 'a' },
        { "plain", no_argument, NULL, 'p' },
        { "seconds", no_argument, NULL, 's' },
        { 0 }
    };

    // Infinite loop, to be broken when we are done parsing options
    while (1) {
        opt = getopt_long(argc, argv, "hvcsoan:rpt", Opts, 0);

        // A return value of -1 indicates that there are no more options
        if (opt == -1) {
            if(HelpFlag && VersionFlag) {
                Usage();;
                return EXIT_FAILURE;
            }
            break;
        } 

        switch (opt) {
        case 'h':
            HelpFlag = 1;
            break;
        case 'v':
            VersionFlag = 1;
            break;
        case 'n':
            Name = optarg;
            break;
        case 't':
            StartFlag = 1;
            break;
        case 'o':
            StopFlag = 1;
            break;
        case 'r':
            ResetFlag = 1;
            break;
        case 'a':
            Add = optarg;
            break;
        case 'p':
            PlainFlag = 1;
            break;
        case 's':
            SecondsFlag = 1;
            break;
        case '?':
            Usage();
            return EXIT_FAILURE;
        default:
            Usage();
            break;
        }
    }

    if((StartFlag + StopFlag + ResetFlag + !Add.empty()) > 1) {
        std::cout << "Error: cannot use more then one of flags --start, --stop, --reset, or --add at the same time\n\n";
        Usage();
        return 1;
    }

    if(!Name.empty()) {
        TimerManager timer;
        TimerLogger logger;

        std::time_t last_start = 0;
        double last_elapsed = 0.0;
        std::string last_event_type = "";

        // Check if log file exists before trying to read from it
        if(std::filesystem::exists(logger.get_file_path(Name))) {
            last_start = logger.get_last_start_time(Name);
            last_elapsed = logger.read_prev_elapsed(Name);
            last_event_type = logger.get_last_event_type(Name);
        }
        
        if(StartFlag) {
            if(last_event_type == "START") {
                std::cout << "Error: Timer already started\n";
                return 1;
            }
            logger.log_event(timer.start(Name, last_elapsed));
        }
        else if(StopFlag) {
            if(last_event_type != "START") {
                std::cout << "Error: Timer not started or does not exist\n";
                return 1;
            }
            logger.log_event(timer.stop(Name, last_start, last_elapsed));
        }
        else if(ResetFlag) {
            logger.log_reset(timer.reset(Name));

        } else if(!Add.empty()) {
            double add_double = 0.0;
            if(!SecondsFlag) {
                add_double = parse_duration(Add);
            } else {
                try {
                    // Convert the string to a double using std::stod.
                    add_double = std::stod(Add);
                } catch (const std::invalid_argument& e) {
                    std::cerr << "Conversion of add value failed: The provided string '" << Add
                              << "' does not contain a valid number. Error: " << e.what() << std::endl;
                } catch (const std::out_of_range& e) {
                    std::cerr << "Conversion of add value failed: The number in the string '" << Add
                              << "' is out of the range representable by a double. Error: " << e.what() << std::endl;
                }
            }
            logger.log_event(timer.add(Name, last_elapsed, add_double));
        } else {
            // Handle no timer operations to just print
            double elapsed = 0.0;
            if(last_event_type == "START") {
                elapsed = timer.stop(Name, last_start, last_elapsed).total_elapsed;
            } else {
                elapsed = last_elapsed;
            }

            std::string elapsed_string = "";

            if(SecondsFlag) {
                std::ostringstream oss;
                oss << std::fixed << std::setprecision(0) << elapsed;
                elapsed_string = oss.str();
            } else {
                elapsed_string = format_duration(elapsed);
            }

            if(PlainFlag) {
                std::cout << elapsed_string << std::endl;
            } else {
                std::cout << "Total elapsed time for " << Name << ": " << elapsed_string << std::endl;
            }
        }
        
        return 0;
    }

    // Act on flags
    if(HelpFlag) {
        Usage();
        return EXIT_SUCCESS;
    }
    if(VersionFlag) {
        PrintVersion();
        return EXIT_SUCCESS;
    }

    return 0;
}

// Copyright (c) 2024, Maxamilian Kidd-May
// All rights reserved.

// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree. 

