#ifndef CHRONO_TIMER_H
#define CHRONO_TIMER_H

#include <chrono>
#include <string>
#include <unordered_map>
#include <iostream>

class TimerManager {
    public:
        struct TimerState {
            std::string name = "";
            std::chrono::system_clock::time_point start_time;
            std::string event_type;
            std::chrono::system_clock::time_point last_start;
            double total_elapsed = 0;
        };

        TimerManager::TimerState start(const std::string& name, double last_elapsed);
        TimerManager::TimerState stop(const std::string& name, std::time_t last_start, double last_elapsed);
        TimerManager::TimerState reset(const std::string& name);
        TimerManager::TimerState add(const std::string& name, double last_elapsed, double elapsed_addition);

    private:
        std::tm get_time();
        double get_elapsed(std::time_t stored_time);
};

#endif
