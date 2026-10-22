#include <iostream>
#include <fstream>
#include <string>
#include <cstddef>
#include <stdexcept>

namespace baskakov {
  constexpr int expected_argc = 4;
  constexpr int err_args = 1;
  constexpr int err_data = 2;
  constexpr int max_stack_size = 10000;

  enum class ParamStatus
  {
    Success = 0,
    NotANumber,
    OutOfRange
  };

  ParamStatus checkFirstParam(const char* first_param)
  {
    try {
      std::size_t char_num = 0;
      const std::string arg_str(first_param);
      const int value = std::stoi(arg_str, &char_num);
      if (arg_str.length() != char_num) {
        return ParamStatus::NotANumber;
      }
      if (value != 1 && value != 2) {
        return ParamStatus::OutOfRange;
      }
      return ParamStatus::Success;
    } catch (const std::invalid_argument&) {
      return ParamStatus::NotANumber;
    } catch (const std::out_of_range&) {
      return ParamStatus::OutOfRange;
    }
  }

  int countSaddlePoints(const int* matrix, int rows_num, int columns_num)
  {
    int cnt = 0;
    for (int i = 0; i < rows_num; ++i) {
      for (int j = 0; j < columns_num; ++j) {
        bool is_min_in_row = true;
        for (int k = 0; k < columns_num; ++k) {
          if (matrix[i * columns_num + k] < matrix[i * columns_num + j]) {
            is_min_in_row = false;
            break;
          }
        }

        if (!is_min_in_row) {
          continue;
        }

        bool is_max_in_col = true;
        for (int p = 0; p < rows_num; ++p) {
          if (matrix[p * columns_num + j] > matrix[i * columns_num + j]) {
            is_max_in_col = false;
            break;
          }
        }

        if (is_max_in_col) {
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

  int rows_num = 0;
  int columns_num = 0;
  if (!(file >> rows_num >> columns_num)) {
    std::cerr << "Invalid matrix dimensions\n";
    return baskakov::err_data;
  }

  if (rows_num < 0 || columns_num < 0) {
    std::cerr << "Dimensions cannot be negative\n";
    return baskakov::err_data;
  }

  std::ofstream out(argv[3]);
  if (!out.is_open()) {
    std::cerr << "Cannot open output file\n";
    return baskakov::err_data;
  }

  const int total = rows_num * columns_num;
  const int mode = std::stoi(argv[1]);

  int stack_matrix[baskakov::max_stack_size];
  int* dynamic_matrix = nullptr;
  int* matrix = nullptr;

  if (mode == 1) {
    if (total > baskakov::max_stack_size) {
      std::cerr << "Matrix exceeds max stack size\n";
      return baskakov::err_data;
    }
    matrix = stack_matrix;
  } else {
    dynamic_matrix = new int[total];
    matrix = dynamic_matrix;
  }

  for (int i = 0; i < total; ++i) {
    if (!(file >> matrix[i])) {
      delete[] dynamic_matrix;
      std::cerr << "Invalid matrix data\n";
      return baskakov::err_data;
    }
  }

  std::string extra;
  if (file >> extra) {
    delete[] dynamic_matrix;
    std::cerr << "Extra data in file\n";
    return baskakov::err_data;
  }

  const int cnt = baskakov::countSaddlePoints(matrix, rows_num, columns_num);
  delete[] dynamic_matrix;

  out << cnt << "\n";
  return 0;
}
