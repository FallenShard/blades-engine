#pragma once

#include <string>
#include <fstream>

#define LOG(message) Logger::log(message);
#define FLUSH_LOG() Logger::flush();

class Logger
{
public:
    static Logger* getInstance();
    
    static void log(std::string message);
    static void flush();

    ~Logger();

private:
    Logger();

    void openNewFile();

    static Logger* m_instance;
    std::ofstream m_logFile;
};