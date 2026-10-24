#include <cstddef>
#include <fstream>
#include <iostream>
#include <string>

namespace sopin {
  void spiral(int* a, std::size_t rows, std::size_t cols)
  {
    std::size_t top = 0, bottom = rows, left = 0, right = cols;
    int k = 1;
    while (top < bottom && left < right) {
      for (std::size_t j = left; j < right; ++j) {
        a[top * cols + j] -= k++;
      }
      ++top;
      for (std::size_t i = top; i < bottom; ++i) {
        a[i * cols + right - 1] -= k++;
      }
      --right;
      if (top < bottom) {
        for (std::size_t j = right; j-- > left;) {
          a[(bottom - 1) * cols + j] -= k++;
        }
        --bottom;
      }
      if (left < right) {
        for (std::size_t i = bottom; i-- > top;) {
          a[i * cols + left] -= k++;
        }
        ++left;
      }
    }
  }
}

int main(int argc, char** argv)
{
  if (argc != 4) {
    std::cerr << "Wrong number of arguments\n";
    return 1;
  }
  const std::string num = argv[1];
  if (num != "1" && num != "2") {
    std::cerr << "First parameter must be 1 or 2\n";
    return 1;
  }

  std::ifstream in(argv[2]);
  std::size_t rows = 0, cols = 0;
  if (!(in >> rows >> cols) || (num == "1" && rows * cols > 10000)) {
    std::cerr << "Invalid matrix\n";
    return 2;
  }

  int fixed[10000] = {};
  int* a = num == "1" ? fixed : new int[rows * cols];
  for (std::size_t i = 0; i < rows * cols; ++i) {
    if (!(in >> a[i])) {
      if (a != fixed) {
        delete[] a;
      }
      std::cerr << "Invalid matrix\n";
      return 2;
    }
  }

  sopin::spiral(a, rows, cols);

  std::ofstream out(argv[3]);
  out << rows << " " << cols;
  for (std::size_t i = 0; i < rows * cols; ++i) {
    out << " " << a[i];
  }
  out << "\n";

  if (a != fixed) {
    delete[] a;
  }
  return 0;
}
