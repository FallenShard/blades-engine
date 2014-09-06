#ifndef LOGGER_H
#define LOGGER_H

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

#endif