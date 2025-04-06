#ifndef CHRONO_TIMER_H
#define CHRONO_TIMER_H

#include <chrono>
#include <string>
#include <unordered_map>

class TimerManager {
    struct TimerState {
        std::chrono::steady_clock::time_point start_time;
        double total_elapsed = 0;
        bool is_running = false;
    };

    std::unordered_map<std::string, TimerState> timers;
    
public:
    void start(const std::string& name);
    void stop(const std::string& name);
    void reset(const std::string& name);
    double get_elapsed(const std::string& name) const;
};

#endif
