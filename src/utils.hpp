#pragma once

#include <chrono>
#include <iomanip>
#include <sstream>
#include <iostream>

std::string unix_time_to_string(uint64_t timestamp, const char* format = "%c") {
    auto timePoint = std::chrono::system_clock::time_point{std::chrono::seconds{timestamp}};
    std::time_t time = std::chrono::system_clock::to_time_t(timePoint);
    std::stringstream ss;
    ss << std::put_time(std::localtime(&time), format);
    return ss.str();
}