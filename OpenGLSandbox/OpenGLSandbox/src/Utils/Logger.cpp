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
    Logger* logger = Logger::getInstance();

    SYSTEMTIME time;
    GetLocalTime(&time);
    logger->m_logFile << '\n' << time.wHour << ":" << time.wMinute << ":" << time.wSecond << " ";
    logger->m_logFile << message;
}

void Logger::flush()
{
    Logger* logger = Logger::getInstance();
    
    logger->m_logFile.close();
    logger->m_logFile.open("error.log", std::ios::app);
}

Logger::~Logger()
{
    m_logFile.close();

    delete m_instance;
    m_instance = nullptr;
}