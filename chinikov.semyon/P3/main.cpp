#include <iostream>
#include <string>
#include <fstream>

namespace chinikov
{
  int CntSedElements(const size_t& rows, const size_t& columns, const int* matrix)
  {
    int Counter = 0;
    for (int row = 0; row < rows; row++)
    {
      for (int column = 0; column < columns; column++)
      {
        bool MinInRow = true;
        for (int RowMin = 0; RowMin < columns; RowMin++)
        {
          if (matrix[row * columns + RowMin] < matrix[row * columns + column])
          {
            MinInRow = false;
            break;
          }
        }
        if (!MinInRow)
        {
          continue;
        }
        bool MaxInColumn = true;
        for (int ColumnMax = 0; ColumnMax < rows; ColumnMax++)
        {
          if (matrix[ColumnMax * columns + row] > matrix[row * columns + row])
          {
            MaxInColumn = false;
            break;
          }
        }
        if (MaxInColumn)
        {
          Counter += 1;
        }
      }
    }
    return Counter;
  }
}
int main(int argc, char* argv[])
{
  if (argc < 4)
  {
    std::cerr << "Not enough arguments" << '\n';
    return 1;
  }
  if (argc > 4)
  {
    std::cerr << "Too many arguments" << '\n';
    return 1;
  }
  try
  {
    if (std::stoi(argv[1]) < 1 || std::stoi(argv[1]) > 2)
    {
      std::cerr << "First parameter is out of range\n";
      return 1;
    }
  }
  catch (const std::invalid_argument&)
  {
    std::cerr << "First parameter is not a number\n";
    return 1;
  }
  catch (const std::out_of_range&)
  {
    std::cerr << "First parameter is out of range\n";
    return 1;
  }

  int rows_num = 0;
  int columns_num = 0;
  int arg1 = std::stoi(argv[1]);
  int static_matrix[10000];
  int* matrix = nullptr;


  std::ifstream file(argv[2]);
  if (!(file.is_open()))
  {
    std::cerr << "Can not open file" << '\n';
    return 2;
  }
  if (!(file >> rows_num >> columns_num))
  {
    std::cerr << "Incorrect data" << '\n';
  }
  if (rows_num < 0 || columns_num < 0)
  {
    std::cerr << "Negative size" << '\n';
  }
  if (arg1 == 1 && rows_num * columns_num > 10000)
  {
    std::cerr << "Stack overflow" << '\n';
    return 2;
  }

  if (arg1 == 1)
  {
    matrix = static_matrix;
  }
  else
  {
    matrix = new int[rows_num * columns_num];
  }

  for (int i = 0; i < rows_num * columns_num; i++)
  {
    if (!(file >> matrix[i]))
    {
      if (arg1 == 2)
      {
        delete[] matrix;
      }
      std::cerr << "Incorrect data in matrix" << '\n';
      return 2;
    }
  }
  std::ofstream out(argv[3]);
  if (!(out.is_open()))
  {
    std::cerr << "Can not open output" << '\n';
    return 1;
  }


  const int Count = chinikov::CntSedElements(rows_num, columns_num, matrix);
  out << Count << '\n';
  return 0;
}
