#include <gperftools/profiler.h>

#include <iostream>
#include <fstream>
#include <string>
#include <queue>
#include <ctime>

class Logger
{
public:
    Logger();
    virtual ~Logger();
};

class LoggerFixedCntCMDs : public Logger
{
public:
    LoggerFixedCntCMDs();
    ~LoggerFixedCntCMDs();
    void logMessage();
};

class LoggerRemainingCMDs : public Logger
{
public:
    LoggerRemainingCMDs();

    ~LoggerRemainingCMDs();

    void logMessage()
    {
//        if (commands.size() == 0)
//        {   return;   }

//        std::ofstream file(projName + std::to_string(static_cast<ulong>(timeFirstCommand)) + ".log");

//        std::cout << projName + ": ";
//        file << projName + ": ";
//        while (commands.size() != 0)
//        {
//            const std::string outStr = commands.front() + (((commands.size() - 1) != 0)? ", " : "\n");
//            std::cout << outStr;
//            file << outStr;
//            commands.pop();
//        }
//        file.close();
    }
};

