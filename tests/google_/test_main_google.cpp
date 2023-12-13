#define GOOGLE_TEST_MODULE test_main_google

#include "test_main_google.hpp"

#include <gtest/gtest.h>

//{
//    std::cout << "Constructions project of objects:\n\n";

//    // бесконечная матрица int заполнена значениями -1
//    Matrix<int, -1> matrix;


//    std::cout << "\nWork part:\n\n";

//    assert(matrix.size() == 0); // все ячейки свободны

//    matrix[1][2] = 8;
//    matrix[1][2] = 8;

//    assert(matrix.size() == 1);
//    assert(matrix[1][2] == 8);

//    auto a = matrix[1][2];

//    assert(a == 8);
//    assert(matrix.size() == 1);

//    matrix[100][100] = 314;
//    assert(matrix[100][100] == 314);
//    assert(matrix.size() == 2);

//    ((matrix[100][101] = 314) = 0) = 217;
//    assert(matrix.size() == 3);

////        // 100100314
//    for(auto item: matrix)
//    {
//        int x;
//        int y;
//        int v;
//        std::tie(x, y, v) = item;
//        std::cout << x << y << v << std::endl;
//    }

//    std::cout << "\n\nDestructions objects:\n";
//}

//TEST(Inittialization, Subtest_1)
//{
//    // бесконечная матрица int заполнена значениями -1
//    Matrix<int, -1> matrix;

//    EXPECT_EQ(matrix.size(), 0);    // все ячейки свободны
//}

//TEST(TestGroupName, Subtest_2)
//{
//    // бесконечная матрица int заполнена значениями -1
//    Matrix<int, -1> matrix;

//    EXPECT_EQ(matrix.size(), 0);    // все ячейки свободны

//    // ---------

//    matrix[1][2] = 8;
//    matrix[1][2] = 9;

//    EXPECT_EQ(matrix.size(), 1);    // нет дубляжей
//    EXPECT_TRUE(matrix[1][2] == 9); //
//}

//TEST(TestGroupName, Subtest_3)
//{
//    // бесконечная матрица int заполнена значениями -1
//    Matrix<int, -1> matrix;

//    EXPECT_EQ(matrix.size(), 0);    // все ячейки свободны

//    // ---------

//    matrix[1][2] = 8;
//    matrix[1][2] = 9;

//    EXPECT_EQ(matrix.size(), 1);    // нет дубляжей
//    EXPECT_TRUE(matrix[1][2] == 9);

//    // ---------

//    auto a = matrix[1][2];

//    EXPECT_TRUE(a == 9);                // проверка возврата ссылки из операторов
//    EXPECT_EQ(matrix.size(), 1);    // сверка текущего размера
//}

//TEST(TestGroupName, Subtest_4)
//{
//    // бесконечная матрица int заполнена значениями -1
//    Matrix<int, -1> matrix;

//    EXPECT_EQ(matrix.size(), 0);    // все ячейки свободны

//    // ---------

//    matrix[1][2] = 8;
//    matrix[1][2] = 9;

//    EXPECT_EQ(matrix.size(), 1);    // нет дубляжей
//    EXPECT_TRUE(matrix[1][2] == 9);

//    // ---------

//    auto a = matrix[1][2];

//    EXPECT_TRUE(a == 9);                // проверка возврата ссылки из операторов
//    EXPECT_EQ(matrix.size(), 1);    // сверка текущего размера

//    // ---------

//    matrix[100][100] = 314;

//    EXPECT_TRUE(matrix[100][100] == 314);
//    EXPECT_EQ(matrix.size(), 2);    // сверка текущего размера



//    ((matrix[100][101] = 314) = 0) = 217;   // проверка возврата ссылки из операторов. И перезаписи её значения
//    EXPECT_EQ(matrix.size(), 3);            // сверка текущего размера
//    EXPECT_TRUE(matrix[100][101] == 217);
//}

//TEST(TestGroupName, Subtest_5)
//{
//    // бесконечная матрица int заполнена значениями -1
//    Matrix<int, -1> matrix;

//    EXPECT_EQ(matrix.size(), 0);    // все ячейки свободны

//    // ---------

//    matrix[1][2] = 8;
//    matrix[1][2] = 9;

//    EXPECT_EQ(matrix.size(), 1);    // нет дубляжей
//    EXPECT_TRUE(matrix[1][2] == 9);

//    // ---------

//    auto a = matrix[1][2];

//    EXPECT_TRUE(a == 9);                // проверка возврата ссылки из операторов
//    EXPECT_EQ(matrix.size(), 1);        // сверка текущего размера

//    // ---------

//    matrix[100][100] = 314;

//    EXPECT_TRUE(matrix[100][100] == 314);
//    EXPECT_EQ(matrix.size(), 2);            // сверка текущего размера



//    ((matrix[100][101] = 314) = 0) = 217;   // проверка возврата ссылки из операторов. И перезаписи её значения
//    EXPECT_EQ(matrix.size(), 3);            // сверка текущего размера
//    EXPECT_TRUE(matrix[100][101] == 217);

//    // ---------

//    // проверка вывода значение с использованием for-range (begin(), end(), cbegin(), cend())
//    for(auto item: matrix)
//    {
//        int x;
//        int y;
//        int v;
//        std::tie(x, y, v) = item; // проверка вывода с использованием tuple
//        std::cout << x << y << v << std::endl;
//    }
//}

//TEST(TestGroupName, Subtest_20)
//{
////    EXPECT_NO_THROW(
////        projModel->callSignalAddElem(TypeElem::RECT); // simulation call signal
////            );

////    EXPECT_NO_THROW(
////        projModel->callSignalRemElem(0); // simulation call signal
////            );


//    std::cout << "\n";
//}

