#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>

const int ARGC_EXACT = 4;
const int NUM_STATIC = 1;
const int NUM_DYNAMIC = 2;
const std::size_t MAX_NUMBER_OF_ELEMENTS = 10000;

bool readMatrix(std::ifstream &file, std::size_t rows, std::size_t cols, int *const matrix)     // NOLINT(readability-non-const-parameter)
{
  for (std::size_t i = 0; i < rows; i++)
  {
    for (std::size_t j = 0; j < cols; j++)
    {
      file >> *(matrix + i * cols + j);
      if (!file)
      {
        return false;
      }
    }
  }
  return true;
}

void printMatrix(const int *matrix, std::size_t rows, std::size_t cols)
{
  std::cout << "Матрица:\n";
  for (std::size_t i = 0; i < rows; i++)
  {
    for (std::size_t j = 0; j < cols; j++)
    {
      std::cout << *(matrix + i * cols + j) << "\t";
    }
    std::cout << "\n";
  }
}

std::size_t countSaddleElements(const int *matrix, std::size_t rows, std::size_t cols)
{
  std::size_t count = 0;

  for (std::size_t i = 0; i < rows; i++)
  {
    for (std::size_t j = 0; j < cols; j++)
    {
      const int current = *(matrix + i * cols + j);
      bool is_min_in_row = true;
      bool is_max_in_col = true;

      for (std::size_t k = 0; k < cols; k++)
      {
        if (*(matrix + i * cols + k) < current)
        {
          is_min_in_row = false;
          break;
        }
      }

      for (std::size_t k = 0; k < rows; k++)
      {
        if (*(matrix + k * cols + j) > current)
        {
          is_max_in_col = false;
          break;
        }
      }

      if (is_min_in_row && is_max_in_col)
      {
        count++;
      }
    }
  }
  return count;
}

void freeMatrix(const int *matrix)
{
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
    std::cerr << "First parameter is not a number" << '\n';
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

  std::size_t rows{0};
  ifs >> rows;
  if (!ifs)
  {
    std::cerr << "Input error in rows" << '\n';
    return 2;
  }

  std::size_t cols{0};
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
    arr = new int[rows * cols];
  }

  if (readMatrix(ifs, rows, cols, arr) == false)
  {
    if (num == NUM_DYNAMIC)
    {
      freeMatrix(arr);
    }
    ifs.close();
    std::cerr << "Input error in data" << '\n';
    return 2;
  }

  ifs.close();

  const std::size_t total_saddle = countSaddleElements(arr, rows, cols);

  ofs << total_saddle << std::endl;
  ofs.close();

  if (num == NUM_DYNAMIC)
  {
    freeMatrix(arr);
  }
  return 0;
}
