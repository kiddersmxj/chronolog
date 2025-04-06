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

const std::string ConfigFilePath = "/home/kidders/devel/chronolog/config.conf";
// Function to initialize global configuration variables
int InitConfig();

const std::string ProgramName = "chronolog";
const std::string Version = "0.0.0";
const std::string UsageNotes = R"(usage: chronolog [ -h/-v ]
options:
    -h / --help         show help and usage notes
    -v / --version      print version and exit)";

void Usage();
void Usage(std::string Message);
void PrintVersion();

#endif

// Copyright (c) 2024, Maxamilian Kidd-May
// All rights reserved.

// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree. 

