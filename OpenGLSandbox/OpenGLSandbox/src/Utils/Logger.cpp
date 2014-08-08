#include <windows.h>
#include <iomanip>
#include "Utils/Logger.h"

Logger* Logger::m_instance = nullptr;

Logger::Logger()
{
    m_logFile.open("error.log", std::ios::app);
}

Logger* Logger::getInstance()
{
    if (m_instance == nullptr)
        m_instance = new Logger();

    return m_instance;
}

void Logger::log(std::string message)
{
    SYSTEMTIME time;
    GetLocalTime(&time);
    m_logFile << '\n' << time.wHour << ":" << time.wMinute << ":" << time.wSecond << " ";
    m_logFile << message;
}

void Logger::flush()
{
    m_logFile.close();

    m_logFile.open("error.log", std::ios::app);
}

Logger::~Logger()
{
    m_logFile.close();
}