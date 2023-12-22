#include <gperftools/profiler.h>

#include <iostream>
#include <fstream>
#include <string>
#include <queue>
#include <ctime>

#include "lib.hpp"

const std::string projName = "bulk";

int main(int argc, const char* argv[])
{
    ProfilerStart(std::string(projName + ".prof").c_str());

///    Разработать программу для пакетной обработки команд.
///    Команды считываются построчно из стандартного ввода и обрабатываются блоками по N команд.
///    Одна команда - одна строка, конкретное значение роли не играет. Если данные закончились - блок
///    завершается принудительно. Параметр N передается как единственный параметр командной
///    строки в виде целого числа.


    {
        /*!
            Grup args
        */

        try
        {
            if (argc > 0)
            {
                if (argv)
                {
                    std::string currCommand = "";
                    std::queue<std::string> poolCommands;
                    std::queue<std::string> poolSubCommands;
                    size_t cntCommands = std::stoi(static_cast<std::string>(argv[1]));
                    size_t cntUnfinishedBraces = 0;
                    time_t timeFirstCommandInstantOut;
                    time_t timeFirstCommandGeneralOut;

                    auto outputInstant = [&timeFirstCommandInstantOut](const time_t& timeFirstCommand, std::queue<std::string>& commands){

                        if (commands.size() == 0)
                        {   return;   }

                        std::ofstream file(projName + std::to_string(static_cast<ulong>(timeFirstCommand)) + ".log");

                        std::cout << projName + ": ";
                        file << projName + ": ";
                        while (commands.size() != 0)
                        {
                            const std::string outStr = commands.front() + (((commands.size() - 1) != 0)? ", " : "\n");
                            std::cout << outStr;
                            file << outStr;
                            commands.pop();
                        }
                        file.close();
                    };
                    while (std::getline(std::cin, currCommand))
                    {
                        if (currCommand == "{")
                        {
                            ++cntUnfinishedBraces;

                            if (cntUnfinishedBraces == 1)
                            {                                
                                outputInstant(timeFirstCommandGeneralOut, poolCommands);
                            }

                            continue;
                        }
                        if (currCommand == "}")
                        {
                            --cntUnfinishedBraces;

                            if (cntUnfinishedBraces == 0)
                            {
                                outputInstant(timeFirstCommandInstantOut, poolSubCommands);
                            }

                            continue;
                        }

                        if (cntUnfinishedBraces >= 1)
                        {
                            poolSubCommands.push(currCommand);
                        }
                        else
                        {
                            poolCommands.push(currCommand);
                        }

                        if (poolSubCommands.size() == 1)
                        {
                            timeFirstCommandInstantOut = time(nullptr);
                        }
                        if (poolCommands.size() == 1)
                        {
                            timeFirstCommandGeneralOut = time(nullptr);
                        }

                        if ((poolCommands.size() % cntCommands) == 0)
                        {
                            if (poolCommands.size() >= cntCommands)
                            {
                                std::ofstream file(projName + std::to_string(static_cast<ulong>(timeFirstCommandGeneralOut)) + ".log");

                                std::cout << projName + ": ";
                                file << projName + ": ";
                                for (size_t i = 0; i < cntCommands; ++i)
                                {
                                    const std::string outStr = poolCommands.front() + (((i + 1) < cntCommands)? ", " : "\n");
                                    std::cout << outStr;
                                    file << outStr;

                                    poolCommands.pop();
                                }
                                file.close();
                            }
                        }
                    }

                    outputInstant(timeFirstCommandGeneralOut, poolCommands);
                }
            }

        }
        catch (const std::exception& except)
        {
            std::cerr << except.what() << '\n';
        }


        /*!
            Логика работы для статических блоков (в примере N == 3):

            ./bulk 3
            Ввод    Вывод                   Комментарий
            cmd1
            cmd2
            cmd3
                    bulk: cmd1, cmd2, cmd3  Блок завершён – выводим блок.
            cmd4
            cmd5
            EOF
                    bulk: cmd4, cmd5        Конец ввода – принудительно завершаем блок.

cmd1
cmd2
cmd3
cmd4
        */

        std::cout << "Constructions project of objects:\n\n";

///        Размер блока можно изменить динамически, если перед началом блока и сразу после дать
///        команды `{` и `}` соответственно. Предыдущий пакет при этом принудительно завершается. Такие
///        блоки могут быть включены друг в друга при этом вложенные команды `{` и `}` игнорируются (но не
///        сами блоки). Если данные закончились внутри динамического блока, весь динамический блок
///        игнорируется.

        /*!
            Логика работы для динамического размера блоков (в примере N == 3):

            ./bulk 3

            Ввод    Вывод                   Комментарий
            cmd1
            cmd2
            {
                    bulk: cmd1, cmd2        Начало динамического блока – выводим предыдущий статический досрочно
            cmd3
            cmd4
            }
                    bulk: cmd3, cmd4
            {
            cmd5
            cmd6
            {                               Игнорируем вложенные команды.
            cmd7
            cmd8
            }                               Игнорируем вложенные команды.
            cmd9
            }
                    bulk: cmd5, cmd6,       Конец динамического блока – выводим.
                    cmd7, cmd8, cmd9
            {
            cmd10                           Конец ввода – динамический блок игнорируется, не выводим
            cmd11
            EOF


cmd1
cmd2
{
cmd3
cmd4
}
{
cmd5
cmd6
{
cmd7
cmd8
}
cmd9
}
{
cmd10
cmd11

        */


        /*!
            Вместе с выводом в консоль блоки должны сохранятся в отдельные файлы с именами
            bulk1517223860.log , где 1517223860 - это время получения первой команды из блока. По одному
            файлу на блок.
        */



        std::cout << "\n\nDestructions objects:\n";
    }

    std::cout << "\n";


    ProfilerStop();
    return 0;
}


