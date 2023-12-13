#include <gperftools/profiler.h>

#include <iostream>

#include <boost/program_options.hpp>

#include "lib.hpp"

namespace prog_options = boost::program_options;

void set_bulk(size_t bulk)
{
    std::cout << "bulk size is " << bulk << std::endl;
}

int main(int argc, const char *argv[])
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
            prog_options::options_description desc {"Options"};
            desc.add_options()
                    ("help,h", "This screen")
                    ("config", prog_options::value<std::string>()->default_value("app.yaml"), "config filename")
                    ("bulk", prog_options::value<size_t>()->default_value(5)->notifier(set_bulk), "bulk_size")
                    ;
            prog_options::variables_map vm;
            prog_options::store(parse_command_line(argc, argv, desc), vm);
            prog_options::notify(vm);

            if (vm.count("help"))
            {
                std::cout << desc << '\n';
            }
            else if (vm.count("config"))
            {
                std::cout << "readfrom: " << vm["config"].as<std::string>() << '\n';
            }
            else if (vm.count("bulk"))
            {
                std::cout << "bulk: " << vm["bulk"].as<std::size_t>() << '\n';
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


