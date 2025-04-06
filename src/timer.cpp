#include "../inc/timer.hpp"

void TimerManager::start(const std::string& name) {
    auto& timer = timers[name];
    if (!timer.is_running) {
        timer.start_time = std::chrono::steady_clock::now();
        timer.is_running = true;
    }
}

void TimerManager::stop(const std::string& name) {
    auto it = timers.find(name);
    if (it != timers.end() && it->second.is_running) {
        auto& timer = it->second;
        auto end = std::chrono::steady_clock::now();
        timer.total_elapsed += std::chrono::duration<double>(end - timer.start_time).count();
        timer.is_running = false;
    }
}

void TimerManager::reset(const std::string& name) {
    timers.erase(name);
}

double TimerManager::get_elapsed(const std::string& name) const {
    auto it = timers.find(name);
    if (it != timers.end()) {
        return it->second.total_elapsed;
    }
    return 0.0;
}

// Copyright (c) 2024, Maxamilian Kidd-May
// All rights reserved.

// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree. 

