#include <gperftools/profiler.h>

#include <iostream>
#include <memory>

#include "lib.hpp"


int main(int argc, const char* argv[])
{
    ProfilerStart("bulk.prof");

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
        if (argc > 0)
        {
            if (argv)
            {
                std::cout << "Constructions project of objects:\n\n";

                std::unique_ptr<LoggerFixedCntCMDs> genLogger = std::make_unique<LoggerFixedCntCMDs>(std::stoi(static_cast<std::string>(argv[1])));
                std::unique_ptr<LoggerRemainingCMDs> otherLogger = std::make_unique<LoggerRemainingCMDs>();

                std::string currCommand = "";   /// Текущая комманда
                size_t cntUnfinishedBraces = 0; /// Подсчёт скобок по принципу стека

                while (std::getline(std::cin, currCommand))
                {
                    if (currCommand.empty())
                    {   break;   }

                    if (currCommand == "{")
                    {
                        ++cntUnfinishedBraces;

                        if (cntUnfinishedBraces == 1)
                        {
                            /*!
                                Вывод оставшихся комманд из статического блока
                            */
                            genLogger->logMessage(LoggerFixedCntCMDs::Mode::REMAINING);
                        }

                        continue;
                    }
                    if (currCommand == "}")
                    {
                        --cntUnfinishedBraces;

                        if (cntUnfinishedBraces == 0)
                        {
                            /*!
                                Вывод комманд из динамического блока, когда ввод закончен
                            */
                            otherLogger->logMessage();
                        }

                        continue;
                    }

                    /*!
                        Ввод комманд из динамического и статического блоков
                    */
                    if (cntUnfinishedBraces >= 1)
                    {
                        otherLogger->pushCommand(currCommand);
                    }
                    else
                    {
                        genLogger->pushCommand(currCommand);
                    }

                    /*!
                        Вывод фиксированного кол-ва комманд из статического блока
                    */
                    genLogger->logMessage();
                }
                genLogger->logMessage(LoggerFixedCntCMDs::Mode::REMAINING);
            }
        }
        std::cout << "\n\nDestructions objects:\n";
    }
    catch (const std::exception& except)
    {
        std::cerr << except.what() << '\n';
    }


    std::cout << "\n";

    ProfilerStop();
    return 0;
}


