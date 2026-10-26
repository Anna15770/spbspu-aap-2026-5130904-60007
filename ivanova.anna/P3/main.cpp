#include <fstream>
#include <iostream>

namespace ivanova {
bool readMatrix(const char *filename, int &rows, int &cols, int *data,
                int maxSize) {
  std::ifstream fin(filename);
  if (!fin.is_open())
    return false;

  fin >> rows >> cols;

  if (rows < 0 || cols < 0)
    return false;
  if (rows * cols > maxSize)
    return false;

  int total = rows * cols;
  for (int i = 0; i < total; i++) {
    fin >> data[i];
    if (fin)
      return false;
  }

  int extra;
  if (fin >> extra) {
    return false;
  }

  return true;
}

int findColumn(const int *data, int rows, int cols) {
  if (rows <= 0 || cols <= 0)
    return 0;

  int bestCol = 1;
  int bestLen = 0;

  for (int j = 0; j < cols; j++) {
    int curLen = 1;
    int maxInCol = 1;
    for (int i = 1; i < rows; i++) {
      int cur = data[i * cols + j];
      int prev = data[(i - 1) * cols + j];
      if (cur == prev) {
        curLen++;
        if (curLen > maxInCol)
          maxInCol = curLen;
      } else {
        curLen = 1;
      }
    }
    if (maxInCol > bestLen) {
      bestLen = maxInCol;
      bestCol = j + 1;
    }
  }
  return bestCol;
}

} // namespace ivanova

int main(int argc, char *argv[]) {
  using namespace ivanova;

  if (argc != 4) {
    std::cerr << "Error: need 3 arguments\n";
    return 1;
  }

  if (argv[1][0] != '1' && argv[1][0] != '2') {
    std::cerr << "Error: first argument must be 1 or 2\n";
    return 1;
  }
  if (argv[1][1] != '\0') {
    std::cerr << "Error: first argument must be 1 or 2\n";
    return 1;
  }
  int mode = argv[1][0] - '0';

  const char *inputFile = argv[2];
  const char *outputFile = argv[3];

  int rows = 0;
  int cols = 0;

  if (mode == 1) {
    int data[10000];
    if (!readMatrix(inputFile, rows, cols, data, 10000)) {
      std::cerr << "Error: cannot read matrix\n";
      return 2;
    }
    int result = findColumn(data, rows, cols);

    std::ofstream fout(outputFile);
    fout << rows << " " << cols << " " << result << "\n";
  } else {
    std::ifstream probe(inputFile);
    if (!probe.is_open()) {
      std::cerr << "Error: cannot open file\n";
      return 2;
    }

    static int temp[1000000];
    if (!readMatrix(inputFile, rows, cols, temp, 1000000)) {
      std::cerr << "Error: cannot read matrix\n";
      return 2;
    }

    int total = rows * cols;
    int *data = 0;
    if (total > 0) {
      data = new int[total];
    }

    for (int i = 0; i < total; i++) {
      data[i] = temp[i];
    }

    int result = findColumn(data, rows, cols);

    std::ofstream fout(outputFile);
    fout << rows << " " << cols << " " << result << "\n";

    if (data != 0) {
      delete[] data;
    }
  }

  return 0;
}
