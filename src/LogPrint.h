#pragma once

#include <iostream>
#include <string>

// 定义日志级别
enum LogLevel {
    LOG_DEBUG,
    LOG_INFO,
    LOG_WARNING,
    LOG_ERROR
};

// 将日志级别转换为字符串

inline const char* logLevelToString(LogLevel level) {
    switch (level) {
        case LOG_DEBUG: return "DEBUG";
        case LOG_INFO: return "INFO";
        case LOG_WARNING: return "WARNING";
        case LOG_ERROR: return "ERROR";
        default: return "UNKNOWN";
    }
}

// 定义日志打印宏
#define LOG(level, message) \
    do { \
        if (level >= LOG_DEBUG) { \
            std::cerr << "[" << logLevelToString(level) << "] " \
                      << __FUNCTION__ << " at line: " << __LINE__ << ": " \
                      << message << std::endl; \
        } \
    } while (0)


// 定义错误打印宏
#define PRINT_ERROR(errorCode, errorMsg) \
    do { \
        std::cerr << "Error in function: " << __FUNCTION__ << " at line: " << __LINE__ \
                  << " with error code: " << (errorCode) << ". " \
                  << "Error message: " << (errorMsg) << std::endl; \
    } while (0)
    
