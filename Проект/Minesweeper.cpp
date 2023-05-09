#include <ctime>
#include <cstdlib>
#include <iostream>
#include <Windows.h>

using namespace std;

void getFieldSize(int& width, int& height);
int getMinesCount(int width, int height);

bool start(int** field, int** mines, int width, int height, int nMines);

void displayMineField(int** field, int** mines, int width, int height);
void genMines(int** field, int** mines, int width, int height, int nMines);

bool playerMove(int** field, int** mines, int width, int height);
bool isWinGame(int** field, int** mines, int width, int height);

int main()
{
    setlocale(LC_ALL, "russian");
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    int fieldWidth = 0; // Ширина игрового поля
    int fieldHeight = 0; // Высота игрового поля
    int nMines = 0; // Количество мин на игровом поле

    getFieldSize(fieldWidth, fieldHeight);
    
    // Создаём игровое поле, заданного игроком размера
    int** field = new int* [fieldWidth];
    int** mines = new int* [fieldWidth];
    for (int i = 0; i < fieldWidth; i++)
    {
        field[i] = new int [fieldHeight];
        mines[i] = new int [fieldHeight];
    }

    for (int j = 0; j < fieldHeight; j++)
    {
        for (int i = 0; i < fieldWidth; i++)
        {
            field[i][j] = 0;
            mines[i][j] = 0;
        }
    }

    // Получаем от пользователя количество мин, которое необходимо сгенерировать
    nMines = getMinesCount(fieldWidth, fieldHeight);

    // Выводим результат игры на экран
    if (start(field, mines, fieldWidth, fieldHeight, nMines))
    {
        system("cls");
        cout << "Поздравляем! Вы победили!" << endl << endl;
    }
    else
    {
        system("cls");
        cout << "К сожалению, Вы проиграли" << endl << endl;
    }

    system("pause");
}

/*
 * =================================================
 * | Получение от пользователя параметров для игры |
 * =================================================
*/

void getFieldSize(int& width, int& height)
{
    // Получаем от игрока ширину игрового поля. Для нормальной игры в консоли, задаём рамки от 3 до 9 элементов в ширину
    while (width < 3 || width > 9)
    {
        system("cls");
        cout << "Привет! Начнём игру!" << endl << "Введите ширину игрового поля(от 3 до 9): ";
        cin >> width;
    }

    // Получаем от игрока высоту игрового поля. Для нормальной игры в консоли, задаём рамки от 3 до 9 элементов в высоту
    while (height < 3 || height > 9)
    {
        system("cls");
        cout << "Введите высоту игрового поля(от 3 до 9): ";
        cin >> height;
    }
}

int getMinesCount(int width, int height)
{
    int count = 0;

    /*
     * Просим пользователя ввести количество мин, которое необходимо
     * сгенерировать, пока пользователь не введёт значение больше нуля, но
     * но меньше чем количество клеток на поле
    */
    while (count <= 0 || count > (width * height))
    {
        system("cls");
        cout << "Как много мин Вы хотите сгенерировать?: ";
        cin >> count;
    }

    // Возвращаем количество мин, которое нужно сгенерировать
    return count;
}

/*
 * ======================
 * | Основной цикл игры |
 * ======================
*/

bool start(int** field, int** mines, int width, int height, int nMines)
{
    bool isAnyFieldElementOpen = false;

    while (true)
    {
        // Отображаем игровое поле
        displayMineField(field, mines, width, height);

        /*
         * Если функция playerMove возвращает true - проверяем является ли
         * текущий ход победным
        */
        if(playerMove(field, mines, width, height))
        {
            /* 
             * Если текущий ход является победным, возвращаем true, сообщая
             * основной функции о выигрыше
            */
            if (isWinGame(field, mines, width, height))
            {
                return true;
            }
        }
        else // Иначе, возвращаем false, сообщая основной функции о проигрыше
        {
            return false;
        }

        /*
         * Если ещё ни одна клетка не была открыта - значит это первый ход.
         * Генерируем заданное количество мин на поле так, чтобы на месте
         * первого хода не было мины
        */
        if (isAnyFieldElementOpen == false)
        {
            genMines(field, mines, width, height, nMines);
            isAnyFieldElementOpen = true;
        }
    }
    
}

/*
 * =================================
 * | Вывод игрового поля в консоль |
 * =================================
*/

void displayMineField(int** field, int** mines, int width, int height)
{
    system("cls");

    cout << "  ";

    // Выводим вертикальные цифры-индексы поля
    for (int i = 0; i < width; i++)
    {
        cout << i + 1 << " ";
    }

    cout << endl;

    for (int j = 0; j < height; j++)
    {
        // Выводим горизонтальные цифры-индексы поля
        cout << j + 1 << " ";

        for (int i = 0; i < width; i++)
        {
            int minesCount = 0;

            // Если поле равняется 0 - оно ещё не открыто. Выводим звёздочку
            if (field[i][j] == 0)
            {
                cout << "* ";
            }
            else
            {
                /*
                 * Иначе проверяем окружение открытой клетки, и считаем
                 * количество мин вокруг неё.
                */
                for (int k = j - 1; k <= (j + 1); k++)
                {
                    for (int l = i - 1; l <= (i + 1); l++)
                    {
                        if (k >= 0 && k < height && l >= 0 && l < width)
                        {
                            if (mines[l][k] == 1)
                                minesCount++;
                        }
                    }
                }

                cout << minesCount << " ";
            }
        }

        cout << endl;
    }

    // Игнорируем один ввод с клавиатуры для корректной работы приложения
    cin.ignore();
}

/*
 * ==========================
 * | Генерация минного поля |
 * ==========================
*/

void genMines(int** field, int** mines, int width, int height, int nMines)
{
    int minesCount = 0;

    srand(static_cast<unsigned int>(time(0))); // Автоматическая рандомизация

    while(minesCount < nMines)
    {
        /*
         * Генерируем случайные координаты мины
        */
        int x = rand() % width;
        int y = rand() % height;

        /*
         * Помещаем мину на клетку только если на этом месте ещё нет мины,
         * и если эта клетка не первая клетка, выбранная игроком
        */
        if (mines[x][y] == 0 && field[x][y] == 0)
        {
            mines[x][y] = 1;
            minesCount++;
        }
    }
}

/*
 * =======================
 * | Проверка на выигрыш |
 * =======================
*/

bool isWinGame(int** field, int** mines, int width, int height)
{
    for (int j = 0; j < height; j++)
    {
        for (int i = 0; i < width; i++)
        {
            /*
             * Проходимся по всему игровому полю, проверяя наличие неоткрытых
             * клеток без мин. Если попадаем хоть на одну - значит не все
             * пустые клетки ещё открыты. Возвращаем false, сообщая основному
             * циклу, что игра ещё не выиграна
            */
            if (field[i][j] == 0 && mines[i][j] == 0)
                return false;
        }
    }

    /*
     * Если не осталось неоткрытых пустых клеток без мин - игра выиграна
     * Возвращаем основному циклу значение true, сообщая, что игра выиграна
    */
    return true;
}

/*
 * ===========================================
 * | Обработка выбора клетки от пользователя |
 * ===========================================
*/

bool playerMove(int** field, int** mines, int width, int height)
{
    int x = 0;
    int y = 0;

    /*
     * Просим пользователя ввести X координату клетки
    */
    cout << "Введите X координату(под какой цифрой-индексом находиться клетка): ";
    cin >> x;

    /*
     * Просим пользователя ввести Y координату клетки
    */
    cout << "Введите Y координату(справа от какой цифры-индекса находиться клетка): ";
    cin >> y;

    /*
     * Если на указанной клетке находится мина - возвращаем false, сообщая
     * основному циклу, что игрок попал на мину и проиграл
    */ 
    if (mines[x - 1][y - 1] == 1)
        return false;

    /*
     * Если всё прошло нормально - открываем заданную клетку
    */
    field[x - 1][y - 1] = 1;

    /*
     * Возвращаем true, сообщая основному циклу, что игрок не попал на мину
     * и игра может продолжиться
    */ 
    return true;
}