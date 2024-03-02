// библиотека ввода - вывода
#include <iostream>
// библиотека для векторов
#include <vector>
// библиотека для работы с временем
#include <chrono>
// библиотека для форматирования вывода
#include <iomanip>


using namespace std;
using namespace std::chrono;

// ФУНКЦИИ ВЫВОДА
// функция для вывода меню
void printMenu() {
    cout << "--------------------- Меню ----------------------" << endl;
    cout << "1. Решить систему линейных уравнений" << endl;
    cout << "2. Показать помощь" << endl;
    cout << "3. Выйти из программы" << endl;
    cout << "-------------------------------------------------" << endl;
}
// функция для вывода справки
void printHelp() {
    cout << "---------------------- Помощь -----------------------" << endl;
    cout << "Программа позволяет решить систему линейных уравнений" << endl;
    cout << "с использованием метода Крамера." << endl;
    cout << "Для решения системы необходимо ввести коэффициенты" << endl;
    cout << "матрицы системы и вектор свободных членов." << endl;
    cout << "-----------------------------------------------------" << endl;
}
// функция для вывода СЛАУ
void printSLAU(const vector<vector<double>>& matrix, const vector<double>& vector_b, int size) {
    cout << "-------------------СЛАУ--------------------" << endl;
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            cout << matrix[i][j] << " ";
        }
        cout << "| " << vector_b[i] << endl;
    }
}
// функция для вывода модифицированных матриц
void printModifiedMatrix(const vector<vector<double>>& modifiedMatrix, int numberOfMatrix, double determinant) {
    // наверное, понятно, что тут
    cout << "----------------Матрица " << numberOfMatrix << ":-----------------" << endl;
    // получаем размер матрицы
    int size = modifiedMatrix.size();
    // цикл для вывода матрицы, идем по строкам
    for (int i = 0; i < size; i++) {
        // цикл для вывода матрицы, идем по столбцам
        for (int j = 0; j < size; j++) {
            // выводим элемент
            cout << modifiedMatrix[i][j] << " ";
        }
        // переход на новую строку
        cout << endl;
    }
    // выводим определитель матрицы
    cout << "Определитель матрицы = " << determinant << endl << "-------------------------------------------" << endl;
}
// ФУНКЦИИ ВВОДА
// функция ввода и проверки ввода
double inputNumber() {
    // оглашаем переменную для ввода числа
    double number;
    bool validInput = false;
    // цикл будет продолжаться, пока не будет введено число
    do {
        // еслм введено число
        if (cin >> number) {
            validInput = true;
        }
        // если введено не число
        else {
            // чистим ввод
            cin.clear();
            // игнорируем неправильный ввод
            cin.ignore(numeric_limits<std::streamsize>::max(), '\n');
            // отправляем ошибку
            throw runtime_error("Неверный ввод");
        }
    } while (!validInput);

    return number;
}
// функция для ввода размерности СЛАУ
int getSize() {
    int size;
    // проверка на правильность ввода
    bool validInput = false;
    // наверное,понятно что тут
    cout << "------Введите количество переменных------- " << endl;
    // цикл do while для ввода размерности матрицы
    do {
        try {
            // ...
            cout << "size = ";
            size = int(inputNumber());
            validInput = true;
        }
        catch (const runtime_error& e) {
            // выводим ошибку
            cout << "Ошибка: " << e.what() << endl;
        }
    } while (!validInput);
    return (size);
}
// функция для ввода матрицы коэффициентов
vector<vector<double>> inputMatrix(int size) {
    // оглашаем переменную, отвечающую за правильность ввода
    bool validInput = false;
    // ввод коэффициентов уравнений, создаем вектор матрицы
    vector<vector<double>> matrix(size, vector<double>(size));
    // ...
    cout << "-----------Введите коэффициенты------------" << endl;
    // цикл для ввода коэффициентов, по строкам
    for (int i = 0; i < size; i++) {
        // цикл для ввода коэффициентов, по стобцам
        for (int j = 0; j < size; j++) {
            // присваиваем переменной validInput значение ЛОЖЬ (false)
            validInput = false;
            // цикл do - while
            do {
                // выводим какой коэффициент вводим
                cout << "Коэффициент [" << i + 1 << "][" << j + 1 << "] = ";
                // начинаем отлов исключений
                try {
                    matrix[i][j] = inputNumber();
                    validInput = true;
                    // ловим ошибку
                }
                catch (const runtime_error& e) {
                    // выводим ошибку
                    cout << "Ошибка: " << e.what() << endl;
                }
                // цикл будет повторятся, пока не будет введено число
            } while (!validInput);
        }
    }
    return(matrix);
}
// функция для ввода вектора
vector<double> inputVector(int size) {
    // оглашаем переменную и присваиваем ей значение
    bool validInput = false;
    // создаем вектор свободных членов
    vector<double> vector_b(size);
    // понятно, надеюсь
    cout << "-----Введите вектор свободных членов-------" << endl;
    // цикл для ввода, строки
    for (int i = 0; i < size; i++) {
        validInput = false;
        do {
            cout << "Свободный член [" << i + 1 << "] = ";
            try {
                vector_b[i] = inputNumber();
                validInput = true;
            }
            catch (const runtime_error& e) {
                cout << "Ошибка: " << e.what() << endl;
            }
        } while (!validInput);
    }

    return vector_b;
}
// ФУНКЦИИ ДЛЯ ВЫЧИСЛЕНИЙ
// функция для вычисления определителя матрицы методом Гаусса
double determinant(const vector<vector<double>>& matrix) {
    // получаем размер матрицы
    int size = matrix.size();

    // проверка на "квадратность" матрицы (не получится ввести не квадратную)
    for (int i = 0; i < size; i++) { 
        // если не совпадают размеры
        if (matrix[i].size() != size) {
            // отправим ошибку с текстом ниже
            throw runtime_error("Матрица не является квадратной.");
        }
    }
    // вводим буфферную матрицу
    vector<vector<double>> temp_matrix = matrix;
    // оглашаем переменную для определителя
    double det = 1.0;
    //берем главную диагональ
    for (int i = 0; i < size; i++) {
        double pivot = temp_matrix[i][i];

        // проверка на ноль на главной диагонали
        if (pivot == 0) {
            throw runtime_error("Матрица вырожденная, определитель равен нулю.");
        }

        // делим строку на ведущий элемент
        for (int j = i; j < size; j++) {
            temp_matrix[i][j] /= pivot;
        }
        det *= pivot;

        // вычитаем текущую строку из остальных строк
        for (int j = i + 1; j < size; j++) {
            double factor = temp_matrix[j][i];
            for (int k = i; k < size; k++) {
                temp_matrix[j][k] -= factor * temp_matrix[i][k];
            }
        }
    }
    // возвращаем определитель
    return det;
}
// функция для решения системы линейных уравнений методом Крамера
vector<double> solveUsingCramer(const vector<vector<double>>& matrix, const vector<double>& vector_b) {
    // количество переменных
    int size = matrix.size(); 
    // номер матрицы
    int numberOfMatrix = 1; 
    // проверка на совпадение размерностей матрицы и вектора b
    if (size != vector_b.size()) {
        // отправляем ошибку с текстом ниже
        throw runtime_error("Размеры матрицы и вектора b не совпадают.");
    }

    // вычисление определителя матрицы
    double det_A = determinant(matrix);
    // выводим определитель матрицы
    cout << "Определитель матрицы: = " << det_A << endl;
    // проверка на невырожденность матрицы
    if (det_A == 0) {
        // отправляем ошибку с текстом ниже
        throw runtime_error("Матрица вырожденная, система не имеет решения.");
    }
    // создаем вектор с ответами
    vector<double> solution(size);

    // вычисляем определитель для каждой измененной матрицы
    for (int i = 0; i < size; i++) {
        // создаем вектор модифицированной матрицы, равный обычной матрице
        vector<vector<double>> modified_matrix = matrix;
        // цикл для замены столбца
        for (int j = 0; j < size; j++) {
            // заменяем столбец матрицы на свободные члены
            modified_matrix[j][i] = vector_b[j];
        }
        // вычисляем определитель измененной матрицы
        double det_A_i = determinant(modified_matrix);
        // получаем ответ, деля определитель измененной матрицы, на определитель оригинальной матрицы
        solution[i] = det_A_i / det_A;
        // выводим модифицированную матрицу
        printModifiedMatrix(modified_matrix, numberOfMatrix,det_A_i);
        // добавляем 1 для номера матрицы
        numberOfMatrix++;
    }
    // возвращаем вектор с решениями
    return solution;
}
// функция 1 пункта меню
int solveSystem() {
    // количество переменных
    int size = getSize(); 
    // проверка на допустимое количество переменных
    if (size <= 0 || size > 20) {
        // ...
        cout << "----Недопустимое количество переменных.----" << endl;
        return 0;
    }
    vector<vector<double>> matrix = inputMatrix(size);
    vector<double> vector_b = inputVector(size);
    printSLAU(matrix, vector_b, size);
    // Решение системы уравнений методом Крамера
    try {
        auto start = high_resolution_clock::now();
        vector<double> solution = solveUsingCramer(matrix, vector_b);
        auto end = high_resolution_clock::now();

        // Вывод результатов
        cout << "Результаты решения:" << endl;
        for (int i = 0; i < size; i++) {
            cout << "x" << i + 1 << " = " << solution[i] << endl;
        }

        // Вывод времени, затраченного на расчет
        auto duration = duration_cast<milliseconds>(end - start);
        cout << "Время выполнения: " << fixed << setprecision(2) << duration.count() / 1000.0 << " сек" << endl;
    }
    catch (const exception& e) {
        cout << "Ошибка: " << e.what() << endl;
    }

    return 0;
}
// основная функция
int main() {
    // установка русского языка в консоли
    setlocale(LC_ALL, "");
    // оглашаем переменную для выбора пунктов меню
    int choice;
    // выводим название, фио или чет такое добавь сама, так же через system(pause) можно 
    cout << "Программа для решения системы линейных уравнений" << endl;
    // бесконечный цикл
    while(true){
        // вызов функции вывода меню
        printMenu();
        // наверное, понятно что тут
        cout << "Пункт меню: ";
        // вводим с клавиатуры пункт меню
        cin >> choice;
        // в зависимости от выбора, вызываем функцию, так же отлов исключений
        try {
            if (choice == 1) {
                // вызов функции, которая спросит основные данные о матрице 
                solveSystem();
            }
            else if (choice == 2) {
                // вывод "помощи"
                printHelp();
            }
            else if (choice == 3) {
                // выход
                break;
            }
            // исключение не правильного ввода
            else {
                throw runtime_error("Неверный выбор пункта меню.");
            }
        }
        // отлов исключения
        catch (const std::runtime_error& e) {
            // вывод ошибки
            std::cout << "Ошибка: " << e.what() << std::endl;
        }
    }
}
/*
github.com/xanax01d, t.me/xanax01d 
date :03.03.2024, last change time 01:21 
*/
