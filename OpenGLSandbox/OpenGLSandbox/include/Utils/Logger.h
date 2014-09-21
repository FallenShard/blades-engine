#pragma once

#include <string>
#include <fstream>

class Logger
{
public:
    static Logger* getInstance();
    
    static void log(std::string message);
    static void flush();

    ~Logger();

private:
    Logger();

    static Logger* m_instance;
    std::ofstream m_logFile;
};