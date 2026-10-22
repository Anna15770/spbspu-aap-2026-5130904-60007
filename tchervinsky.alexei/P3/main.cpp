#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <string>

const int ARGC_EXACT = 4;
const int NUM_STATIC = 1;
const int NUM_DYNAMIC = 2;
const std::size_t MAX_NUMBER_OF_ELEMENTS = 10000;

// 1. Функция для выделения памяти и чтения цельного массива из файла
bool readMatrix(std::ifstream & file, std::size_t rows, std::size_t cols, int *matrix)
{

    for (std::size_t i = 0; i < rows; i++) {
        for (std::size_t j = 0; j < cols; j++) {
            // matrix + i * cols + j — вычисляет точный адрес элемента в сплошной ленте памяти
            file >> *(matrix + i * cols + j);
            if (!file)
            {
             return false;
            }
        }
    }
    file.close();
    return true;
}

// 2. Функция для вывода цельной матрицы
void printMatrix(const int* matrix, std::size_t rows, std::size_t cols) {
    std::cout << "Матрица:\n";
    for (std::size_t i = 0; i < rows; i++) {
        for (std::size_t j = 0; j < cols; j++) {
            // Разыменовываем указатель для получения значения
            std::cout << *(matrix + i * cols + j) << "\t";
        }
        std::cout << "\n";
    }
}

// 3. Функция подсчета седловых элементов (Строгое определение: Min в строке, Max в столбце)
std::size_t countSaddleElements(const int* matrix, std::size_t rows, std::size_t cols) {
    std::size_t count = 0;

    for (std::size_t i = 0; i < rows; i++) {
        for (std::size_t j = 0; j < cols; j++) {
            // Текущий проверяемый элемент
            int current = *(matrix + i * cols + j);
            bool isMinInRow = true;
            bool isMaxInCol = true;

            // Проверка по строке i (меняем индекс столбца k)
            for (std::size_t k = 0; k < cols; k++) {
                if (*(matrix + i * cols + k) < current) {
                    isMinInRow = false;
                    break;
                }
            }

            // Проверка по столбцу j (меняем индекс строки k)
            for (std::size_t k = 0; k < rows; k++) {
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

int main(int argc, char *argv[])
{
  if (argc < ARGC_EXACT)
  {
    std::cerr << "Not enough arguments" << '\n';
    return 1;
  }
  else if (argc > ARGC_EXACT)
  {
    std::cerr << "Too many arguments" << '\n';
    return 1;
  }

  std::stringstream ss_num(argv[1]);
  int num{0};
  ss_num >> num;
  std::streambuf *pbuf = ss_num.rdbuf();
  const std::streamsize ss_num_avail = pbuf->in_avail();
  if (!ss_num || ss_num_avail != 0)
  {
    std::cerr << "First parameter is not a number"  << '\n';
    return 1;
  }

  if (num != NUM_STATIC && num != NUM_DYNAMIC)
  {
    std::cerr << "First parameter is out of range" << '\n';
    return 1;
  }

  std::ifstream ifs(argv[2]);
  if (!ifs)
  {
    std::cerr << "Input file not found" << '\n';
    return 1;
  }

std::ofstream ofs(argv[3]);
    if (!ofs)
    {
    std::cerr << "Output file cannot be opened" << '\n';
    return 1;
    }


  std::size_t rows;
  ifs >> rows;
  if (!ifs)
  {
    std::cerr << "Input error in rows" << '\n';
    return 2;
  }

  std::size_t cols;
  ifs >> cols;
  if (!ifs)
  {
    std::cerr << "Input error in cols" << '\n';
    return 2;
  }

  int arr_static[MAX_NUMBER_OF_ELEMENTS]{0};
  int *arr{nullptr};
  if (num == NUM_STATIC)
  {
    arr = arr_static;
  }
  else
  {
    arr = new int [rows * cols];
  }

    // Вызываем функции по цепочке
    if (readMatrix(ifs, rows, cols, arr) == false)
    {
        if (num == NUM_DYNAMIC)
        {
            freeMatrix(arr);
        }
        ifs.close();
        std::cerr << "Input error in data" <<'\n';
        return 2;
    }

    ifs.close();

    // printMatrix(matrix, ROWS, COLS);

    std::size_t totalSaddle = countSaddleElements(arr, rows, cols);

    ofs << totalSaddle << std::endl;
    ofs.close();

    if (num == NUM_DYNAMIC)
    {
        freeMatrix(arr);
    }
    return 0;
}
