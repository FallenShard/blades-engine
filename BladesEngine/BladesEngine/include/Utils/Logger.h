#pragma once

#include <string>
#include <fstream>

#define LOG(message) fsi::Logger::log(message);
#define FLUSH_LOG() fsi::Logger::flush();

namespace fsi
{
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
}