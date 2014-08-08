#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#include <fstream>

class Logger
{
public:
    static Logger* getInstance();

    void log(std::string message);
    void flush();

    ~Logger();

private:
    Logger();

    static Logger* m_instance;
    std::ofstream m_logFile;
};

#endif