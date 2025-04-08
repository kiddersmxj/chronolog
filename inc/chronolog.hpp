#ifndef K_CHRONOLOG
#define K_CHRONOLOG

#include "config.hpp"
#include "timer.hpp"
#include "logger.hpp"
#include <getopt.h>
#include <string>
#include <iomanip>

std::string format_duration(double total_seconds);
double parse_duration(const std::string& duration_str);

#endif

// Copyright (c) 2024, Maxamilian Kidd-May
// All rights reserved.

// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree. 

