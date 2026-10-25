#include <cmath>
#include <cstring>
#include <fstream>
#include <iostream>

namespace volkova {
void printTenths(std::ostream &out, double v) {
  long long t = std::llround(v * 10.0);
  if (t < 0) {
    out << '-';
    t = -t;
  }
  out << t / 10;
  if (t % 10 != 0) {
    out << '.' << t % 10;
  }
}
} // namespace volkova

int main(int argc, char **argv) {
  if (argc != 4) {
    std::cerr << "Wrong number of arguments\n";
    return 1;
  }

  int num = 0;
  if (std::strcmp(argv[1], "1") == 0) {
    num = 1;
  } else if (std::strcmp(argv[1], "2") == 0) {
    num = 2;
  } else {
    std::cerr << "First parameter must be 1 or 2\n";
    return 1;
  }

  std::ifstream in(argv[2]);
  int rows = 0;
  int cols = 0;
  if (!(in >> rows >> cols) || rows < 0 || cols < 0) {
    std::cerr << "Input is not a matrix\n";
    return 2;
  }
  long long total = static_cast<long long>(rows) * cols;

  int stackArr[10000];
  int *dynArr = nullptr;
  int *m = stackArr;
  if (num == 2) {
    dynArr = new int[total];
    m = dynArr;
  } else if (total > 10000) {
    std::cerr << "Matrix is too large\n";
    return 2;
  }

  for (long long i = 0; i < total; ++i) {
    if (!(in >> m[i])) {
      std::cerr << "Input is not a matrix\n";
      delete[] dynArr;
      return 2;
    }
  }
  char extra;
  if (in >> extra) {
    std::cerr << "Input is not a matrix\n";
    delete[] dynArr;
    return 2;
  }

  std::ofstream out(argv[3]);
  out << rows << ' ' << cols;
  for (int r = 0; r < rows; ++r) {
    for (int c = 0; c < cols; ++c) {
      long long sum = 0;
      int cnt = 0;
      for (int dr = -1; dr <= 1; ++dr) {
        for (int dc = -1; dc <= 1; ++dc) {
          int nr = r + dr;
          int nc = c + dc;
          bool isSelf = (dr == 0 && dc == 0);
          bool inside = nr >= 0 && nr < rows && nc >= 0 && nc < cols;
          if (!isSelf && inside) {
            sum += m[nr * cols + nc];
            ++cnt;
          }
        }
      }
      double value =
          (cnt == 0) ? m[r * cols + c] : static_cast<double>(sum) / cnt;
      out << ' ';
      volkova::printTenths(out, value);
    }
  }
  out << '\n';

  delete[] dynArr;
  return 0;
}
