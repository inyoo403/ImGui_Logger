#include "Logger.h"
#include <iostream>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <cstdarg>

Logger& Logger::GetInstance()
{
    static Logger instance;
    return instance;
}

Logger::Logger()
{
    AutoScroll = true;
    memset(InputBuf, 0, sizeof(InputBuf));
    FileStream.open("log.txt", std::ios::out | std::ios::app);
}

Logger::~Logger()
{
    if (FileStream.is_open())
        FileStream.close();
}

std::string Logger::GetTimestamp()
{
    // Get current time
    auto now = std::chrono::system_clock::now();
    auto now_time_t = std::chrono::system_clock::to_time_t(now);
    auto now_ms = std::chrono::duration_cast<std::chrono::milliseconds>(
        now.time_since_epoch()) % 1000;

    std::tm time_info;
    localtime_s(&time_info, &now_time_t);

    std::stringstream ss;
    ss << "[" << std::put_time(&time_info, "%H:%M:%S")
       << "." << std::setfill('0') << std::setw(3) << now_ms.count() << "]";
    return ss.str();
}

void Logger::Log(LogLevel level, const char* fmt, ...)
{
    char buf[1024];
    va_list args;
    va_start(args, fmt);
    vsnprintf(buf, sizeof(buf), fmt, args);
    va_end(args);

    LogEntry entry;
    entry.timestamp = GetTimestamp();
    entry.message = buf;
    entry.level = level;

    Items.push_back(entry);

    // Save to file
    if (FileStream.is_open())
    {
        const char* levelStr = "";
        switch (level) {
            case LogLevel::INFO: levelStr = "[INFO] "; break;
            case LogLevel::WARNING: levelStr = "[WARN] "; break;
            case LogLevel::ERROR: levelStr = "[ERROR] "; break;
            case LogLevel::GAME: levelStr = "[GAME] "; break;
        }
        FileStream << entry.timestamp << " " << levelStr << entry.message << "\n";
        FileStream.flush();
    }
}

void Logger::LogInfo(const char* fmt, ...)
{
    char buf[1024];
    va_list args;
    va_start(args, fmt);
    vsnprintf(buf, sizeof(buf), fmt, args);
    va_end(args);
    Log(LogLevel::INFO, "%s", buf);
}

void Logger::LogWarning(const char* fmt, ...)
{
    char buf[1024];
    va_list args;
    va_start(args, fmt);
    vsnprintf(buf, sizeof(buf), fmt, args);
    va_end(args);
    Log(LogLevel::WARNING, "%s", buf);
}

void Logger::LogError(const char* fmt, ...)
{
    char buf[1024];
    va_list args;
    va_start(args, fmt);
    vsnprintf(buf, sizeof(buf), fmt, args);
    va_end(args);
    Log(LogLevel::ERROR, "%s", buf);
}

void Logger::LogGameEvent(const char* fmt, ...)
{
    char buf[1024];
    va_list args;
    va_start(args, fmt);
    vsnprintf(buf, sizeof(buf), fmt, args);
    va_end(args);
    Log(LogLevel::GAME, "%s", buf);
}

void Logger::Draw(const char* title, bool* p_open)
{
    ImGui::SetNextWindowSize(ImVec2(600, 400), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowPos(ImVec2(50, 50), ImGuiCond_FirstUseEver);
    if (!ImGui::Begin(title, p_open))
    {
        ImGui::End();
        return;
    }

    // Toolbar
    if (ImGui::Button("Options")) ImGui::OpenPopup("Options");
    ImGui::SameLine();
    if (ImGui::Button("Clear")) Items.clear();
    ImGui::SameLine();
    
    if (ImGui::Button("Test Info")) LogInfo("This is a test info message");
    ImGui::SameLine();
    if (ImGui::Button("Test Warning")) LogWarning("This is a test warning message");
    ImGui::SameLine();
    if (ImGui::Button("Test Error")) LogError("This is a test error message");

    ImGui::SameLine();
    Filter.Draw("Filter", -100.0f);

    ImGui::Separator();

    float footer_height_to_reserve = ImGui::GetStyle().ItemSpacing.y + ImGui::GetFrameHeightWithSpacing(); 
    ImGui::BeginChild("ScrollingRegion", ImVec2(0, -footer_height_to_reserve), false, ImGuiWindowFlags_HorizontalScrollbar);

    if (ImGui::BeginPopup("Options"))
    {
        ImGui::Checkbox("Auto-scroll", &AutoScroll);
        ImGui::EndPopup();
    }

    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 1));

    for (const auto& item : Items)
    {
        if (!Filter.PassFilter(item.message.c_str()))
            continue;

        ImVec4 color;
        bool has_color = false;
        
        switch (item.level)
        {
        case LogLevel::INFO: 
            color = ImVec4(1.0f, 1.0f, 1.0f, 1.0f); 
            break;
        case LogLevel::WARNING: 
            color = ImVec4(1.0f, 1.0f, 0.0f, 1.0f);
            has_color = true; 
            break;
        case LogLevel::ERROR: 
            color = ImVec4(1.0f, 0.4f, 0.4f, 1.0f);
            has_color = true; 
            break;
        case LogLevel::GAME: 
            color = ImVec4(0.0f, 1.0f, 1.0f, 1.0f);
            has_color = true; 
            break;
        }

        if (has_color) ImGui::PushStyleColor(ImGuiCol_Text, color);
        
        ImGui::TextUnformatted(item.timestamp.c_str());
        ImGui::SameLine();
        
        switch (item.level)
        {
        case LogLevel::INFO:    ImGui::Text("[INFO]"); break;
        case LogLevel::WARNING: ImGui::Text("[WARN]"); break;
        case LogLevel::ERROR:   ImGui::Text("[ERROR]"); break;
        case LogLevel::GAME:    ImGui::Text("[GAME]"); break;
        }
        ImGui::SameLine();
        ImGui::TextUnformatted(item.message.c_str());

        if (has_color) ImGui::PopStyleColor();
    }

    if (AutoScroll && ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
        ImGui::SetScrollHereY(1.0f);

    ImGui::PopStyleVar();
    ImGui::EndChild();
    ImGui::Separator();

    bool reclaim_focus = false;
    ImGuiInputTextFlags input_text_flags = ImGuiInputTextFlags_EnterReturnsTrue;
    
    ImGui::PushItemWidth(-100);
    if (ImGui::InputText("Command", InputBuf, IM_ARRAYSIZE(InputBuf), input_text_flags))
    {
        LogInfo("Executed command: %s", InputBuf);
        memset(InputBuf, 0, sizeof(InputBuf));
        reclaim_focus = true;
    }
    ImGui::PopItemWidth();
    
    ImGui::SetItemDefaultFocus();
    if (reclaim_focus)
        ImGui::SetKeyboardFocusHere(-1);

    ImGui::SameLine();
    if (ImGui::Button("Help"))
    {
        LogInfo("Available commands: help, clear, test");
    }

    ImGui::End();
}
