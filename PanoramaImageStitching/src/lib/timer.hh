#pragma once

#include <chrono>
#include <functional>
#include <map>
#include <iostream>
#include <string>

class Timer {
  public:
    using Clock = std::chrono::high_resolution_clock;
    Timer() {
        restart();
    }

    void restart() {
        m_start_time = std::chrono::high_resolution_clock::now();
    }

    double duration() const {
        auto now = std::chrono::high_resolution_clock::now();
        auto m = std::chrono::duration_cast<std::chrono::microseconds>(now-m_start_time).count();
        return m * 1.0/1e6;
    }
  protected:
    std::chrono::time_point<Clock> m_start_time;
};

class TotalTimer {
  public:

    TotalTimer(const std::string& msg):msg(msg) {
        timer.restart();
    }

    ~TotalTimer();

    static void print();

    std::string msg;
    Timer timer;

    static std::map<std::string, std::pair<int, double>> rst;
};
