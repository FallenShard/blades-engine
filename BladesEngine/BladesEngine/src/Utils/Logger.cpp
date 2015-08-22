#include <iomanip>
#include <iostream>
#include <ctime>

#include "Utils/Logger.h"

namespace fsi
{

Logger* Logger::m_instance = nullptr;

Logger::Logger()
{
    openNewFile();
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

    time_t currentTime = time(0);
    tm time;
    localtime_s(&time, &currentTime);
    logger->m_logFile << time.tm_hour << ":" << time.tm_min << ":" << time.tm_sec << " ";
    logger->m_logFile << message;
    logger->m_logFile << '\n';
}

void Logger::flush()
{
    Logger* logger = Logger::getInstance();
    
    logger->m_logFile.close();
    logger->openNewFile();
}

Logger::~Logger()
{
    m_logFile.close();

    delete m_instance;
    m_instance = nullptr;
}

void Logger::openNewFile()
{
    time_t currentTime = time(0);
    tm time;
    localtime_s(&time, &currentTime);

    char strBuffer[80];
    strftime(strBuffer, sizeof(strBuffer), "%Y-%m-%d %H.%M.%S", &time);
    std::string fileName(strBuffer);

    m_logFile.open(fileName + ".log", std::ios::app);
}

}