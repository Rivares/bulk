#include <gperftools/profiler.h>

#include <iostream>
#include <memory>

#include "asyncN.hpp"

#include <thread>

//struct parser;
//extern parser parser;
//size_t connect(size_t N);
//void receive(const char *buff, size_t buff_size, const size_t &id);
//void disconnect(const size_t &id);



//Требования к реализации
//Результатом работы должна стать библиотека, устанавливаемая по стандартному пути. Библиотека
//должна называться libasync.so и находиться в пакете async.
//Для проверки работоспособности следует реализовать также исполняемый файл, который
//использует библиотеку с демонстрацией всех вызовов. Исполняемый файл также должен быть
//добавлен в пакет.

//Проверка
//Задание считается выполненным успешно, если после установки пакета, линковки с тестовым
//кодом (пример в main.cpp) и запуска с тестовыми данными вывод соответствует описанию Задания
//6. Данные подаются порциями в разных контекстах в большом объёме без пауз.


int main([[maybe_unused]] int argc, [[maybe_unused]] const char* argv[])
{
    ProfilerStart("parser_async.prof");

///    Разработать программу для пакетной обработки команд.
///    Команды считываются построчно из стандартного ввода и обрабатываются блоками по N команд.
///    Одна команда - одна строка, конкретное значение роли не играет. Если данные закончились - блок
///    завершается принудительно. Параметр N передается как единственный параметр командной
///    строки в виде целого числа.

    /*!
        Вместе с выводом в консоль блоки должны сохранятся в отдельные файлы с именами
        bulk1517223860.log , где 1517223860 - это время получения первой команды из блока. По одному
        файлу на блок.
    */

    /*!
        Collecting args
    */

    try
    {
        {
            ParserCommands parser;

            auto threadID_1 = parser.connect(3);

            parser.receive("cmd1", 4, threadID_1);
            parser.receive("cmd2", 4, threadID_1);
            parser.receive("{",    1, threadID_1);
            parser.receive("cmd3", 4, threadID_1);
            parser.receive("cmd4", 4, threadID_1);
            parser.receive("}",    1, threadID_1);
            parser.receive("{",    1, threadID_1);
            parser.receive("cmd5", 4, threadID_1);
            parser.receive("cmd6", 4, threadID_1);
            parser.receive("{",    1, threadID_1);
            parser.receive("cmd7", 4, threadID_1);
            parser.receive("cmd8", 4, threadID_1);
            parser.receive("}",    1, threadID_1);
            parser.receive("cmd9", 4, threadID_1);
            parser.receive("}",    1, threadID_1);
            parser.receive("{",    1, threadID_1);
            parser.receive("cmd10", 4, threadID_1);
            parser.receive("cmd11", 4, threadID_1);

            auto threadID_2 = parser.connect(4);
            parser.receive("cmd1", 4, threadID_2);
            parser.receive("cmd2", 4, threadID_2);
            parser.receive("{",    1, threadID_2);
            parser.receive("cmd3", 4, threadID_2);
            parser.receive("cmd4", 4, threadID_2);
            parser.receive("}",    1, threadID_2);
            parser.receive("{",    1, threadID_2);
            parser.receive("cmd5", 4, threadID_2);
            parser.receive("cmd6", 4, threadID_2);
            parser.receive("{",    1, threadID_2);
            parser.receive("cmd7", 4, threadID_2);
            parser.receive("cmd8", 4, threadID_2);
            parser.receive("}",    1, threadID_2);
            parser.receive("cmd9", 4, threadID_2);
            parser.receive("}",    1, threadID_2);
            parser.receive("{",    1, threadID_2);
            parser.receive("cmd10", 4, threadID_2);
            parser.receive("cmd11", 4, threadID_2);

            parser.disconnect(threadID_1);
            parser.disconnect(threadID_2);


//            std::unique_ptr<LoggerFixedCntCMDs> genLogger = std::make_unique<LoggerFixedCntCMDs>(std::stoi(static_cast<std::string>(argv[1])));
//            std::unique_ptr<LoggerRemainingCMDs> otherLogger = std::make_unique<LoggerRemainingCMDs>();

//            std::string currCommand = "";   /// Текущая комманда
//            size_t cntUnfinishedBraces = 0; /// Подсчёт скобок по принципу стека

//            while (std::getline(std::cin, currCommand))
//            {
//                if (currCommand.empty())
//                {   break;   }

//                if (currCommand == "{")
//                {
//                    ++cntUnfinishedBraces;

//                    if (cntUnfinishedBraces == 1)
//                    {
//                        /*!
//                            Вывод оставшихся комманд из статического блока
//                        */
//                        genLogger->logMessage(LoggerFixedCntCMDs::Mode::REMAINING);
//                    }

//                    continue;
//                }
//                if (currCommand == "}")
//                {
//                    --cntUnfinishedBraces;

//                    if (cntUnfinishedBraces == 0)
//                    {
//                        /*!
//                            Вывод комманд из динамического блока, когда ввод закончен
//                        */
//                        otherLogger->logMessage();
//                    }

//                    continue;
//                }

//                /*!
//                    Ввод комманд из динамического и статического блоков
//                */
//                if (cntUnfinishedBraces >= 1)
//                {
//                    otherLogger->pushCommand(currCommand);
//                }
//                else
//                {
//                    genLogger->pushCommand(currCommand);
//                }

//                /*!
//                    Вывод фиксированного кол-ва комманд из статического блока
//                */
//                genLogger->logMessage();
//            }
//            genLogger->logMessage(LoggerFixedCntCMDs::Mode::REMAINING);

        }

        {
            const std::size_t numberOfCommandsPerThread = 1;//10;

            auto worker = [numberOfCommandsPerThread](std::size_t threadNum, std::size_t blockSize) {
                auto handle = connect(blockSize);
                for (std::size_t i = 0; i < numberOfCommandsPerThread; ++i) {
                    std::string command{"cmd"};
                    command += std::to_string(i + 1) + "_" + std::to_string(threadNum);
                    receive(command.data(), command.size(), handle);
                }
                disconnect(handle);
            };

            std::thread t1(worker, 0, 1);
            std::thread t2(worker, 1, 3);
            std::thread t3(worker, 2, 5);

            t1.join();
            t2.join();
            t3.join();
        }
    }
    catch (const std::exception& except)
    {
        std::cerr << except.what() << '\n';
    }


    std::cout << "\n";

    ProfilerStop();
    return 0;
}


