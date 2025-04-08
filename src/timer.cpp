#include "../inc/timer.hpp"

TimerManager::TimerState TimerManager::start(const std::string& name, double last_elapsed) {
    TimerState timer;
    timer.name = name;
    timer.start_time = std::chrono::system_clock::now();
    timer.event_type = "START";
    timer.total_elapsed = last_elapsed;
    return timer;
}

TimerManager::TimerState TimerManager::stop(const std::string& name, std::time_t last_start, double last_elapsed) {
    double elapsed = get_elapsed(last_start);

    TimerState timer;
    timer.name = name;
    timer.event_type = "STOP";
    timer.total_elapsed = elapsed + last_elapsed;

    return timer;
}

TimerManager::TimerState TimerManager::reset(const std::string& name) {
    TimerState timer;

    timer.name = name;
    timer.event_type = "RESET";
    timer.total_elapsed = 0.0;
    timer.start_time = std::chrono::system_clock::now();

    return timer;
}

// Function that calculates the elapsed time, in seconds, between the provided stored time and the current time.
double TimerManager::get_elapsed(std::time_t stored_time) {
    std::time_t current_time = std::time(nullptr);
    double elapsed = std::difftime(current_time, stored_time);
        // std::cout << "Elapsed time between the stored timestamp and current time: "
        //           << elapsed << " seconds." << std::endl;
    return elapsed;
}

// Copyright (c) 2024, Maxamilian Kidd-May
// All rights reserved.

// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree. 

