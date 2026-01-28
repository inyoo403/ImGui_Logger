#pragma once

#include "imgui/imgui.h"
#include <fstream>
#include <string>
#include <vector>

enum class LogLevel {
    INFO,
    WARNING,
    ERROR,
    GAME
};

struct LogEntry {
    std::string timestamp;
    std::string message;
    LogLevel level;
};

class Logger
{
public:
    static Logger& GetInstance();

    void Log(LogLevel level, const char* fmt, ...);
    
    void LogInfo(const char* fmt, ...);
    void LogWarning(const char* fmt, ...);
    void LogError(const char* fmt, ...);
    void LogGameEvent(const char* fmt, ...);

    void Draw(const char* title, bool* p_open = nullptr);

private:
    Logger();
    ~Logger();
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;

    std::string GetTimestamp();

    std::vector<LogEntry> Items;
    ImGuiTextFilter     Filter;
    bool                AutoScroll;
    std::ofstream       FileStream;
    
    char InputBuf[256];
};
