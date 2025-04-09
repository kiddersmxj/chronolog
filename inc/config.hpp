#ifndef K_CONFIG_CHRONOLOG
#define K_CONFIG_CHRONOLOG

#include <iostream>
#include <std-k.hpp>

// Define configuration variables with or without default values using inline
inline std::vector<std::string> ExampleArray = {};
inline std::string ExampleString;
inline bool ExampleBool;
inline int ExampleInt;

inline std::string LogPath;
inline std::string LogFormat;
inline std::string CSVHeaders;

const std::string ConfigFilePath = "/home/kidders/.chronolog/chronolog.conf";
// Function to initialize global configuration variables
int InitConfig();

const std::string ProgramName = "chronolog";
const std::string Version = "0.1.0";
const std::string UsageNotes = R"(usage: chronolog [ -h/-v ] [ -n name ] 
[ --start/--stop/--reset ]/[ --add hh:mm:ss/seconds ]
[ -p ] [ -s ]

options:
    -h / --help         show help and usage notes
    -v / --version      print version and exit
    -n / --name         assign name, will create new log if new name and print elapsed time if no timer operation passed
         --start        starts the named timer
         --stop         stops the named timer
    -r / --reset        resets the timer but keeps old logs
    -a / --add          add time to the timer (input type chages with -s flag)
    -t / --toggle       toggles timer on/off
    -c / --create       used to create a new log (must be used with a name and --start flag)
    -p / --plain        print only output to terminal (for command use)
    -s / --seconds      use seconds for input/output instead of hh:mm:ss format)";

void Usage();
void Usage(std::string Message);
void PrintVersion();

#endif

// Copyright (c) 2024, Maxamilian Kidd-May
// All rights reserved.

// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree. 

