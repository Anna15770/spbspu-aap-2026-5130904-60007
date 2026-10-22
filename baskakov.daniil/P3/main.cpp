#include <iostream>
#include <fstream>
#include <string>
#include <cstddef>

namespace baskakov {
  constexpr int expected_argc = 4;
  constexpr int err_args = 1;
  constexpr int err_data = 2;
  constexpr std::size_t max_stack_size = 10000;

  enum class ParamStatus {
    Success = 0,
    NotANumber,
    OutOfRange
  };

  ParamStatus checkFirstParam(const char* firstParam)
  {
    try {
      std::size_t charNum = 0;
      const std::string argStr(firstParam);
      const int value = std::stoi(argStr, &charNum);
      if (argStr.length() != charNum) {
        return ParamStatus::NotANumber;
      }
      if (value != 1 && value != 2) {
        return ParamStatus::OutOfRange;
      }
      return ParamStatus::Success;
    } catch (const std::invalid_argument &) {
      return ParamStatus::NotANumber;
    } catch (const std::out_of_range &) {
      return ParamStatus::OutOfRange;
    }
  }

  int countSaddlePoints(const int* matrix, int rowsNum, int columnsNum)
  {
    int cnt = 0;
    for (int i = 0; i < rowsNum; ++i) {
      for (int j = 0; j < columnsNum; ++j) {
        bool isMinInRow = true;
        for (int k = 0; k < columnsNum; ++k) {
          if (matrix[i * columnsNum + k] < matrix[i * columnsNum + j]) {
            isMinInRow = false;
            break;
          }
        }

        if (!isMinInRow) {
          continue;
        }

        bool isMaxInCol = true;
        for (int p = 0; p < rowsNum; ++p) {
          if (matrix[p * columnsNum + j] > matrix[i * columnsNum + j]) {
            isMaxInCol = false;
            break;
          }
        }

        if (isMaxInCol) {
          ++cnt;
        }
      }
    }
    return cnt;
  }
}

int main(int argc, char* argv[])
{
  if (argc < baskakov::expected_argc) {
    std::cerr << "Not enough arguments\n";
    return baskakov::err_args;
  }
  if (argc > baskakov::expected_argc) {
    std::cerr << "Too many arguments\n";
    return baskakov::err_args;
  }

  const baskakov::ParamStatus status = baskakov::checkFirstParam(argv[1]);
  if (status == baskakov::ParamStatus::NotANumber) {
    std::cerr << "First parameter is not a number\n";
    return baskakov::err_args;
  }
  if (status == baskakov::ParamStatus::OutOfRange) {
    std::cerr << "First parameter is out of range\n";
    return baskakov::err_args;
  }

  std::ifstream file(argv[2]);
  if (!file.is_open()) {
    std::cerr << "Cannot open input file\n";
    return baskakov::err_data;
  }

  int rowsNum = 0;
  int columnsNum = 0;
  if (!(file >> rowsNum >> columnsNum)) {
    std::cerr << "Invalid matrix dimensions\n";
    return baskakov::err_data;
  }

  if (rowsNum < 0 || columnsNum < 0) {
    std::cerr << "Dimensions cannot be negative\n";
    return baskakov::err_data;
  }

  std::ofstream out(argv[3]);
  if (!out.is_open()) {
    std::cerr << "Cannot open output file\n";
    return baskakov::err_data;
  }

  const int total = rowsNum * columnsNum;
  const int mode = std::stoi(argv[1]);

  int stackMatrix[baskakov::max_stack_size];
  int* dynamicMatrix = nullptr;
  int* matrix = nullptr;

  if (mode == 1) {
    if (static_cast<std::size_t>(total) > baskakov::max_stack_size) {
      std::cerr << "Matrix exceeds max stack size\n";
      return baskakov::err_data;
    }
    matrix = stackMatrix;
  } else {
    dynamicMatrix = new int[total];
    matrix = dynamicMatrix;
  }

  for (int i = 0; i < total; ++i) {
    if (!(file >> matrix[i])) {
      delete[] dynamicMatrix;
      std::cerr << "Invalid matrix data\n";
      return baskakov::err_data;
    }
  }

  std::string extra;
  if (file >> extra) {
    delete[] dynamicMatrix;
    std::cerr << "Extra data in file\n";
    return baskakov::err_data;
  }

  const int cnt = baskakov::countSaddlePoints(matrix, rowsNum, columnsNum);
  delete[] dynamicMatrix;

  out << cnt << "\n";
  return 0;
}