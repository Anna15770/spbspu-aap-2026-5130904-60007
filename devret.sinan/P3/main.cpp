#include <cstddef>
#include <fstream>
#include <iostream>
#include <limits>
#include <new>

namespace devret {
const std::size_t MAX_SIZE = 10000;

bool read_elements(std::ifstream &input, int *matrix, std::size_t total) {
  for (std::size_t i = 0; i < total; ++i) {
    if (!(input >> matrix[i])) {
      return false;
    }
  }

  char extra = '\0';
  if (input >> extra) {
    return false;
  }

  return input.eof();
}

int count_rows(const int *matrix, int rows, int cols) {
  int count = 0;

  for (int i = 0; i < rows; ++i) {
    bool valid = true;
    const std::size_t start =
        static_cast<std::size_t>(i) * static_cast<std::size_t>(cols);

    for (int j = 1; j < cols; ++j) {
      const std::size_t index = start + static_cast<std::size_t>(j);

      if (matrix[index] == matrix[index - 1]) {
        valid = false;
        break;
      }
    }

    if (valid) {
      ++count;
    }
  }

  return count;
}

int write_result(const char *output_name, int result) {
  std::ofstream output(output_name);

  if (!output) {
    std::cerr << "Error: cannot open output file\n";
    return 2;
  }

  output << result << '\n';
  output.close();

  if (!output) {
    std::cerr << "Error: cannot write output file\n";
    return 2;
  }

  return 0;
}
} // namespace devret

int main(int argc, char *argv[]) {
  if (argc != 4) {
    std::cerr << "Error: usage: lab <1|2> <input> <output>\n";
    return 1;
  }

  if ((argv[1][0] != '1' && argv[1][0] != '2') || argv[1][1] != '\0') {
    std::cerr << "Error: first argument must be 1 or 2\n";
    return 1;
  }

  const int mode = argv[1][0] - '0';
  std::ifstream input(argv[2]);

  if (!input) {
    std::cerr << "Error: cannot open input file\n";
    return 2;
  }

  int rows = 0;
  int cols = 0;

  if (!(input >> rows >> cols) || rows < 0 || cols < 0) {
    std::cerr << "Error: invalid input dimensions\n";
    return 2;
  }

  const std::size_t row_count = static_cast<std::size_t>(rows);
  const std::size_t col_count = static_cast<std::size_t>(cols);
  const std::size_t max_elements =
      std::numeric_limits<std::size_t>::max() / sizeof(int);

  if (row_count != 0 && col_count > max_elements / row_count) {
    std::cerr << "Error: matrix is too large\n";
    return 2;
  }

  const std::size_t total = row_count * col_count;

  if (mode == 1 && total > devret::MAX_SIZE) {
    std::cerr << "Error: matrix is too large\n";
    return 2;
  }

  if (mode == 1) {
    int matrix[devret::MAX_SIZE];

    if (!devret::read_elements(input, matrix, total)) {
      std::cerr << "Error: invalid input matrix\n";
      return 2;
    }

    const int result = devret::count_rows(matrix, rows, cols);
    return devret::write_result(argv[3], result);
  }

  int *const matrix = total == 0 ? nullptr : new (std::nothrow) int[total];

  if (total != 0 && matrix == nullptr) {
    std::cerr << "Error: memory allocation failed\n";
    return 2;
  }

  if (!devret::read_elements(input, matrix, total)) {
    delete[] matrix;
    std::cerr << "Error: invalid input matrix\n";
    return 2;
  }

  const int result = devret::count_rows(matrix, rows, cols);
  delete[] matrix;

  return devret::write_result(argv[3], result);
}