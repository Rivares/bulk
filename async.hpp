#include <gperftools/profiler.h>

#include <condition_variable>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <string>
#include <thread>
#include <atomic>
#include <queue>
#include <ctime>
#include <mutex>


//Инициатором обмена будет выступать внешний код. Вместо привычной точки входа в приложение
//main() будут три внешних функции connect(), receive() и disconnect().
//Порядок вызовов следующий:


// вызывается connect() с передачей размера блока команд, сохраняется значение возврата.
//Значение никак не интерпретируется вызывающим кодом и служит только в качестве
//контекста для функций receive() и disconnect().

// вызывается receive() c передачей указателя на начало буфера, его размера, а также
//контекста. Вызов повторяемый – вызывающий код может использовать его для передачи
//нескольких команд подряд.

// вызывается disconnect() с передачей контекста. Вызов разрушает контекст полностью. С
//точки зрения логики обработки команд этот вызов считается завершением текущего блока
//команд.

//Необходимо реализовать эти функции так, чтобы сохранить прежнюю функционально проекта.

//Реализация должна допускать множественные вызовы connect(). Вызовы receive() с разными
//контекстами не должны мешать друг другу. Вызовы могут осуществляться из разных потоков,
//однако вызовы с одинаковым контекстом всегда выполняются из одного и того же потока.
//Опционально реализовать возможность вызывать все функции из любых потоков.



//log – поток для вывода данных в консоль
//file1 – первый поток для вывода в файл
//file2 – второй поток для вывода в файл

//Основная логика обработки меняется таким образом, что блок команд после своего формирования
//должен быть отправлен в консоль (потоком log) и сразу в файл (одним из потоков file1 или file2).
//При этом отправка блока в файл распределяется между файловыми потоками.

//Можно напрямую отправлять, например, чётные команды через поток file1, а нечётные – через
//file2. Но лучшим решение станет использование единой очереди команд, которую будут
//обрабатывать оба файловых потока одновременно.



/*!
    Логика структуры - простое наследование с разграничением функционала по классам
*/

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

    /*!
        Функция void pushCommand(const std::string& command)
        - добавление новой комманды в очередь std::queue<std::string> commands
        и фиксация времени поступления первой комманды time_t timeFirstCommandOut
    */
    void pushCommand(const std::string& command)
    {
        if (command.empty())
        {   return;   }

        commands.push(command);

        if (commands.size() == 1)
        {   timeFirstCommandOut = time(nullptr); }
    }

    /*!
        Функция size_t countCommands() const)
        - получение текущего кол-ва комманд из очереди std::queue<std::string> commands
    */
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

    /*!
        Функция std::string logMessage(Mode currModeOutput = Mode::GEN)
        - вывод комманд из очереди std::queue<std::string> commands в двух режимах:
        1) Mode::GEN - позволяет вывести только фиксированное кол-во комманд из очереди.
        2) Mode::REMAINING - позволяет вывести оставшиеся комманды из очереди.
    */
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

//                Следует обратить внимание на недостаточную точность часов для формирования уникального
//                имени. Необходимо, сохранив timestamp в имени, добавить дополнительный постфикс, который
//                будет гарантированно отличаться у файловых потоков.


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


//советую воспользоваться паттерном наблюдатель (observer) для абстрагирования кода обработки команд
//от конкретного (в файл или консоль) способа вывода.
//Так получится еще больше "развязать" фрагменты кода, относящиеся к парсеру и печати.

class LoggerRemainingCMDs : public Logger
{
    std::string m_outputStr = "";
    std::unique_ptr<std::ofstream> m_ptrToFile;

    std::mutex m_mutex;
    std::condition_variable m_cv;

public:

    std::thread m_theadConsole;
    std::vector<std::thread> m_poolThreadsFiles;
    std::vector<bool> m_poolFreeThreads;

    LoggerRemainingCMDs()
    {   /*std::cout << __PRETTY_FUNCTION__ << '\n';*/
        m_theadConsole = std::thread(&LoggerRemainingCMDs::consoleOutputThread, this);
        m_poolThreadsFiles.emplace_back(&LoggerRemainingCMDs::fileOutputThread, this);
        m_poolThreadsFiles.emplace_back(&LoggerRemainingCMDs::fileOutputThread, this);

        m_poolFreeThreads.push_back(false);
        m_poolFreeThreads.push_back(false);
    }

    ~LoggerRemainingCMDs() override
    {   /*std::cout << __PRETTY_FUNCTION__ << '\n';*/

        if (m_theadConsole.joinable())
        {   m_theadConsole.join();  }

        for (auto& item : m_poolThreadsFiles)
        {
            if (item.joinable())
            {   item.join();  }
        }
        m_poolThreadsFiles.clear();
    }

    /*!
        Функция std::string logMessage()
        - вывод всех комманд из очереди std::queue<std::string> commands.
    */
    std::string logMessage()
    {
        if (commands.size() == 0)
        {   return "";   }

        std::ofstream file(projName + std::to_string(static_cast<ulong>(timeFirstCommandOut)) + ".log");

        const std::string prefixStr = projName + ": ";
        std::string outputStrForTests = prefixStr;


        {
            std::lock_guard<std::mutex> locker(m_mutex);
            m_outputStr = prefixStr;
            m_ptrToFile = std::make_unique<std::ofstream>(std::move(file));
        }
        m_cv.notify_all();
std::cout << m_outputStr;

        while (commands.size() != 0)
        {
            const std::string outStr = commands.front() + (((commands.size() - 1) != 0)? ", " : "\n");
            outputStrForTests += outStr;

            {
                std::lock_guard<std::mutex> locker(m_mutex);
                m_outputStr = outStr;
            }
            m_cv.notify_all();
std::cout << m_outputStr;

            commands.pop();
        }        

        return outputStrForTests;
    }

    void consoleOutputThread()
    {
        while(commands.size() != 0)
        {
            std::unique_lock<std::mutex> locker(m_mutex);

            m_cv.wait(locker, [this] { return !m_outputStr.empty(); } );

            std::cout << m_outputStr;
            m_outputStr = "";
        }
    }

    void fileOutputThread()
    {
        while(commands.size() != 0)
        {
            std::unique_lock<std::mutex> locker(m_mutex);

            m_cv.wait(locker, [this] { return ((!m_outputStr.empty()) && (std::count_if(m_poolFreeThreads.begin()
                                                                                        , m_poolFreeThreads.end()
                                                                                        , [](bool val){ return val; }))); } );

            (*m_ptrToFile) << m_outputStr;
            m_outputStr = "";
        }

        if ((m_ptrToFile) && ((*m_ptrToFile).is_open()))
        {   (*m_ptrToFile).close(); }
    }
};

