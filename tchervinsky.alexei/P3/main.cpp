#include <iostream>
#include <fstream>

// 1. Функция для выделения памяти и чтения цельного массива из файла
int* readMatrix(const char* filename, int rows, int cols, int *matrix)
{
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Ошибка: не удалось открыть файл " << filename << std::endl;
        delete[] matrix; // Освобождаем память в случае ошибки
        return nullptr;
    }

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            // matrix + i * cols + j — вычисляет точный адрес элемента в сплошной ленте памяти
            file >> *(matrix + i * cols + j);
        }
    }
    file.close();
    return matrix;
}

// 2. Функция для вывода цельной матрицы
void printMatrix(const int* matrix, int rows, int cols) {
    std::cout << "Матрица:\n";
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            // Разыменовываем указатель для получения значения
            std::cout << *(matrix + i * cols + j) << "\t";
        }
        std::cout << "\n";
    }
}

// 3. Функция подсчета седловых элементов (Строгое определение: Min в строке, Max в столбце)
int countSaddleElements(const int* matrix, int rows, int cols) {
    int count = 0;

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            // Текущий проверяемый элемент
            int current = *(matrix + i * cols + j);
            bool isMinInRow = true;
            bool isMaxInCol = true;

            // Проверка по строке i (меняем индекс столбца k)
            for (int k = 0; k < cols; k++) {
                if (*(matrix + i * cols + k) < current) {
                    isMinInRow = false;
                    break;
                }
            }

            // Проверка по столбцу j (меняем индекс строки k)
            for (int k = 0; k < rows; k++) {
                if (*(matrix + k * cols + j) > current) {
                    isMaxInCol = false;
                    break;
                }
            }

            // Если элемент подошел под строгое критерий
            if (isMinInRow && isMaxInCol) {
                count++;
                std::cout << "Найден седловой элемент: " << current
                          << " в позиции [" << i << "][" << j << "]\n";
            }
        }
    }
    return count;
}

// 4. Функция освобождения памяти (Теперь удаление происходит в одно действие!)
void freeMatrix(int* matrix) {
    delete[] matrix;
}

int main() {
    const int ROWS = 2;
    const int COLS = 3;

    int rows = ROWS;
    int cols = ROWS;

    // Выделяем ОДИН сплошной блок памяти на rows * cols элементов
    int matrix[ROWS * COLS]{0}; // num 1 (static)
    // int* matrix = new int[rows * cols]; // num 2 (dynamic)


    // Вызываем функции по цепочке
    int* matrix_ret = readMatrix("input.txt", ROWS, COLS, matrix);

    printMatrix(matrix, ROWS, COLS);

    int totalSaddle = countSaddleElements(matrix, ROWS, COLS);
    std::cout << "Всего седловых элементов: " << totalSaddle << std::endl;

    // freeMatrix(matrix);
    return 0;
}
