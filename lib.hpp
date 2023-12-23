#include <gperftools/profiler.h>

#include <iostream>
#include <fstream>
#include <string>
#include <queue>
#include <ctime>



class Logger
{
protected:
    std::string projName;
    std::queue<std::string> commands;
    time_t timeFirstCommandOut;

public:
    Logger(const std::string& projName_ = "bulk") :
        projName(projName_)
      , timeFirstCommandOut(time(nullptr))
    {   /*std::cout << __PRETTY_FUNCTION__ << '\n';*/   }
    virtual ~Logger()
    {   /*std::cout << __PRETTY_FUNCTION__ << '\n';*/   }

    void pushCommand(const std::string& command)
    {
        if (command.empty())
        {   return;   }

        commands.push(command);

        if (commands.size() == 1)
        {   timeFirstCommandOut = time(nullptr); }
    }

    size_t countCommands() const
    {   return commands.size(); }
};

class LoggerFixedCntCMDs : public Logger
{
public:
    enum class Mode
    {
        GEN,
        REMAINING
    };

private:
    size_t cntCommands;

public:
    LoggerFixedCntCMDs(size_t cntCommands_ = 3) :
        cntCommands(cntCommands_)
    {   /*std::cout << __PRETTY_FUNCTION__ << '\n';*/   }

    ~LoggerFixedCntCMDs() override
    {   /*std::cout << __PRETTY_FUNCTION__ << '\n';*/   }

    std::string logMessage(Mode currModeOutput = Mode::GEN)
    {
        if (commands.size() == 0)
        {   return "";   }

        std::string outputStrForTests;

        size_t currSizeOfQueue = cntCommands;
        if (currModeOutput == Mode::REMAINING)
        {   currSizeOfQueue = commands.size();  }

        if (commands.size() == currSizeOfQueue)
        {
            std::ofstream file(projName + std::to_string(static_cast<ulong>(timeFirstCommandOut)) + ".log");

            const std::string prefixStr = projName + ": ";
            outputStrForTests = prefixStr;

            std::cout << prefixStr;
            file << prefixStr;
            for (size_t i = 0; i < currSizeOfQueue; ++i)
            {
                const std::string outStr = commands.front() + (((i + 1) < currSizeOfQueue)? ", " : "\n");
                outputStrForTests += outStr;

                std::cout << outStr;
                file << outStr;

                commands.pop();
            }
            file.close();
        }

        return outputStrForTests;
    }
};

class LoggerRemainingCMDs : public Logger
{
public:
    LoggerRemainingCMDs()
    {   /*std::cout << __PRETTY_FUNCTION__ << '\n';*/   }

    ~LoggerRemainingCMDs() override
    {   /*std::cout << __PRETTY_FUNCTION__ << '\n';*/   }

    std::string logMessage()
    {
        if (commands.size() == 0)
        {   return "";   }

        std::ofstream file(projName + std::to_string(static_cast<ulong>(timeFirstCommandOut)) + ".log");

        const std::string prefixStr = projName + ": ";
        std::string outputStrForTests = prefixStr;

        std::cout << prefixStr;
        file << prefixStr;
        while (commands.size() != 0)
        {
            const std::string outStr = commands.front() + (((commands.size() - 1) != 0)? ", " : "\n");
            outputStrForTests += outStr;

            std::cout << outStr;
            file << outStr;

            commands.pop();
        }
        file.close();

        return outputStrForTests;
    }
};

