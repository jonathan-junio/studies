//
// Created by Jonathan Matos Nunes on 2/6/26.
//

#pragma once
#include <iostream>
#include <mutex>
#include <string_view>

enum class LogLevel {
    DEBUG = 0,
    INFO  = 1,
    WARN  = 2,
    ERROR = 3,
    OFF   = 4
};

class Logger {
public:
    inline static LogLevel s_minLevel = LogLevel::DEBUG;

    static void setLevel(LogLevel level) {
        s_minLevel = level;
    }
    static std::mutex& getMutex() {
        static std::mutex m_mutex;
        return m_mutex;
    }

    static void log(LogLevel level, const char* file, int line, std::string_view message) {
        if (level < s_minLevel) return;
        std::lock_guard<std::mutex> lock(getMutex());

        const char* levelStr = "UNKNOWN";
        const char* colorCode = "\033[0m";

        switch (level) {
            case LogLevel::DEBUG: levelStr = "DEBUG"; colorCode = "\033[36m"; break;
            case LogLevel::INFO:  levelStr = "INFO "; colorCode = "\033[32m"; break;
            case LogLevel::WARN:  levelStr = "WARN "; colorCode = "\033[33m"; break;
            case LogLevel::ERROR: levelStr = "ERROR"; colorCode = "\033[31m"; break;
            default: break;
        }

        std::string_view filename = file;
        size_t lastSlash = filename.find_last_of("/\\");
        if (lastSlash != std::string_view::npos) {
            filename = filename.substr(lastSlash + 1);
        }
        std::cout << colorCode << "[" << levelStr << "] \033[0m"
                  << filename << ":" << line << " > "
                  << message << std::endl;
    }
};
#ifndef NDEBUG
#define LOG_DEBUG(msg) Logger::log(LogLevel::DEBUG, __FILE__, __LINE__, msg)
#else
#define LOG_DEBUG(msg) void(0)
#endif

#define LOG_INFO(msg)  Logger::log(LogLevel::INFO,  __FILE__, __LINE__, msg)
#define LOG_WARN(msg)  Logger::log(LogLevel::WARN,  __FILE__, __LINE__, msg)
#define LOG_ERROR(msg) Logger::log(LogLevel::ERROR, __FILE__, __LINE__, msg)