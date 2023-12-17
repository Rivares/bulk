#include <gperftools/profiler.h>

#include <string>
#include <iostream>
#include <queue>

#include "lib.hpp"

int main(int argc, const char* argv[])
{
    ProfilerStart("bulk.prof");

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
                    std::queue<std::string> poolCommands;
                    std::queue<std::string> poolSubCommands;
                    size_t cntCommands = std::stoi(static_cast<std::string>(argv[1]));
                    std::string currCommand = "";
                    bool streamSubCommands = false;
                    auto output = [cntCommands](std::queue<std::string>& poolCommands){
                        if (poolCommands.size() >= cntCommands)
                        {
                            for (size_t i = 0; i < cntCommands; ++i)
                            {   std::cout << poolCommands.front() << '\n';  poolCommands.pop();   }
                        }

                    };
                    while (std::getline(std::cin, currCommand))
                    {
                        if (currCommand == "{")
                        {   streamSubCommands = true;   continue;   }
                        if (currCommand == "}")
                        {
                            streamSubCommands = false;
                            output(poolSubCommands);
                            continue;
                        }

                        if (streamSubCommands)
                        {   poolSubCommands.push(currCommand);  }
                        else
                        {
                            poolCommands.push(currCommand);
                        }

                        if ((poolCommands.size() % cntCommands) == 0)
                        {
                            output(poolCommands);
                        }
                    }

                    while (poolCommands.size() != 0)
                    {   std::cout << poolCommands.front() << '\n';  poolCommands.pop();   }
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


